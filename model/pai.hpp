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

namespace RoleBits_ {
enum RoleBits : std::uint32_t {
  OneTriple   = (1 << Pai::M1) | (1 << Pai::P1) | (1 << Pai::S1),
  NineTriple  = (1 << Pai::M9) | (1 << Pai::P9) | (1 << Pai::S9),
  NotWord     = (1 << Pai::M1) | (1 << Pai::M9) | (1 << Pai::P1) | (1 << Pai::P2) | (1 << Pai::P3) |
                (1 << Pai::P4) | (1 << Pai::P5) | (1 << Pai::P6) | (1 << Pai::P7) | (1 << Pai::P8) |
                (1 << Pai::P9) | (1 << Pai::S1) | (1 << Pai::S2) | (1 << Pai::S3) | (1 << Pai::S4) |
                (1 << Pai::S5) | (1 << Pai::S6) | (1 << Pai::S7) | (1 << Pai::S8) | (1 << Pai::S9),
  Tanyao      = (1 << Pai::P2) | (1 << Pai::P3) | (1 << Pai::P4) | (1 << Pai::P5) | (1 << Pai::P6) |
                (1 << Pai::P7) | (1 << Pai::P8) | (1 << Pai::S2) | (1 << Pai::S3) | (1 << Pai::S4) |
                (1 << Pai::S5) | (1 << Pai::S6) | (1 << Pai::S7) | (1 << Pai::S8),
  ManzuPure   = (1 << Pai::M1) | (1 << Pai::M9),
  PinzuPure   = (1 << Pai::P1) | (1 << Pai::P2) | (1 << Pai::P3) | (1 << Pai::P4) | (1 << Pai::P5) |
                (1 << Pai::P6) | (1 << Pai::P7) | (1 << Pai::P8) | (1 << Pai::P9),
  SozuPure    = (1 << Pai::S1) | (1 << Pai::S2) | (1 << Pai::S3) | (1 << Pai::S4) | (1 << Pai::S5) |
                (1 << Pai::S6) | (1 << Pai::S7) | (1 << Pai::S8) | (1 << Pai::S9),
  GreenSingle = (1 << Pai::S2) | (1 << Pai::S3) | (1 << Pai::S4) | (1 << Pai::S6) | (1 << Pai::S8) |
                (1 << Pai::From),
  WordSingle  = (1 << Pai::East) | (1 << Pai::South) | (1 << Pai::West) | (1 << Pai::North) | (1 << Pai::White) |
                (1 << Pai::From) | (1 << Pai::Center),
  ManzuSingle = ManzuPure | WordSingle,
  PinzuSingle = PinzuPure | WordSingle,
  SozuSingle  = SozuPure | WordSingle,
  PureOldHead = (1 << Pai::M1) | (1 << Pai::M9) | (1 << Pai::P1) | (1 << Pai::P9) | (1 << Pai::S1) | 
                (1 << Pai::S9),
  OldHead     = PureOldHead | WordSingle,
};
}

using RoleBits_::RoleBits;

constexpr int paiKindMax = 27;
constexpr int squealOffset = 32;

using PaiKindArray = std::array<int, paiKindMax>;
using PaiKindBitset = std::uint32_t;
using HandVector = std::vector<int>;
using DoraVector = std::vector<Pai>;
using TiPair = std::pair<Pai, Pai>;

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
  return RoleBits::Tanyao & (1 << pai);
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
bool IsStraightEnablePai(int pai) {
  return pai >= Pai::P1 && pai <= Pai::S9;
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
      return Pai::S1;
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
