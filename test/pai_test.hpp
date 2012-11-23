#ifndef FEVERMJ_TEST_PAI_TSET_HPP_
#define FEVERMJ_TSET_PAI_TSET_HPP_
#include "gtest/gtest.h"
#include "../model/pai/pai.hpp"
#include "../model/pai/dora.hpp"

TEST(Pai, Operators) {
  using namespace FeverMJ::Model::Pai;
  Pai p = Pai::Sozu1();
  ASSERT_TRUE(p == Pai::Sozu1());
  ASSERT_FALSE(p == Pai::Pinzu2());
  ASSERT_TRUE(p != Pai::Manzu1());
  ASSERT_FALSE(p != Pai::Sozu1());
  ASSERT_TRUE(p < Pai::Sozu8());
  ASSERT_TRUE(Pai::Pinzu9() < p);
  ASSERT_FALSE(Pai::Sozu1() < p);
  ASSERT_TRUE(p <= Pai::Ton());
  ASSERT_TRUE(Pai::Sozu1() <= p);
  ASSERT_TRUE(Pai::Pinzu9() < p);
  ASSERT_TRUE(Pai::Pe() >= p);
  ASSERT_TRUE(Pai::Sozu1() >= p);
  ASSERT_FALSE(Pai::Pinzu9() >= p);
  ASSERT_TRUE(p > Pai::Pinzu9());
  ASSERT_FALSE(p > Pai::Sozu1());
  ASSERT_TRUE(Pai::Sozu2() > p); 
}

TEST(Pai, DorahyoToDora) {
  using namespace FeverMJ::Model::Pai;
  ASSERT_EQ(DorahyoToDora(Pai::Manzu1()), Pai::Manzu9());
  ASSERT_EQ(DorahyoToDora(Pai::Manzu9()), Pai::Manzu1());
  ASSERT_EQ(DorahyoToDora(Pai::Pinzu1()), Pai::Pinzu2());
  ASSERT_EQ(DorahyoToDora(Pai::Pinzu5()), Pai::Pinzu6());
  ASSERT_EQ(DorahyoToDora(Pai::Pinzu9()), Pai::Pinzu1());
  ASSERT_EQ(DorahyoToDora(Pai::Sozu1()), Pai::Sozu2());
  ASSERT_EQ(DorahyoToDora(Pai::Sozu3()), Pai::Sozu4());
  ASSERT_EQ(DorahyoToDora(Pai::Ton()), Pai::Nan());
  ASSERT_EQ(DorahyoToDora(Pai::Sya()), Pai::Pe());
  ASSERT_EQ(DorahyoToDora(Pai::Pe()), Pai::Ton());
  ASSERT_EQ(DorahyoToDora(Pai::Haku()), Pai::Hatsu());
  ASSERT_EQ(DorahyoToDora(Pai::Hatsu()), Pai::Chun());
  ASSERT_EQ(DorahyoToDora(Pai::Chun()), Pai::Haku());
}

#endif
