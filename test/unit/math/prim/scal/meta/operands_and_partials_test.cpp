#include <stan/math/prim/scal/meta/operands_and_partials.hpp>
#include <gtest/gtest.h>

TEST(AgradPartialsVari, OperandsAndPartials_) {
  using stan::math::operands_and_partials;
  double d1 = 0.1;
  double d2 = 0.2;
  double d3 = 0.3;

  operands_and_partials<double> o1(d1);
  operands_and_partials<double, double, double> o2(d1, d2, d3);
  //std::cout << "hello" << std::endl << sizeof(stan::math::OperandsAndPartials<double, double, double>) << std::endl;
  //This is 6 bytes in old OperandsAndPartials
  //std::cout << "hello" << std::endl << sizeof(operands_and_partials<double, double, double>) << std::endl;
  //New operands_and_partials is 2 bytes, for the pointers to empty tuples :/

  SUCCEED() << "Construction should be ok.";
}

TEST(AgradPartialsVari, OperandsAndPartials_check_throw_) {
  using stan::math::operands_and_partials;

  double d;

  operands_and_partials<double, double, double> o1(d,d,d);
  EXPECT_THROW(o1.partials.get<0>(), std::logic_error);
  EXPECT_THROW(o1.partials.get<1>(), std::logic_error);
  EXPECT_THROW(o1.partials.get<2>(), std::logic_error);
}
