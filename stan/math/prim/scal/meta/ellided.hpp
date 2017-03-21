#ifndef STAN_MATH_PRIM_SCAL_META_ELLIDED_HPP
#define STAN_MATH_PRIM_SCAL_META_ELLIDED_HPP

namespace stan {
  namespace math {
    namespace detail {
  /**
    * Empty struct for use in 
    * boost\::condtional\<is_constant_struct\<T1\>\::value, T1, ellided\>\::type
    * as false condtion for safe indexing 
    *
    */
  template<typename T>
  struct ellided { 
    ellided(T t) {}
  };

    } // end namespace detail
  } //end namespace math
} // end namespace stan

#endif
