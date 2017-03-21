#ifndef STAN_MATH_PRIM_SCAL_META_OPERANDS_AND_PARTIALS_HPP
#define STAN_MATH_PRIM_SCAL_META_OPERANDS_AND_PARTIALS_HPP

#include <stan/math/prim/scal/meta/return_type.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/at.hpp>

namespace stan {
  namespace math {
    namespace internal {
      
      template<typename T1, typename T2, typename T3>
      class empty_tuple {
        public:
          typedef boost::mpl::vector<T1, T2, T3> types;

          template<int N>
          typename boost::mpl::at_c<types,N>::type get() {
            throw std::logic_error(".get() called on empty tuple");
          }
      };
    }

    template<typename T1,
      typename T2 = boost::tuples::null_type,
      typename T3 = boost::tuples::null_type,
      typename T_return_type = typename stan::return_type<T1, T2, T3>::type>
    class operands_and_partials {
      public:
        internal::empty_tuple<T1, T2, T3> operands;
        internal::empty_tuple<T1, T2, T3> partials;

        /* In the following, if I use add_reference then I can't construct an
         * o&p with just a double: op(1.1) for example. I can if I use
         * add_rvalue_reference, but I read up a bit on those and don't really
         * understand what tradeoff I'm making there when deciding what type of
         * reference to add. TODO: Figure this out in the pull request.
         */
        typedef typename boost::add_const<
              typename boost::add_reference<T1>::type>::type T1Ref;
        typedef typename boost::add_const<
              typename boost::add_reference<T2>::type>::type T2Ref;
        typedef typename boost::add_const<
              typename boost::add_reference<T3>::type>::type T3Ref;

        operands_and_partials() {}
        operands_and_partials(T1Ref t1) {}
        operands_and_partials(T1Ref t1, T2Ref t2) {}
        operands_and_partials(T1Ref t1, T2Ref t2, T3Ref t3) {}

        T_return_type value(double value) { return value; }
    };
  }
}
#endif
