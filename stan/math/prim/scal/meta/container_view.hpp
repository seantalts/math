#ifndef STAN_MATH_PRIM_SCAL_META_CONTAINER_VIEW_HPP
#define STAN_MATH_PRIM_SCAL_META_CONTAINER_VIEW_HPP

#include <stan/math/prim/scal/meta/scalar_type.hpp>
#include <stan/math/prim/scal/meta/ellided.hpp>
#include <stdexcept>

namespace stan {
  namespace math {

    /**
     * Primary template class for container view of
     * array y with same structure as T1 and 
     * size as x
     *
     * @tparam T1 type of view.
     * @tparam T2 type of scalar returned by view.
     */
    template <typename T1, typename T2>
    class container_view {
    public:
      /**
       * Constructor
       *
       * @param x object from which size is to be inferred
       * @param y underlying array 
       */
      container_view(const T1& x) : y_(y) { }

      /**
       * operator[](int i) returns reference to view, 
       * indexed by i
       * Specialization handle appropriate broadcasting
       * if size of x is 1
       *
       * @param i index
       */
      T2& operator[](int i) {
        return y_[0];
      }
    private:
      T2* y_;
    };

    /**
     * Ellided type specialization, used in
     * conjunction with struct ellided as 
     * described above
     *
     * @tparam T2 type of scalar returned by view
     */
    template <typename T1, typename T2>
    class container_view<detail::ellided<T1>, T2> {
    public:
      typedef typename stan::scalar_type<T2>::type scalar_t;

      /**
       * Nothing initialized
       *
       * @param x input object
       * @param y underlying array 
       */
      container_view(const T1& x, T2 y) { }

      /**
       * operator[](int i)
       *
       * @param n index
       * @throw std::out_of_range regardless of the input.
       */
      scalar_t operator[](int n) const {
        throw std::out_of_range("can't access ellided elements.");
      }
    };
  }
}

#endif
