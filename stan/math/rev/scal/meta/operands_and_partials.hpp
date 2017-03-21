#ifndef STAN_MATH_REV_SCAL_META_OPERANDS_AND_PARTIALS_HPP
#define STAN_MATH_REV_SCAL_META_OPERANDS_AND_PARTIALS_HPP

#include <stan/math/rev/core/vari.hpp>
#include <stan/math/rev/core/var.hpp>
#include <stan/math/prim/scal/meta/ellided.hpp>
#include <stan/math/prim/scal/meta/length.hpp>
#include <stan/math/prim/scal/meta/mutable_scalar_seq_view.hpp>
#include <stan/math/prim/scal/meta/operands_and_partials.hpp>
#include <stan/math/rev/scal/meta/partials.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/type_traits.hpp>

#include <iostream>
#include <typeinfo>

namespace stan {
  namespace math {
    namespace detail {
      template<typename T>
      struct const_ref {
        /*
        typedef typename boost::add_const<typename boost::add_reference<T>
          ::type>::type type;
          */
        typedef typename boost::add_reference<T>::type type;
      };
      template<>
      struct const_ref<boost::tuples::null_type> {
        typedef boost::tuples::null_type type;
      };

      /* In the following helper structs we just
       * need to make sure that we don't decorate null_type, which is the \0
       * that terminates a tuple and is also used as the sentinel value for
       * operands_and_partials to signal the end of the variables we care about.
       */
      template<typename T>
      struct seq_of {
        typedef mutable_scalar_seq_view<ellided<T>, T> type;
      };
      template<>
      struct seq_of<boost::tuples::null_type> {
        typedef boost::tuples::null_type type;
      };
      template<>
      struct seq_of<var &> {
        typedef mutable_scalar_seq_view<var &> type;
      };

      inline const boost::tuples::null_type cnull() {
        return boost::tuples::null_type();
      }
    } // end namespace detail

    namespace {
      using boost::is_same;
      using boost::tuples::null_type;
      using boost::tuples::element;

      void chain_(detail::mutable_scalar_seq_view<var&, var&> vars,
          const double& adj, detail::partials<var> partials) {
        for (int n = 0; n < vars.size(); ++n) {
          vars[n]->adj_ += adj * partials[n];
        }
      }

      template<typename O, typename P>
      class partials_vari : public vari {
      private:
        O operands;
        P partials;
      public:
        partials_vari(double value, O operands, P partials)
          : vari(value), operands(operands), partials(partials) {}
        void chain() {
          if (!is_same<typename O::head_type, null_type>::value) {
            chain_(operands.template get<0>(), adj_, partials.template get<0>());
            std::cout << "SOUP: " << val_ << std::endl;
            std::cout << "hello: " << typeid(typename O::tail_type).name() << std::endl;
            std::cout << "hello2: " << boost::is_same<typename O::tail_type, null_type>::value << std::endl;
            /*
            if (!is_same<typename O::tail_type, null_type>::value) {
              chain_(operands.template get<1>(),
                  adj_, partials.template get<1>());
              if (!is_same<typename O::tail_type::tail_type,
                  null_type>::value) {
                chain_(operands.template get<2>(),
                    adj_, partials.template get<2>());
              }
            }
            */
          }
        }
      };

      template <typename O, typename P>
      var partials_to_var(double logp, O operands, P partials) {
        return var(new partials_vari<O, P>(logp, operands, partials));
      }
    }

    template<typename T1, typename T2, typename T3>
    class operands_and_partials<T1, T2, T3, stan::math::var> {
      public:
        /* In the following, if I use add_reference then I can't construct an
         * o&p with just a double: o&p(1.1) for example. I can if I use
         * add_rvalue_reference, but I read up a bit on those and don't really
         * understand what tradeoff I'm making there when deciding what type of
         * reference to add. TODO: Figure this out in the pull request.
         */
        typedef typename detail::const_ref<T1>::type T1Ref;
        typedef typename detail::const_ref<T2>::type T2Ref;
        typedef typename detail::const_ref<T3>::type T3Ref;
        typedef typename detail::seq_of<T1Ref>::type T1Ops;
        typedef typename detail::seq_of<T2Ref>::type T2Ops;
        typedef typename detail::seq_of<T3Ref>::type T3Ops;
        boost::tuple<T1Ops, T2Ops, T3Ops> operands;
        boost::tuple<detail::partials<T1>,
          detail::partials<T2>, detail::partials<T3> > partials;

        operands_and_partials(T1Ref t1 = detail::cnull(),
            T2Ref t2 = detail::cnull(),
            T3Ref t3 = detail::cnull()):
          operands(boost::make_tuple(T1Ops(t1), T2Ops(t2), T3Ops(t3))),
          partials(boost::make_tuple(detail::partials<T1>(t1),
                detail::partials<T2>(t2), detail::partials<T3>(t3))) {}

        var value(double value) {
          return partials_to_var(value, operands, partials);
        }
    };
  }
}

#endif
