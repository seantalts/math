#include <stan/math/rev/core.hpp>
#include <stan/math/rev/scal/meta/operands_and_partials.hpp>
#include <gtest/gtest.h>

TEST(AgradPartialsVari,OperandsAndPartials1) {
  using stan::math::vari;
  using stan::math::operands_and_partials;
  using stan::math::var;

  var x = 2.0;
  var z = -3.0 * x;  // dz/dx = -3
  operands_and_partials<var> o(z);
  o.partials.get<0>()[0] += 5.0;  // dy/dz = 5

  var y = o.value(-1.0);

  stan::math::grad(y.vi_);
  EXPECT_FLOAT_EQ(-15.0, x.adj());  // dy/dx = -15
}
TEST(AgradPartialsVari,OperandsAndPartials2) {
  using stan::math::operands_and_partials;
  using stan::math::vari;
  using stan::math::var;

  var x1 = 2.0;
  var x2 = 3.0;
  var z1 = -5.0 * x1; // dz1/dx1 = -5
  var z2 = -7.0 * x2; // dz2/dx2 = -7
  operands_and_partials<var,var> o(z1,z2);
  o.partials.get<0>()[0] += 11.0;  // dy/dz1 = 11.0
  o.partials.get<1>()[0] += 13.0;  // dy/dz2 = 13.0
  var y = o.value(-1.0);

  stan::math::grad(y.vi_);
  EXPECT_FLOAT_EQ(-55.0, x1.adj());  // dy/dx1 = -55
  EXPECT_FLOAT_EQ(-91.0, x2.adj());  // dy/dx2 = -91
}
TEST(AgradPartialsVari, OperandsAndPartials3) {
  using stan::math::operands_and_partials;
  using stan::math::vari;
  using stan::math::var;

  var x1 = 2.0;
  var x2 = 3.0;
  var x3 = 5.0;
  var z1 = -7.0 * x1;  // dz1/dx1 = -5
  var z2 = -9.0 * x2;  // dz2/dx2 = -7
  var z3 = -11.0 * x3; // dz3/dx3 = -11

  operands_and_partials<var,var,var> o(z1, z2, z3);
  o.partials.get<0>()[0] += 17.0;  // dy/dz1 = 17.0
  o.partials.get<1>()[0] += 19.0;  // dy/dz2 = 19.0
  o.partials.get<2>()[0] += 23.0;  // dy/dz3 = 23.0
  var y = o.value(-1.0);

  stan::math::grad(y.vi_);
  EXPECT_FLOAT_EQ(-119.0, x1.adj());  // dy/dx1 = -119
  EXPECT_FLOAT_EQ(-171.0, x2.adj());  // dy/dx2 = -133
  EXPECT_FLOAT_EQ(-253.0, x3.adj());  // dy/dx2 = -253
}
TEST(AgradPartialsVari, OperandsAndPartials_check_throw) {
  using stan::math::operands_and_partials;
  using stan::math::var;
  
  var v;

  operands_and_partials<var,var,var> o2(v,v,v);
  EXPECT_NO_THROW(o2.partials.get<0>()[0]);
  EXPECT_NO_THROW(o2.partials.get<1>()[0]);
  EXPECT_NO_THROW(o2.partials.get<2>()[0]);
}
