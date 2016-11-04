#ifndef STAN_MATH_PRIM_SCAL_META_AD_PROMOTABLE_HPP
#define STAN_MATH_PRIM_SCAL_META_AD_PROMOTABLE_HPP

namespace stan {
  namespace math {

    /**
     * Template traits metaprogram to determine if a variable of one
     * template type can be promoted to a second target template
     * type. 
     * 
     * <p>It will delcare an enum <code>value</code> equal to
     * <code>false</code>.
     *
     * @tparam V promoted type
     * @tparam T target type
     */
    template <typename V, typename T>
    struct ad_promotable {
      enum { value = false };
    };

    template <typename T>
    struct ad_promotable<T, T> {
      enum { value = true };
    };

    template <>
    struct ad_promotable<long double, double> {
      enum { value = true };
    };

    template <>
    struct ad_promotable<double, double> {
      enum { value = true };
    };

    template <>
    struct ad_promotable<float, double> {
      enum { value = true };
    };

    template <>
    struct ad_promotable<long, double> {
      enum { value = true };
    };

    template <>
    struct ad_promotable<int, double> {
      enum { value = true };
    };

    template <>
    struct ad_promotable<short, double> {
      enum { value = true };
    };

    template <>
    struct ad_promotable<char, double> {
      enum { value = true };
    };

    template <>
    struct ad_promotable<bool, double> {
      enum { value = true };
    };

    template <>
    struct ad_promotable<unsigned long, double> {
      enum { value = true };
    };

    template <>
    struct ad_promotable<unsigned int, double> {
      enum { value = true };
    };

    template <>
    struct ad_promotable<unsigned short, double> {
      enum { value = true };
    };

    template <>
    struct ad_promotable<unsigned char, double> {
      enum { value = true };
    };

  }
}
#endif
