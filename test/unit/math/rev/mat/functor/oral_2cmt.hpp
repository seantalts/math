#ifndef TEST_UNIT_MATH_REV_MAT_FUNCTOR_ORAL_2CMT_HPP
#define TEST_UNIT_MATH_REV_MAT_FUNCTOR_ORAL_2CMT_HPP

#include <stan/math/rev/core.hpp>

#include <stan/math/prim/scal/prob/normal_lpdf.hpp>

struct oral_2cmt_ode_fun {
  template <typename T0, typename T1, typename T2>
  inline
  std::vector<typename stan::return_type<T1,T2>::type>
  operator()(const T0& t_in, // initial time
             const std::vector<T1>& y, //initial positions
             const std::vector<T2>& parms, // parameters
             const std::vector<double>& sx, // double data
             const std::vector<int>& sx_int,
             std::ostream* msgs) const { // integer data
    std::vector<typename stan::return_type<T1,T2>::type> dydt(3);

    const T2 ka  = parms[0];
    const T2 k10 = parms[1];
    const T2 k12 = parms[2];
    const T2 k21 = parms[3];

    const double dose = sx[0];

    // add a dose at time-point 24 over a time-span of 0.5 time-units
    dydt[0] = -ka * y[0] + dose * exp(stan::math::normal_lpdf(t_in, 24., 0.5/4.));
    dydt[2] = k12 * y[1] - k21 * y[2];
    dydt[1] = ka * y[0] - k10 * y[1] -dydt[2];

    return(dydt);
  }
};

#endif
