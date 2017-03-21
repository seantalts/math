#include <stan/math/rev/arr.hpp>
#include <gtest/gtest.h>
#include <test/unit/util.hpp>

using stan::scalar_type;
using stan::math::var;
using std::vector;

TEST(MetaTraits,ScalarTypeRev) {
  test::expect_same_type<var, scalar_type<var>::type>();
  test::expect_same_type<var, scalar_type<vector<var> >::type>();
  test::expect_same_type<double, scalar_type<vector<double> >::type>();
}
