#include <stan/math/rev/arr.hpp>
#include <gtest/gtest.h>
#include <test/unit/util.hpp>

using stan::return_type;
using stan::math::var;
using std::vector;

TEST(MetaTraits, ReturnTypeVectorVar) {
  test::expect_same_type<var, return_type<var>::type>();
  test::expect_same_type<var, return_type<vector<var> >::type>();
  test::expect_same_type<var, return_type<double, vector<var> >::type>();
  test::expect_same_type<double, return_type<double, vector<double> >::type>();
  test::expect_same_type<var, return_type<double, var >::type>();
}
