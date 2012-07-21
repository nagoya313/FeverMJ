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
  Fever            = 1LL << 3,
  DoubleFever      = 1LL << 4,
  First            = 1LL << 5,
  Tumo             = 1LL << 6,
  Tanyao           = 1LL << 7,
  Peace            = 1LL << 8,
  Ipeko            = 1LL << 9,
  FieldWind        = 1LL << 10,
  SelfEastWind     = 1LL << 11,
  SelfSouthWind    = 1LL << 12,
  SelfWestWind     = 1LL << 13,
  North            = 1LL << 14,
  White            = 1LL << 15,
  From             = 1LL << 16,
  Center           = 1LL << 17,
  RinsyanKaiho     = 1LL << 18,
  Tyankan          = 1LL << 19,
  HaiteiMoyue      = 1LL << 20,
  HaiteiRaoyui     = 1LL << 21,
  Straight         = 1LL << 22,
  Tyanta           = 1LL << 23,
  SevenDouble      = 1LL << 24,
  Toitoi           = 1LL << 25,
  ThreeTriple      = 1LL << 26,
  OldHead          = 1LL << 27,
  ThreeColorTriple = 1LL << 28,
  ThreeQuadruple   = 1LL << 29,
  LitleThree       = 1LL << 30,
  SingleColor      = 1LL << 31,
  PureTyanta       = 1LL << 32,
  Ryanpeko         = 1LL << 33,
  PureColor        = 1LL << 34,
  Benz             = 1LL << 35,
  SpinalCord       = 1LL << 36,
  KokusiMuso       = 1LL << 37,
  FourTriple       = 1LL << 38,
  BigThree         = 1LL << 39,
  WordColor        = 1LL << 40,
  LitleWind        = 1LL << 41,
  BigWind          = 1LL << 42,
  GreenColor       = 1LL << 43,
  PureOldHead      = 1LL << 44,
  FourQuadruple    = 1LL << 45,
  NineTreasure     = 1LL << 46,
  HeavenGoal       = 1LL << 47,
  GroundGoal       = 1LL << 48,
  PersonGoal       = 1LL << 49,
  LimitHandSink    = 1LL << 50,
  Dora             = 1LL << 51
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
