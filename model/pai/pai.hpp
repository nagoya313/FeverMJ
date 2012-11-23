#ifndef FEVERMJ_MODEL_PAI_PAI_HPP_
#define FEVERMJ_MODEL_PAI_PAI_HPP_
#include <cstddef>
#include "../../utility/operators/equality_comparable.hpp"
#include "../../utility/operators/less_than_comparable.hpp"

namespace FeverMJ { namespace Model { namespace Pai {
class Pai : Utility::Operators::EqualityComparable<Pai>,
            Utility::Operators::LessThanComparable<Pai> {
 public:
  static constexpr std::size_t max = 27;
  static constexpr Pai Manzu1() {return {0};}
  static constexpr Pai Manzu9() {return {1};}
  static constexpr Pai Pinzu1() {return {2};}
  static constexpr Pai Pinzu2() {return {3};}
  static constexpr Pai Pinzu3() {return {4};}
  static constexpr Pai Pinzu4() {return {5};}
  static constexpr Pai Pinzu5() {return {6};}
  static constexpr Pai Pinzu6() {return {7};}
  static constexpr Pai Pinzu7() {return {8};}
  static constexpr Pai Pinzu8() {return {9};}
  static constexpr Pai Pinzu9() {return {10};}
  static constexpr Pai Sozu1() {return {11};}
  static constexpr Pai Sozu2() {return {12};}
  static constexpr Pai Sozu3() {return {13};}
  static constexpr Pai Sozu4() {return {14};}
  static constexpr Pai Sozu5() {return {15};}
  static constexpr Pai Sozu6() {return {16};}
  static constexpr Pai Sozu7() {return {17};}
  static constexpr Pai Sozu8() {return {18};}
  static constexpr Pai Sozu9() {return {19};}
  static constexpr Pai Ton() {return {20};}
  static constexpr Pai Nan() {return {21};}
  static constexpr Pai Sya() {return {22};}
  static constexpr Pai Pe() {return {23};}
  static constexpr Pai Haku() {return {24};}
  static constexpr Pai Hatsu() {return {25};}
  static constexpr Pai Chun() {return {26};}
  constexpr int ID() const {return pai;}
 private:
  constexpr Pai(int p) : pai{p} {}
  friend constexpr
  bool operator ==(Pai lhs, Pai rhs) {return lhs.pai == rhs.pai;}
  friend constexpr
  bool operator <(Pai lhs, Pai rhs) {return lhs.pai < rhs.pai;}
  int pai;
};
}}}

#endif

