#ifndef FEVERMJ_MODEL_ROLE_HPP_
#define FEVERMJ_MODEL_ROLE_HPP_
#include <cstdint>
#include <vector>
#include "pai.hpp"
#include "wind.hpp"

namespace FeverMJ { namespace Model {
constexpr const char *roleString[] = {
  "リーチ",
  "ダブリー",
  "オープン",
  "フィーバー",
  "ダブルフィーバー",
  "一発",
  "門前清自摸和",
  "タンヤオ",
  "平和",
  "一盃口",
  "場風　東",
  "自風",
  "翻牌　北",
  "翻牌　白",
  "翻牌　発",
  "翻牌　中",
  "嶺上開花",
  "槍槓",
  "海底摸月",
  "海底撈魚",
  "一気通貫",
  "チャンタ",
  "七対子",
  "対々和",
  "三暗刻",
  "混老頭",
  "三色同刻",
  "三槓子",
  "小三元",
  "混一色",
  "純チャンタ",
  "二盃口",
  "清一色",
  "ベンツ",
  "脊髄",
  "国士無双",
  "四暗刻",
  "大三元",
  "字一色",
  "小四喜",
  "大四喜",
  "緑一色",
  "清老頭",
  "四槓子",
  "九蓮宝燈",
  "天和",
  "地和",
  "人和",
  "流し満貫",
  "ドラ"
};

namespace Role_ {
enum Role : std::uint64_t {
  Reach = 1LL << 0,
  DoubleReach = 1LL << 1,
  Open = 1LL << 2,
  Fever = 1LL << 3,
  DoubleFever = 1LL << 4,
  First = 1LL << 5,
  Tumo = 1LL << 6,
  Tanyao = 1LL << 7,
  Peace = 1LL << 8,
  Ipeko = 1LL << 9,
  FieldWind = 1LL << 10,
  SelfWind = 1LL << 11,
  North = 1LL << 12,
  White = 1LL << 13,
  From = 1LL << 14,
  Center = 1LL << 15,
  RinsyanKaiho = 1LL << 16,
  Tyankan = 1LL << 17,
  HaiteiMoyue = 1LL << 18,
  HaiteiRaoyui = 1LL << 19,
  Straight = 1LL << 20,
  Tyanta = 1LL << 21,
  SevenDouble = 1LL << 22,
  Toitoi = 1LL << 23,
  ThreeTriple = 1LL << 24,
  OldHead = 1LL << 25,
  ThreeColorTriple = 1LL << 26,
  ThreeQuadruple = 1LL << 27,
  LitleThree = 1LL << 28,
  SingleColor = 1LL << 29,
  PureTyanta = 1LL << 30,
  Ryanpeko = 1LL << 31,
  PureColor = 1LL << 32,
  Benz = 1LL << 33,
  SpinalCord = 1LL << 34,
  KokusiMuso = 1LL << 35,
  FourTriple = 1LL << 36,
  BigThree = 1LL << 37,
  WordColor = 1LL << 38,
  LitleWind = 1LL << 39,
  BigWind = 1LL << 40,
  GreenColor = 1LL << 41,
  PureOldHead = 1LL << 42,
  FourQuadruple = 1LL << 43,
  NineTreasure = 1LL << 44,
  HeavenGoal = 1LL << 45,
  GroundGoal = 1LL << 46,
  PersonGoal = 1LL << 47,
  LimitHandSink = 1LL << 48,
  Dora = 1LL << 49
};
}

using Role_::Role;

namespace RoleBits_ {
enum RoleBits : std::uint32_t {
  OneTriple = (1 << Pai::M1) | (1 << Pai::P1) | (1 << Pai::S1),
  NineTriple = (1 << Pai::M9) | (1 << Pai::P9) | (1 << Pai::S9),
  NotWord = (1 << Pai::M1) | (1 << Pai::M9) | (1 << Pai::P1) | (1 << Pai::P2) | (1 << Pai::P3) |
            (1 << Pai::P4) | (1 << Pai::P5) | (1 << Pai::P6) | (1 << Pai::P7) | (1 << Pai::P8) |
            (1 << Pai::P9) | (1 << Pai::S1) | (1 << Pai::S2) | (1 << Pai::S3) | (1 << Pai::S4) |
            (1 << Pai::S5) | (1 << Pai::S6) | (1 << Pai::S7) | (1 << Pai::S8) | (1 << Pai::S9),
  Tanyao = (1 << Pai::P2) | (1 << Pai::P3) | (1 << Pai::P4) | (1 << Pai::P5) | (1 << Pai::P6) |
           (1 << Pai::P7) | (1 << Pai::P8) | (1 << Pai::S2) | (1 << Pai::S3) | (1 << Pai::S4) |
           (1 << Pai::S5) | (1 << Pai::S6) | (1 << Pai::S7) | (1 << Pai::S8),
  ManzuPure = (1 << Pai::M1) | (1 << Pai::M9),
  PinzuPure = (1 << Pai::P1) | (1 << Pai::P2) | (1 << Pai::P3) | (1 << Pai::P4) | (1 << Pai::P5) |
              (1 << Pai::P6) | (1 << Pai::P7) | (1 << Pai::P8) | (1 << Pai::P9),
  SozuPure = (1 << Pai::S1) | (1 << Pai::S2) | (1 << Pai::S3) | (1 << Pai::S4) | (1 << Pai::S5) |
             (1 << Pai::S6) | (1 << Pai::S7) | (1 << Pai::S8) | (1 << Pai::S9),
  GreenSingle = (1 << Pai::S2) | (1 << Pai::S3) | (1 << Pai::S4) | (1 << Pai::S6) | (1 << Pai::S8) |
              (1 << Pai::From),
  WordSingle = (1 << Pai::East) | (1 << Pai::South) | (1 << Pai::West) | (1 << Pai::North) | (1 << Pai::White) |
               (1 << Pai::From) | (1 << Pai::Center),
  ManzuSingle = ManzuPure | WordSingle,
  PinzuSingle = PinzuPure | WordSingle,
  SozuSingle = SozuPure | WordSingle,
  PureOldHead = (1 << Pai::M1) | (1 << Pai::M9) | (1 << Pai::P1) | (1 << Pai::P9) | (1 << Pai::S1) | 
                (1 << Pai::S9),
  OldHead = PureOldHead | WordSingle,
};
}

using RoleBits_::RoleBits;


struct RoleHandState {
  int huCount = 0;
  int darkTripleCount = 0;
  std::uint32_t straightBits = 0x0;
  std::uint32_t tripleBits = 0x0;
  bool isTyanta = true;
  int straightKindCount = 0x0;
  int straightCount = 0x0;
  Pai head = Pai::Invalid;
  bool isKokusiMuso = false;
  bool isSevenDouble = false;
};

struct RoleSquealState {
  int huCount = 0;
  int doraCount = 0;
  int darkTripleCount = 0;
  int quadrupleCount = 0;
  std::uint32_t paiKindBits = 0x0;
  std::uint32_t straightBits = 0x0;
  std::uint32_t tripleBits = 0x0;
  bool isTyanta = true;
  bool hasStraight = false;
  bool isBenz = false;
};

struct RoleResult {
  int huCount = 0;
  int hanCount = 0;
  int doraCount = 0;
  std::uint64_t roleBits = 0x0;
};

inline
int RoundedUpHu(int hu) {
  return (hu + 9) / 10 * 10;
}
}}

#endif
