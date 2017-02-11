/*
 * Parallel ODE Prototype
 *
 * Note: We need an OpenMP capable compiler. clang on MacOS is not
 * openMP capable while g++ is. So make sure you use in the make/local
 *
 * CC=g++
 * CFLAGS += -DNDEBUG -fopenmp
 * CFLAGS_GTEST += -DNDEBUG -fopenmp
 *
 * Also, its recommended to export the variable on the shell
 *
 * OMP_NUM_THREADS=4
 *
 * to set the number of used cores to 4 should you have 4 CPUs on your
 * machine.
 */

#include <stan/math/rev/mat.hpp>
#include <stan/math/rev/core.hpp>

#include <gtest/gtest.h>

#include <vector>

#include <boost/numeric/odeint.hpp>

#include <test/unit/math/rev/mat/functor/oral_2cmt.hpp>
// load specialised template def to avoid usage of AD
#include <test/unit/math/rev/mat/functor/oral_2cmt_jacobian.hpp>

// this is a small diversion from integrate_ode_bdf which returns the
// coupled ode system as double only vector
#include <test/unit/math/rev/mat/functor/integrate_ode_bdf_bare.hpp>

#include <boost/random/mersenne_twister.hpp>

#include <stan/math.hpp>

struct plain_oral_2cmt_ode_fun : public oral_2cmt_ode_fun {};

#include <test/unit/util.hpp>


namespace stan {
  namespace math {

    // very simple OpenMP
    template <typename F, typename T0, typename T1, typename T3, typename T4>
    std::vector<std::vector<typename boost::math::tools::promote_args<T0, T1, T3, T4>::type> >
    integrate_ode_parallel(const F& f,
                           std::vector<std::vector<T0> >& y0,
                           const std::vector<T1>& t0,
                           const std::vector<int>& M, // intended to
                                                      // code a ragged
                                                      // structure in
                                                      // ts at some point
                           const std::vector<T3>& ts, // NOTE: take ts to
                           // be the same for all integrations,
                           // i.e. ignore M
                           const std::vector<std::vector<T4> >& theta,
                           const std::vector<double>& x_r,
                           const std::vector<int>& x_i,
                           std::ostream* pstream__) {
      int O = y0.size(); // number of ODEs
      int exceptions = 0;
      typedef std::vector<std::vector<typename boost::math::tools::promote_args<T0, T1, T3, T4>::type> > res_t;
      res_t res;
#pragma omp parallel
      {
        //std::cout << "Hello from thread " <<  omp_get_thread_num() << ", nthreads " << omp_get_num_threads() << std::endl;
      
#pragma omp for schedule(static,1) ordered
        for(int o = 0; o < O; ++o) {
          std::vector<std::vector<double> > y_coupled;
          // exceptions must not leave an openMP thread, we have to
          // work around this by catching and later on rethrowing
          try {
            if(exceptions == 0) {
              std::vector<double> xrun_r(1, x_r[o]);
              y_coupled = stan::math::integrate_ode_bdf_bare(f, y0[o], t0[o], ts, theta[o], xrun_r, x_i, pstream__);
            }
          } catch(const std::exception& e) {
            ++exceptions;
          }
#pragma omp ordered
          {
            if(exceptions == 0) {
              res_t run = stan::math::decouple_ode_states(y_coupled, y0[o], theta[o]);
              for(int m = 0; m < M[o]; ++m)
                res.push_back(run[m]);
            }
          }
        }
      }
      if (exceptions != 0)
        throw std::domain_error("ODE error");
      return(res);
    }

  }
}



struct StanOde_parallel : public ::testing::Test {
  std::stringstream msgs;
  int T;
  std::vector<double> ts;
  int J;
  std::vector<double> x_r;
  std::vector<int> x_int;
  std::vector<double> t0;
  std::vector<int> M;
  std::vector<std::vector<stan::math::var> > theta_v;
  std::vector<std::vector<stan::math::var> > y0_v;
  oral_2cmt_ode_fun f_;
  //plain_oral_2cmt_ode_fun f_ad_;

  StanOde_parallel() : T(1000), J(1000), t0(J, -1), M(J, T) {

    boost::random::mt19937 rng;

    rng.seed(45656);
    const double sdlog = 10.;
    
    for(int j = 0; j != J; ++j) {
      std::vector<stan::math::var> run_theta_v(4);

      run_theta_v[0] = log(2.)/1   * stan::math::lognormal_rng(0, sdlog, rng);
      run_theta_v[1] = log(2.)/70. * stan::math::lognormal_rng(0, sdlog, rng);
      run_theta_v[2] = log(2.)/20. * stan::math::lognormal_rng(0, sdlog, rng);
      run_theta_v[3] = log(2.)/50. * stan::math::lognormal_rng(0, sdlog, rng);

      theta_v.push_back(run_theta_v);

      std::vector<stan::math::var> run_y0_v(3);

      run_y0_v[0] = stan::math::lognormal_rng(10, sdlog, rng);
      run_y0_v[1] = stan::math::lognormal_rng(-3, sdlog, rng);
      run_y0_v[2] = stan::math::lognormal_rng(5, sdlog, rng);

      y0_v.push_back(run_y0_v);

      x_r.push_back( stan::math::lognormal_rng(3, sdlog, rng) );
    }

    for(int t = 0; t != T; ++t)
      ts.push_back(t);
  }
};

// test which will not use AD to construct the coupled system and run serially
TEST_F(StanOde_parallel, cvodes_2cmt_oral_serial) {

  std::vector<std::vector<stan::math::var> > res;

  for(int j = 0; j != J; ++j) {
    std::vector<std::vector<stan::math::var> > run;
    run = stan::math::integrate_ode_bdf(f_, y0_v[j], t0[j], ts, theta_v[j], std::vector<double>(1, x_r[j]), x_int, 0);

    for(int t = 0; t != T; ++t)
      res.push_back(run[t]);
  }
}

// test which will use AD to construct the coupled system and run in
// parallel with openMP which works if compiler is g++ and the option
// -fopenmp is given. Otherwise it will just run serially.
// By default all processes will be used (including fake
// hyper-threaded ones). So it is recommended to set and export
// OMP_NUM_THREADS=4 if you have 4 real CPUs
TEST_F(StanOde_parallel, cvodes_2cmt_oral_parallel) {

  std::vector<std::vector<stan::math::var> > res;
  
  res = stan::math::integrate_ode_parallel(f_, y0_v, t0, M, ts, theta_v, x_r, x_int, 0);
}
