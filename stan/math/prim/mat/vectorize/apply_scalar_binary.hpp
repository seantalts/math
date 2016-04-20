#ifndef STAN_MATH_PRIM_MAT_VECTORIZE_APPLY_BINARY_SCALAR_HPP
#define STAN_MATH_PRIM_MAT_VECTORIZE_APPLY_BINARY_SCALAR_HPP

#include <stan/math/prim/scal/err/check_equal.hpp>
#include <Eigen/Dense>
#include <vector>

namespace stan {

  namespace math {

    /**
     * Base template class for vectorization of unary scalar functions
     * defined by a template class <code>F</code> to a scalar,
     * standard library vector, or Eigen dense matrix expression
     * template.  
     *
     * <p>The base class applies to any Eigen dense matrix expression
     * template.  Specializations define applications to scalars
     * (primitive or autodiff in the corresponding autodiff library
     * directories) or to standard library vectors of vectorizable
     * types (primitives, Eigen dense matrix expressions, or further
     * standard vectors).
     *
     * <p>Each specialization must define the typedef
     * <code>return_t</code> for the vectorized return type and the
     * function <code>apply</code> which defines the vectorization or
     * base application of the function defined statically by the
     * class F.  The function definition class F defines a static
     * function <code>fun()</code>, which defines the function's
     * behavior on scalars.
     *
     * @tparam F Type of function to apply.
     * @tparam T Type of argument to which function is applied.
     */
    template <typename F, typename T1, typename T2>
    struct apply_scalar_binary {
      /**
       * Type of underlying scalar for the matrix type T.
       */
      typedef typename Eigen::internal::traits<T1>::Scalar scalar_t1;
      typedef typename Eigen::internal::traits<T2>::Scalar scalar_t2;

      /**
       * Return type for applying the function elementwise to a matrix
       * expression template of type T.
       */
      typedef Eigen::Matrix<typename apply_scalar_binary<
                            F, scalar_t1, scalar_t2>::return_t, 
                            T1::RowsAtCompileTime, T1::ColsAtCompileTime>
      return_t;

      /**
       * Return the result of applying the function defined by the
       * template parameter F to the specified matrix argument. 
       *
       * @param x Matrix to which operation is applied.
       * @return Componentwise application of the function specified
       * by F to the specified matrix.
       */
      static inline return_t apply(const T1& x, const T2& y) {
        using stan::math::check_equal;
        check_equal("binary vectorization", "rows", x.rows(), y.rows()); 
        check_equal("binary vectorization", "cols", x.cols(), y.cols()); 
        return_t result(x.rows(), x.cols());
        for (int j = 0; j < x.cols(); ++j)
          for (int i = 0; i < x.rows(); ++i)
            result(i, j) = apply_scalar_binary<F, scalar_t1, scalar_t2>::
                           apply(x(i, j), y(i, j));
        return result;
      }
    };

    /**
     * Base template class for vectorization of unary scalar functions
     * defined by a template class <code>F</code> to a scalar,
     * standard library vector, or Eigen dense matrix expression
     * template.  
     *
     * <p>The base class applies to any Eigen dense matrix expression
     * template.  Specializations define applications to scalars
     * (primitive or autodiff in the corresponding autodiff library
     * directories) or to standard library vectors of vectorizable
     * types (primitives, Eigen dense matrix expressions, or further
     * standard vectors).
     *
     * <p>Each specialization must define the typedef
     * <code>return_t</code> for the vectorized return type and the
     * function <code>apply</code> which defines the vectorization or
     * base application of the function defined statically by the
     * class F.  The function definition class F defines a static
     * function <code>fun()</code>, which defines the function's
     * behavior on scalars.
     *
     * @tparam F Type of function to apply.
     * @tparam T Type of argument to which function is applied.
     */
    template <typename F, typename T>
    struct apply_scalar_binary<F, int, T> {
      /**
       * Type of underlying scalar for the matrix type T.
       */
      typedef typename Eigen::internal::traits<T>::Scalar scalar_t;

      /**
       * Return type for applying the function elementwise to a matrix
       * expression template of type T.
       */
      typedef Eigen::Matrix<typename apply_scalar_binary<
                            F, int, scalar_t>::return_t,
                            T::RowsAtCompileTime, T::ColsAtCompileTime>
      return_t;

      /**
       * Return the result of applying the function defined by the
       * template parameter F to the specified matrix argument. 
       *
       * @param x Matrix to which operation is applied.
       * @return Componentwise application of the function specified
       * by F to the specified matrix.
       */
      static inline return_t apply(int x, const T& y) {
        return_t result(y.rows(), y.cols());
        for (int j = 0; j < y.cols(); ++j)
          for (int i = 0; i < y.rows(); ++i)
            result(i, j) = apply_scalar_binary<F, int, scalar_t>::
                           apply(x, y(i, j));
        return result;
      }
    };

    /**
     * Base template class for vectorization of unary scalar functions
     * defined by a template class <code>F</code> to a scalar,
     * standard library vector, or Eigen dense matrix expression
     * template.  
     *
     * <p>The base class applies to any Eigen dense matrix expression
     * template.  Specializations define applications to scalars
     * (primitive or autodiff in the corresponding autodiff library
     * directories) or to standard library vectors of vectorizable
     * types (primitives, Eigen dense matrix expressions, or further
     * standard vectors).
     *
     * <p>Each specialization must define the typedef
     * <code>return_t</code> for the vectorized return type and the
     * function <code>apply</code> which defines the vectorization or
     * base application of the function defined statically by the
     * class F.  The function definition class F defines a static
     * function <code>fun()</code>, which defines the function's
     * behavior on scalars.
     *
     * @tparam F Type of function to apply.
     * @tparam T Type of argument to which function is applied.
     */
    template <typename F, typename T>
    struct apply_scalar_binary<F, double, T> {
      /**
       * Type of underlying scalar for the matrix type T.
       */
      typedef typename Eigen::internal::traits<T>::Scalar scalar_t;

      /**
       * Return type for applying the function elementwise to a matrix
       * expression template of type T.
       */
      typedef Eigen::Matrix<typename apply_scalar_binary<
                            F, double, scalar_t>::return_t,
                            T::RowsAtCompileTime, T::ColsAtCompileTime>
      return_t;

      /**
       * Return the result of applying the function defined by the
       * template parameter F to the specified matrix argument. 
       *
       * @param x Matrix to which operation is applied.
       * @return Componentwise application of the function specified
       * by F to the specified matrix.
       */
      static inline return_t apply(double x, const T& y) {
        return_t result(y.rows(), y.cols());
        for (int j = 0; j < y.cols(); ++j)
          for (int i = 0; i < y.rows(); ++i)
            result(i, j) = apply_scalar_binary<F, double, scalar_t>::
                           apply(x, y(i, j));
        return result;
      }
    };

    /**
     * Base template class for vectorization of unary scalar functions
     * defined by a template class <code>F</code> to a scalar,
     * standard library vector, or Eigen dense matrix expression
     * template.  
     *
     * <p>The base class applies to any Eigen dense matrix expression
     * template.  Specializations define applications to scalars
     * (primitive or autodiff in the corresponding autodiff library
     * directories) or to standard library vectors of vectorizable
     * types (primitives, Eigen dense matrix expressions, or further
     * standard vectors).
     *
     * <p>Each specialization must define the typedef
     * <code>return_t</code> for the vectorized return type and the
     * function <code>apply</code> which defines the vectorization or
     * base application of the function defined statically by the
     * class F.  The function definition class F defines a static
     * function <code>fun()</code>, which defines the function's
     * behavior on scalars.
     *
     * @tparam F Type of function to apply.
     * @tparam T Type of argument to which function is applied.
     */
    template <typename F, typename T>
    struct apply_scalar_binary<F, T, int> {
      /**
       * Type of underlying scalar for the matrix type T.
       */
      typedef typename Eigen::internal::traits<T>::Scalar scalar_t;

      /**
       * Return type for applying the function elementwise to a matrix
       * expression template of type T.
       */
      typedef Eigen::Matrix<typename apply_scalar_binary<
                            F, scalar_t, int>::return_t,
                            T::RowsAtCompileTime, T::ColsAtCompileTime>
      return_t;

      /**
       * Return the result of applying the function defined by the
       * template parameter F to the specified matrix argument. 
       *
       * @param x Matrix to which operation is applied.
       * @return Componentwise application of the function specified
       * by F to the specified matrix.
       */
      static inline return_t apply(const T& x, int y) {
        return_t result(x.rows(), x.cols());
        for (int j = 0; j < x.cols(); ++j)
          for (int i = 0; i < x.rows(); ++i)
            result(i, j) = apply_scalar_binary<F, scalar_t, int>::
                           apply(x(i, j), y);
        return result;
      }
    };

    /**
     * Base template class for vectorization of unary scalar functions
     * defined by a template class <code>F</code> to a scalar,
     * standard library vector, or Eigen dense matrix expression
     * template.  
     *
     * <p>The base class applies to any Eigen dense matrix expression
     * template.  Specializations define applications to scalars
     * (primitive or autodiff in the corresponding autodiff library
     * directories) or to standard library vectors of vectorizable
     * types (primitives, Eigen dense matrix expressions, or further
     * standard vectors).
     *
     * <p>Each specialization must define the typedef
     * <code>return_t</code> for the vectorized return type and the
     * function <code>apply</code> which defines the vectorization or
     * base application of the function defined statically by the
     * class F.  The function definition class F defines a static
     * function <code>fun()</code>, which defines the function's
     * behavior on scalars.
     *
     * @tparam F Type of function to apply.
     * @tparam T Type of argument to which function is applied.
     */
    template <typename F, typename T>
    struct apply_scalar_binary<F, T, double> {
      /**
       * Type of underlying scalar for the matrix type T.
       */
      typedef typename Eigen::internal::traits<T>::Scalar scalar_t;

      /**
       * Return type for applying the function elementwise to a matrix
       * expression template of type T.
       */
      typedef Eigen::Matrix<typename apply_scalar_binary<
                            F, scalar_t, double>::return_t,
                            T::RowsAtCompileTime, T::ColsAtCompileTime>
      return_t;

      /**
       * Return the result of applying the function defined by the
       * template parameter F to the specified matrix argument. 
       *
       * @param x Matrix to which operation is applied.
       * @return Componentwise application of the function specified
       * by F to the specified matrix.
       */
      static inline return_t apply(const T& x, double y) {
        return_t result(x.rows(), x.cols());
        for (int j = 0; j < x.cols(); ++j)
          for (int i = 0; i < x.rows(); ++i)
            result(i, j) = apply_scalar_binary<F, scalar_t, double>::
                           apply(x(i, j), y);
        return result;
      }
    };

    /**
     * Template specialization for vectorized functions applying to
     * int and double arguments.
     *
     * @tparam F Type of function defining static apply function.
     */
    template <typename F>
    struct apply_scalar_binary<F, int, double> {
      /**
       * The return type, double.
       */
      typedef double return_t;

      /**
       * Apply the function specified by F to the specified argument.
       * This is defined through a direct application of
       * <code>F::fun()</code>, which must be defined for int and double
       * arguments. 
       *
       * @param x Int argument scalar.
       * @param y Double argument scalar.
       * @return Result of applying F to the scalar.
       */
      static inline return_t apply(int x, double y) {
        return F::fun(static_cast<double>(x), y);
      }
    };

    /**
     * Template specialization for vectorized functions applying to
     * double and int arguments.
     *
     * @tparam F Type of function defining static apply function.
     */
    template <typename F>
    struct apply_scalar_binary<F, double, int> {
      /**
       * The return type, double.
       */
      typedef double return_t;

      /**
       * Apply the function specified by F to the specified argument.
       * This is defined through a direct application of
       * <code>F::fun()</code>, which must be defined for double
       * and int arguments. 
       *
       * @param x Double argument scalar.
       * @param y Int argument scalar.
       * @return Result of applying F to the scalar.
       */
      static inline return_t apply(double x, int y) {
        return F::fun(x, static_cast<double>(y));
      }
    };

    /**
     * Template specialization for vectorized functions applying to
     * double and double arguments.
     *
     * @tparam F Type of function defining static apply function.
     */
    template <typename F>
    struct apply_scalar_binary<F, double, double> {
      /**
       * The return type, double.
       */
      typedef double return_t;

      /**
       * Apply the function specified by F to the specified argument.
       * This is defined through a direct application of
       * <code>F::fun()</code>, which must be defined for double
       * arguments. 
       *
       * @param x Double argument scalar.
       * @param y Double argument scalar.
       * @return Result of applying F to the scalar.
       */
      static inline return_t apply(double x, double y) {
        return F::fun(x, y);
      }
    };

    /**
     * Template specialization for vectorized functions applying to
     * integer arguments.  Although the argument is integer, the
     * return type is specified as double.  This allows promotion of
     * integers to doubles in vectorized functions, or in containers.  
     *
     * @tparam F Type of function defining static apply function.
     */
    template <typename F>
    struct apply_scalar_binary<F, int, int> {
      /**
       * The return type, double.
       */
      typedef double return_t;

      /**
       * Apply the function specified by F to the specified argument.
       * This is defined through a direct application of
       * <code>F::fun()</code>, which must be defined for double
       * arguments. 
       *
       * @param x Int argument scalar.
       * @param y Int argument scalar.
       * @return Result of applying F to the scalar.
       */
      static inline return_t apply(int x, int y) {
        return F::fun(static_cast<double>(x), static_cast<double>(y));
      }
    };

    /**
     * Template specialization for vectorized functions applying to
     * standard vector containers.  The lowest-level scalar type of
     * the argument will determine the return type.  Integers are
     * promoted to double values.
     *
     * @tparam F Class defining a static apply function.
     * @tparam T Type of element contained in standard vector.
     */
    template <typename F, typename T>
    struct apply_scalar_binary<F, int, std::vector<T> > {
      /**
       * Return type, which is calculated recursively as a standard
       * vector of the return type of the contained type T.
       */
      typedef typename std::vector<
              typename apply_scalar_binary<F, int, T>::return_t>
      return_t;

      /**
       * Apply the function specified by F elementwise to the
       * specified argument.  This is defined recursively through this
       * class applied to elements of type T.
       *
       * @param x Argument container.
       * @return Elementwise application of F to the elements of the
       * container.
       */
      static inline return_t apply(int x, const std::vector<T>& y) {
        return_t fx(y.size());
        for (size_t i = 0; i < y.size(); ++i)
          fx[i] = apply_scalar_binary<F, int, T>::apply(x, y[i]);
        return fx;
      }
    };

    /**
     * Template specialization for vectorized functions applying to
     * standard vector containers.  The lowest-level scalar type of
     * the argument will determine the return type.  Integers are
     * promoted to double values.
     *
     * @tparam F Class defining a static apply function.
     * @tparam T Type of element contained in standard vector.
     */
    template <typename F, typename T>
    struct apply_scalar_binary<F, double, std::vector<T> > {
      /**
       * Return type, which is calculated recursively as a standard
       * vector of the return type of the contained type T.
       */
      typedef typename std::vector<
              typename apply_scalar_binary<F, double, T>::return_t>
      return_t;

      /**
       * Apply the function specified by F elementwise to the
       * specified argument.  This is defined recursively through this
       * class applied to elements of type T.
       *
       * @param x Argument container.
       * @return Elementwise application of F to the elements of the
       * container.
       */
      static inline return_t apply(double x, const std::vector<T>& y) {
        return_t fx(y.size());
        for (size_t i = 0; i < y.size(); ++i)
          fx[i] = apply_scalar_binary<F, double, T>::apply(x, y[i]);
        return fx;
      }
    };

    /**
     * Template specialization for vectorized functions applying to
     * standard vector containers.  The lowest-level scalar type of
     * the argument will determine the return type.  Integers are
     * promoted to double values.
     *
     * @tparam F Class defining a static apply function.
     * @tparam T Type of element contained in standard vector.
     */
    template <typename F, typename T>
    struct apply_scalar_binary<F, std::vector<T>, int> {
      /**
       * Return type, which is calculated recursively as a standard
       * vector of the return type of the contained type T.
       */
      typedef typename std::vector<
              typename apply_scalar_binary<F, T, int>::return_t>
      return_t;

      /**
       * Apply the function specified by F elementwise to the
       * specified argument.  This is defined recursively through this
       * class applied to elements of type T.
       *
       * @param x Argument container.
       * @return Elementwise application of F to the elements of the
       * container.
       */
      static inline return_t apply(const std::vector<T>& x, int y) {
        return_t fx(x.size());
        for (size_t i = 0; i < x.size(); ++i)
          fx[i] = apply_scalar_binary<F, T, int>::apply(x[i], y);
        return fx;
      }
    };

    /**
     * Template specialization for vectorized functions applying to
     * standard vector containers.  The lowest-level scalar type of
     * the argument will determine the return type.  Integers are
     * promoted to double values.
     *
     * @tparam F Class defining a static apply function.
     * @tparam T Type of element contained in standard vector.
     */
    template <typename F, typename T>
    struct apply_scalar_binary<F, std::vector<T>, double> {
      /**
       * Return type, which is calculated recursively as a standard
       * vector of the return type of the contained type T.
       */
      typedef typename std::vector<
              typename apply_scalar_binary<F, T, double>::return_t>
      return_t;

      /**
       * Apply the function specified by F elementwise to the
       * specified argument.  This is defined recursively through this
       * class applied to elements of type T.
       *
       * @param x Argument container.
       * @return Elementwise application of F to the elements of the
       * container.
       */
      static inline return_t apply(const std::vector<T>& x, double y) {
        return_t fx(x.size());
        for (size_t i = 0; i < x.size(); ++i)
          fx[i] = apply_scalar_binary<F, T, double>::apply(x[i], y);
        return fx;
      }
    };

    /**
     * Template specialization for vectorized functions applying to
     * standard vector containers.  The lowest-level scalar type of
     * the argument will determine the return type.  Integers are
     * promoted to double values.
     *
     * @tparam F Class defining a static apply function.
     * @tparam T Type of element contained in standard vector.
     */
    template <typename F, typename T1, typename T2>
    struct apply_scalar_binary<F, std::vector<T1>, std::vector<T2> > {
      /**
       * Return type, which is calculated recursively as a standard
       * vector of the return type of the contained type T.
       */
      typedef typename 
      std::vector<typename apply_scalar_binary<F, T1, T2>::return_t>
      return_t;

      /**
       * Apply the function specified by F elementwise to the
       * specified argument.  This is defined recursively through this
       * class applied to elements of type T.
       *
       * @param x Argument container.
       * @return Elementwise application of F to the elements of the
       * container.
       */
      static inline return_t apply(const std::vector<T1>& x,
                                   const std::vector<T2>& y) {
        stan::math::check_equal("apply_scalar_binary", "vector size", 
                                x.size(), y.size());
        return_t fx(x.size());
        for (size_t i = 0; i < x.size(); ++i)
          fx[i] = apply_scalar_binary<F, T1, T2>::apply(x[i], y[i]);
        return fx;
      }
    };

  }
}
#endif