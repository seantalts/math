#ifndef STAN_MATH_REV_SCAL_META_OPERANDS_AND_PARTIALS2_HPP
#define STAN_MATH_REV_SCAL_META_OPERANDS_AND_PARTIALS2_HPP

#include <stan/math/rev/core/vari.hpp>
#include <stan/math/rev/core/var.hpp>
#include <stan/math/prim/scal/meta/length.hpp>
#include <stan/math/prim/scal/meta/mutable_scalar_seq_view.hpp>
#include <stan/math/prim/scal/meta/scalar_seq_view.hpp>
#include <stan/math/prim/scal/meta/operands_and_partials.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/type_traits.hpp>

namespace stan {
  namespace math {
    namespace detail {
      double* alloc_partials(const var& vars) {
        return static_cast<double*> (vari::operator new
            (sizeof(double) * length(vars)));
      }
    } // end namespace detail

    template<typename T1, typename T2, typename T3>
    class operands_and_partials<T1, T2, T3, stan::math::var> {
      public:
        T1 op1

        stan::math::var value(double value) { throw std::logic_error("XXX need to do this"); }
    };
  }
}

#endif
