#ifndef FEVERMJ_MODEL_ROLE_HPP_
#define FEVERMJ_MODEL_ROLE_HPP_
#include <cstdint>
#include <vector>
#include "pai.hpp"
#include "wind.hpp"

namespace FeverMJ { namespace Model {
constexpr const char *roleString[] = {
  "���[�`",
  "�_�u���[",
  "�I�[�v��",
  "�J���h�a",
  "�t�B�[�o�[",
  "�_�u���t�B�[�o�[",
  "�ꔭ",
  "��O�����̘a",
  "�^�����I",
  "���a",
  "��u��",
  "�ꕗ�@��",
  "�����@��",
  "�����@��",
  "�����@��",
  "�|�v�@�k",
  "�|�v�@��",
  "�|�v�@�",
  "�|�v�@��",
  "���J��",
  "����",
  "�C��̌�",
  "�C�ꝝ��",
  "��C�ʊ�",
  "�`�����^",
  "���Ύq",
  "�΁X�a",
  "�O�Í�",
  "���V��",
  "�O�F����",
  "�O�Ȏq",
  "���O��",
  "����F",
  "���`�����^",
  "��u��",
  "����F",
  "�x���c",
  "�Ґ�",
  "���m���o",
  "�l�Í�",
  "��O��",
  "����F",
  "���l��",
  "��l��",
  "�Έ�F",
  "���V��",
  "�l�Ȏq",
  "��@��",
  "�V�a",
  "�n�a",
  "�l�a",
  "��������",
  "�h��"
};

namespace Role_ {
enum Role : std::uint64_t {
  Reach            = 1LL << 0,
  DoubleReach      = 1LL << 1,
  Open             = 1LL << 2,
  OpenRon          = 1LL << 3,
  Fever            = 1LL << 4,
  DoubleFever      = 1LL << 5,
  First            = 1LL << 6,
  Tumo             = 1LL << 7,
  Tanyao           = 1LL << 8,
  Peace            = 1LL << 9,
  Ipeko            = 1LL << 10,
  FieldWind        = 1LL << 11,
  SelfEastWind     = 1LL << 12,
  SelfSouthWind    = 1LL << 13,
  SelfWestWind     = 1LL << 14,
  North            = 1LL << 15,
  White            = 1LL << 16,
  From             = 1LL << 17,
  Center           = 1LL << 18,
  RinsyanKaiho     = 1LL << 19,
  Tyankan          = 1LL << 20,
  HaiteiMoyue      = 1LL << 21,
  HaiteiRaoyui     = 1LL << 22,
  Straight         = 1LL << 23,
  Tyanta           = 1LL << 24,
  SevenDouble      = 1LL << 25,
  Toitoi           = 1LL << 26,
  ThreeTriple      = 1LL << 27,
  OldHead          = 1LL << 28,
  ThreeColorTriple = 1LL << 29,
  ThreeQuadruple   = 1LL << 30,
  LitleThree       = 1LL << 31,
  SingleColor      = 1LL << 32,
  PureTyanta       = 1LL << 33,
  Ryanpeko         = 1LL << 34,
  PureColor        = 1LL << 35,
  Benz             = 1LL << 36,
  SpinalCord       = 1LL << 37,
  KokusiMuso       = 1LL << 38,
  FourTriple       = 1LL << 39,
  BigThree         = 1LL << 40,
  WordColor        = 1LL << 41,
  LitleWind        = 1LL << 42,
  BigWind          = 1LL << 43,
  GreenColor       = 1LL << 44,
  PureOldHead      = 1LL << 45,
  FourQuadruple    = 1LL << 46,
  NineTreasure     = 1LL << 47,
  HeavenGoal       = 1LL << 48,
  GroundGoal       = 1LL << 49,
  PersonGoal       = 1LL << 50,
  LimitHandSink    = 1LL << 51,
  Dora             = 1LL << 52
};
}

using Role_::Role;

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
