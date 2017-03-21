#ifndef STAN_MATH_PRIM_SCAL_META_MUTABLE_SCALAR_SEQ_VIEW_HPP
#define STAN_MATH_PRIM_SCAL_META_MUTABLE_SCALAR_SEQ_VIEW_HPP

#include <stan/math/prim/scal/meta/scalar_type.hpp>
#include <stan/math/prim/scal/meta/ellided.hpp>

namespace stan {
  namespace math {
    namespace detail {
    /**
     * mutable_scalar_seq_view provides a uniform sequence-like wrapper around either a
     * scalar or a sequence of scalars.
     *
     * @tparam C the container type; will be the scalar type if wrapping a scalar
     * @tparam T the scalar type
     */
    template <typename C, typename T = typename scalar_type<C>::type>
      class mutable_scalar_seq_view {
        public:
          typedef T scalar_t;
          explicit mutable_scalar_seq_view(C& c) : c_(c) {}

          /**
           * Segfaults if out of bounds.
           * @param i index
           * @return the element at the specified position in the container
           */
          T& operator[](int i) {
            return c_[i];
          }

          int size() {
            return c_.size();
          }

        private:
          C& c_;
      };

    /**
     * This specialization handles wrapping a scalar as if it were a sequence.
     *
     * @tparam T the scalar type
     */
    template <typename T>
      class mutable_scalar_seq_view<T, T> {
        public:
          typedef T scalar_t;
          explicit mutable_scalar_seq_view(T& t) : t_(t) {}

          T& operator[](int /* i */) {
            return t_;
          }

          int size() {
            return 1;
          }

        private:
          T& t_;
      };

    /**
     * Allow this class to pretend to wrap something, but store nothing.
     *
     * @tparam T the scalar type
     */
    template <typename T>
      class mutable_scalar_seq_view<detail::ellided<T>, T> {
        public:
          typedef T scalar_t;
          explicit mutable_scalar_seq_view(T& t){}

          T& operator[](int /* i */) {
            throw std::logic_error("can't access elements of an ellided thing");
          }

          int size() {
            return 0;
          }
      };
    } // end namespace detail
  } // end namespace math
} // end namespace stan
#endif
