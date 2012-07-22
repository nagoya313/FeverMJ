#ifndef FEVERMJ_MODEL_CHECK_ROLE_HPP_
#define FEVERMJ_MODEL_CHECK_ROLE_HPP_
#include <cassert>
#include "field.hpp"
#include "pai.hpp"
#include "player_state.hpp"
#include "point.hpp"
#include "role.hpp"
#include "role_hand.hpp"
#include "squeal.hpp"
#include "wind.hpp"
#include "../utility/log.hpp"

namespace FeverMJ { namespace Model {
inline
void CheckPeace(RoleResult &result) {
  if (result.huCount == 20) {
    result.roleBits |= Role::Peace;
    ++result.hanCount;
  }
}

inline
void CheckNPeko(int darkStraightKindCount, int darkStraightCount, RoleResult &result) {
  if (darkStraightCount - 1 == darkStraightKindCount) {
    result.roleBits |= Role::Ipeko;
    ++result.hanCount;
  } else if (darkStraightCount - 2 == darkStraightKindCount) {
    result.roleBits |= Role::Ryanpeko;
    result.hanCount += 3;
  }
}

inline
void CheckMenzenRole(int darkStraightKindCount, int darkStraightCount, RoleResult &result) {
  CheckPeace(result);
  CheckNPeko(darkStraightKindCount, darkStraightCount, result);
}

inline
void CheckThreeColorTriple(std::uint32_t threeColorTripleBits, RoleResult &result) {
  if (((threeColorTripleBits & RoleBits::OneTriple) == RoleBits::OneTriple) ||
      ((threeColorTripleBits & RoleBits::NineTriple) == RoleBits::NineTriple)) {
    result.roleBits |= Role::ThreeColorTriple;
    result.hanCount += 2;
  }
}

inline
int CheckWind(Wind selfWind, std::uint32_t tripleBits, RoleResult &result) {
  int windCount = 0;
  for (int i = 0; i < 4; ++i) {
    if (tripleBits & (1 << (Pai::East + i))) {
      if (i == 3) {
        result.roleBits |= Role::North;
        ++result.hanCount;
      } else {
        if (!i) {
          result.roleBits |= Role::FieldWind;
          ++result.hanCount;
        }
        if (i == selfWind) {
          result.roleBits |= (Role::SelfEastWind << i);
          ++result.hanCount;
        }
      }
      ++windCount;
    }
  }
  return windCount;
}

inline
int CheckThree(std::uint32_t tripleBits, RoleResult &result) {
  int threeCount = 0;
  for (int i = 0; i < 3; ++i) {
    if (tripleBits & (1 << (Pai::White + i))) {
      result.roleBits |= (Role::White << i);
      ++result.hanCount;
      ++threeCount;
    }
  }
  return threeCount;
}

inline
void CheckThrees(Pai headPai,int threeCount, RoleResult &result) {
  if (threeCount == 2) {
    if (headPai >= Pai::White) {
      result.roleBits |= Role::LitleThree;
      result.hanCount += 2;
    }
  } else if (threeCount == 3) {
    result.roleBits |= Role::BigThree;
    result.hanCount += 13;
  }
}

inline
void CheckWinds(Pai headPai, int windCount, RoleResult &result) {
  if (windCount == 3) {
    if (headPai >= Pai::East && headPai <= Pai::North) {
      result.roleBits |= Role::LitleWind;
      result.hanCount += 13;
    }
  } else if (windCount == 4) {
    result.roleBits |= Role::BigWind;
    result.hanCount += 13;
  }
}

inline
void CheckNotPeaceAndTanyaoRole(Wind selfWind,
                                Pai headPai,
                                std::uint32_t tripleBits,
                                RoleResult &result) {
  CheckThreeColorTriple(tripleBits, result);
  const int threeCount = CheckThree(tripleBits, result);
  const int windCount = CheckWind(selfWind, tripleBits, result);
  CheckThrees(headPai, threeCount, result);  
  CheckWinds(headPai, windCount, result);
}

inline
void CheckTyanta(std::uint32_t paiKindBits, const PlayerState &playerState, bool isTyanta, RoleResult &result) {
  if (isTyanta && !(result.roleBits & Role::OldHead) && !(result.roleBits & Role::PureOldHead)) {
    if (!(paiKindBits & ~RoleBits::NotWord)) {
      result.roleBits |= Role::PureTyanta;
      result.hanCount += playerState.IsMenzen() ? 3 : 2;
    } else {
      result.roleBits |= Role::Tyanta;
      result.hanCount += playerState.IsMenzen() ? 2 : 1;
    }
  }
}

inline
void CheckStraight(std::uint32_t paiKindBits,
                   const PlayerState &playerState,
                   std::uint32_t straightBits,
                   RoleResult &result) {
  if ((straightBits & 0x7) == 0x7 || (straightBits & (0x7 << 3)) == (0x7 << 3)) {
    result.roleBits |= Role::Straight;
    result.hanCount += playerState.IsMenzen() ? 2 : 1;
  }
}

inline
void CheckNotTanyaoRole(std::uint32_t paiKindBits,
                        const PlayerState &playerState,
                        std::uint32_t straightBits,
                        bool isTyanta,
                        RoleResult &result) {
  CheckTyanta(paiKindBits, playerState, isTyanta, result);
  CheckStraight(paiKindBits, playerState, straightBits, result);
}

inline
void CheckToitoi(bool hasStraight, RoleResult &result) {
  if (!hasStraight) {
    result.roleBits |= Role::Toitoi;
    result.hanCount += 2;
  }
}

inline
void CheckTriple(int darkTripleCount, RoleResult &result) {
  if (darkTripleCount == 3) {
    result.roleBits |= Role::ThreeTriple;
    result.hanCount += 2;
  } else if (darkTripleCount == 4) {
    result.roleBits |= Role::FourTriple;
    result.hanCount += 13;
  }
}

inline
void CheckQuadruple(int quadrupleCount, RoleResult &result) {
  if (quadrupleCount == 3) {
    result.roleBits |= Role::ThreeQuadruple;
    result.hanCount += 2;
  } else if (quadrupleCount == 4) {
    result.roleBits |= Role::FourQuadruple;
    result.hanCount += 13;
  }
}

inline
void CheckNotPeaceRole(bool hasStraight,
                       int darkTripleCount,
                       int quadrupleCount,
                       RoleResult &result) {
  CheckToitoi(hasStraight, result);
  CheckTriple(darkTripleCount, result);
  CheckQuadruple(quadrupleCount, result);
}

inline
void CheckRole(Wind selfWind,
               std::uint32_t paiKindBits,
               const PlayerState &playerState,
               const RoleHandState &handRole,
               const RoleSquealState &squealRole,
               RoleResult &result) {
  result.huCount += handRole.huCount;
  if (playerState.IsMenzen()) {
    FEVERMJ_LOG("‡ŽqŽí—Þ:%d ‡ŽqŒÂ”:%d\n", handRole.straightKindCount, handRole.straightCount);
    CheckMenzenRole(handRole.straightKindCount, handRole.straightCount, result);
  }
  if (!(result.roleBits & Role::Peace)) {
    CheckNotPeaceRole(handRole.straightCount || squealRole.hasStraight,
                      handRole.darkTripleCount + squealRole.darkTripleCount,
                      squealRole.quadrupleCount,
                      result);
    if (!(result.roleBits & Role::Tanyao)) {
      CheckNotPeaceAndTanyaoRole(selfWind,
                                 handRole.head,
                                 handRole.tripleBits | squealRole.tripleBits,
                                 result);
    }
  }
  if (!(result.roleBits & Role::Tanyao)) {
    CheckNotTanyaoRole(paiKindBits,
                       playerState,
                       handRole.straightBits | squealRole.straightBits,
                       handRole.isTyanta && squealRole.isTyanta,
                       result);
  }
}

inline
void CheckTanyaoBitsRole(std::uint32_t paiKindBits, RoleResult &result) {
  if (!(paiKindBits & ~RoleBits::Tanyao)) {
    result.roleBits |= Role::Tanyao;
    ++result.hanCount;
  } else if (!(paiKindBits & ~RoleBits::OldHead) && paiKindBits != RoleBits::OldHead) {
    result.roleBits |= Role::OldHead;
    result.hanCount += 2;
  } else if (!(paiKindBits & ~RoleBits::PureOldHead)) {
    result.roleBits |= Role::PureOldHead;
    result.hanCount += 13;
  }
}

inline
void CheckNineTreasure(std::uint32_t paiKindBits, const PaiKindArray &kind, RoleResult &result) {
  if ((paiKindBits == RoleBits::PinzuPure && kind[Pai::P1] >= 3 && kind[Pai::P9] >= 3) ||
      (paiKindBits == RoleBits::SozuPure && kind[Pai::S1] >= 3 && kind[Pai::S9] >= 3)) {
    result.roleBits |= Role::NineTreasure;
    result.hanCount += 13;
  }
}

inline
void CheckSingleColorRole(std::uint32_t paiKindBits,
                          int handSize,
                          const PaiKindArray &kind,
                          const PlayerState &playerState,
                          RoleResult &result) {
  if (!(paiKindBits & ~RoleBits::WordSingle)) {
    result.roleBits |= Role::WordColor;
    result.hanCount += 13;
  } else if (!(paiKindBits & ~RoleBits::GreenSingle)) {
    result.roleBits |= Role::GreenColor;
    result.hanCount += 13;
  } else if (!(paiKindBits & ~RoleBits::PinzuPure) || !(paiKindBits & ~RoleBits::SozuPure)) {
    result.roleBits |= Role::PureColor;
    result.hanCount += playerState.IsMenzen() ? 6 : 5;
    if (playerState.IsMenzen() && handSize == 13) {
      CheckNineTreasure(paiKindBits, kind, result);
    }
  } else if (!(paiKindBits & ~RoleBits::ManzuSingle) || !(paiKindBits & ~RoleBits::PinzuSingle) || !(paiKindBits & ~RoleBits::SozuSingle)) {
    result.roleBits |= Role::SingleColor;
    result.hanCount += playerState.IsMenzen() ? 2 : 1;
  }
}

inline
void CheckBitsRole(std::uint32_t paiKindBits,
                   int handSize,
                   const PaiKindArray &kind,
                   const PlayerState &playerState,
                   RoleResult &result) {
  CheckTanyaoBitsRole(paiKindBits, result);
  CheckSingleColorRole(paiKindBits, handSize, kind, playerState, result);
}

inline
bool CheckKokusiMuso(const RoleHandState &handRole, RoleResult &result) {
  if (handRole.isKokusiMuso) {
    result.roleBits |= Role::KokusiMuso;
    result.hanCount += 13;
    return true;
  }
  return false;
}

inline
bool CheckSevenDouble(const RoleHandState &handRole, RoleResult &result) {
  if (handRole.isSevenDouble) {
    result.roleBits |= Role::SevenDouble;
    result.hanCount += 2;
    return true;
  }
  return false;
}

inline
void CheckReachRole(const PlayerState &playerState, RoleResult &result) {
  if (playerState.IsReach()) {
    result.roleBits |= Role::Reach;
    ++result.hanCount;
  } else if (playerState.IsDoubleReach()) {
    result.roleBits |= Role::DoubleReach;
    result.hanCount += 2;
  }
  if (playerState.IsOpen()) {
    result.roleBits |= Role::Open;
    ++result.hanCount;
  } else if (playerState.IsFever()) {
    result.roleBits |= Role::Fever;
    ++result.hanCount;
  } else if (playerState.IsDoubleFever()) {
    result.roleBits |= Role::DoubleFever;
    result.hanCount += 6;
  }
}

inline
void CheckFirstRole(Wind selfWind, const PlayerState &playerState, RoleResult &result) {
  if (playerState.IsFirstTumo()) {
    if (playerState.IsTumoGoal()) {
      if (selfWind == Wind::East) {
        result.roleBits |= Role::HeavenGoal;
      } else {
        result.roleBits |= Role::GroundGoal;
      }
      result.hanCount += 13;
    } else {
      result.roleBits |= Role::PersonGoal;
      result.hanCount += 5;
    }
  }
}

inline
void PreCheckRole(Wind selfWind,
                  const PlayerState &playerState,
                  std::uint32_t paiKindBits,
                  int handSize,
                  const RoleSquealState &squealRole,
                  const Field &field,
                  RoleResult &result) {
  if (playerState.IsMenzen()) {
    if (playerState.IsTumoGoal()) {
      result.roleBits |= Role::Tumo;
      ++result.hanCount;
    }
    CheckFirstRole(selfWind, playerState, result);
    CheckReachRole(playerState, result);
  } else {
    if (handSize == 1 && !squealRole.darkTripleCount) {
      result.roleBits |= Role::SpinalCord;
      result.hanCount += 13;
    }
  }
  if (playerState.IsTyankan()) {
    result.roleBits |= Role::Tyankan;
    ++result.hanCount;
  }
  if (playerState.IsRinsyanKaiho()) {
    result.roleBits |= Role::RinsyanKaiho;
    ++result.hanCount;
  } else if (field.IsPaiEmpty()) {
    ++result.hanCount;
    if (playerState.IsTumoGoal()) {
      result.roleBits |= Role::HaiteiMoyue;
    } else {
      result.roleBits |= Role::HaiteiRaoyui;
    }
  }
  if (squealRole.isBenz) {
    result.roleBits |= Role::Benz;
    result.hanCount += 2;
  }
}
}}

#endif
