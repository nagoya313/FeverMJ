#ifndef FEVERMJ_MODEL_PAI_HPP_
#define FEVERMJ_MODEL_PAI_HPP_
#include <cassert>
#include <array>

namespace FeverMJ { namespace Model {
namespace Pai_ {
enum Pai {
  Invalid = -1,
  M1 = 0,
  M9 = 1,
  P1 = 2,
  P2 = 3,
  P3 = 4,
  P4 = 5,
  P5 = 6,
  P6 = 7,
  P7 = 8,
  P8 = 9,
  P9 = 10,
  S1 = 11,
  S2 = 12,
  S3 = 13,
  S4 = 14,
  S5 = 15,
  S6 = 16,
  S7 = 17,
  S8 = 18,
  S9 = 19,
  East = 20,
  South = 21,
  West = 22,
  North = 23,
  White = 24,
  From = 25,
  Center = 26
};
}

using Pai_::Pai;

namespace Color_ {
enum Color {
  Monzu = -1,
  Pinzu = 0,
  Sozu = 1,
  Word = 2
};
}

using Color_::Color;

constexpr int paiKindMax = 27;
constexpr int squealOffset = 32;

typedef std::array<int, paiKindMax> PaiKindArray;
typedef std::pair<Pai, Pai> TiPair;

inline
bool IsSameColor(int lhs, int rhs) {
  assert(lhs != Pai::Invalid);
  assert(rhs != Pai::Invalid);
  assert(lhs <= Pai::Center);
  assert(rhs <= Pai::Center);
  return (lhs - Pai::P1) / 9 == (rhs - Pai::P1) / 9;
}

inline
Color GetColor(int pai) {
  assert(pai != Pai::Invalid);
  assert(pai <= Pai::Center);
  return static_cast<Color>((pai - Pai::P1) / 9);
}

inline
int GetNumber(int pai) {
  assert(pai != Pai::Invalid);
  assert(pai <= Pai::Center);
  return (pai - Pai::P1) % 9;
}

inline
bool IsTyuntyanPai(int pai) {
  assert(pai != Pai::Invalid);
  assert(pai <= Pai::Center);
  return (pai > Pai::P1 &&
          pai < Pai::P9) ||
         (pai > Pai::S1 &&
          pai < Pai::S9);
}

inline
bool IsWordPai(int pai) {
  assert(pai != Pai::Invalid);
  assert(pai <= Pai::Center);
  return pai >= Pai::East;
}

inline
bool IsRolePai(int pai) {
  assert(pai != Pai::Invalid);
  assert(pai <= Pai::Center);
  return pai >= Pai::North;
}

inline
Pai GetDora(Pai displayDora) {
  assert(displayDora != Pai::Invalid);
  switch (displayDora) {
    case Pai::M1:
      return Pai::M9;
    case Pai::M9:
      return Pai::M1;
    case Pai::P9:
      return Pai::P1;
    case Pai::S9:
      return Pai::S9;
    case Pai::North:
      return Pai::East;
    case Pai::Center:
      return Pai::White;
    default:
      return static_cast<Pai>(displayDora + 1);
  }
}

#ifndef NDEBUG
const char * const pais[] = {
  "1m", "9m",
  "1p", "2p", "3p", "4p", "5p", "6p", "7p", "8p", "9p",
  "1s", "2s", "3s", "4s", "5s", "6s", "7s", "8s", "9s",
  "ìå", "ìÏ", "êº", "ñk", "îí", "î≠", "íÜ"
};
#endif
}}

#endif
