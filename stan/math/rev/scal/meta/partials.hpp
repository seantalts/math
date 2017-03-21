#ifndef STAN_MATH_REV_SCAL_META_PARTIALS_HPP
#define STAN_MATH_REV_SCAL_META_PARTIALS_HPP

#include <stan/math/rev/core/var.hpp>
#include <stan/math/prim/scal/meta/partials.hpp>

namespace stan {
  namespace math {
    namespace detail {
      template <>
      class partials<var> {
        public:
          explicit partials(var& vars):
            storage(static_cast<double*>
                (vari::operator new (sizeof(double) * length(vars)))) {}

          double& operator[](int i) {
            return storage[i];
          }

          int size() {
            return 0;
          }

        private:
          double* storage;
      };
    } // end namespace detail
  } // end namespace math
} // end namespace stan
#endif
