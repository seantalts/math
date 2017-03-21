#ifndef STAN_MATH_PRIM_SCAL_META_PARTIALS_HPP
#define STAN_MATH_PRIM_SCAL_META_PARTIALS_HPP

namespace stan {
  namespace math {
    namespace detail {
      template <typename T>
      class partials {
        public:
          explicit partials(T& t) {}

          double& operator[](int i) {
            throw std::logic_error("No partial derivatives of constants");
          }

          int size() {
            return 0;
          }
      };
    } // end namespace detail
  } // end namespace math
} // end namespace stan
#endif
