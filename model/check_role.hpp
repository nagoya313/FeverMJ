#ifndef FEVERMJ_MODEL_CHECK_ROLE_HPP_
#define FEVERMJ_MODEL_CHECK_ROLE_HPP_
#include <cassert>
#include "check_tenpai.hpp"
#include "field.hpp"
#include "hand_role.hpp"
#include "pai.hpp"
#include "player_state.hpp"
#include "point.hpp"
#include "role.hpp"
#include "squeal.hpp"
#include "role_temp.hpp"
#include "wind.hpp"
#include "../utility/log.hpp"

namespace FeverMJ { namespace Model {
inline
void CheckPeace(RoleResult &result) {
  if (result.GetHu() == 20) {
    result.AddRole(Role::Peace, 1);
  }
}

inline
void CheckNPeko(const RoleTemp &temp, RoleResult &result) {
  if (temp.IsIpeko()) {
    result.AddRole(Role::Ipeko, 1);
  } else if (temp.IsRyanpeko()) {
    result.AddRole(Role::Ryanpeko, 3);
  }
}

inline
void CheckMenzenRole(const RoleTemp &temp, RoleResult &result) {
  CheckPeace(result);
  CheckNPeko(temp, result);
}

inline
void CheckThreeColorTriple(const RoleTemp &temp, RoleResult &result) {
  if (((temp.GetTripleBits() & RoleBits::OneTriple) == RoleBits::OneTriple) ||
      ((temp.GetTripleBits() & RoleBits::NineTriple) == RoleBits::NineTriple)) {
    result.AddRole(Role::ThreeColorTriple, 2);
  }
}

inline
int CheckWind(const RoleTemp &temp, RoleResult &result) {
  int windCount = 0;
  for (int i = 0; i < 4; ++i) {
    if (temp.GetTripleBits() & (1 << (Pai::East + i))) {
      if (i == 3) {
        result.AddRole(Role::North, 1);
      } else {
        if (!i) {
          result.AddRole(Role::FieldWind, 1);
        }
        if (i == temp.GetSelfWind()) {
          result.AddRole(static_cast<Role>(Role::SelfEastWind << i), 1);
        }
      }
      ++windCount;
    }
  }
  return windCount;
}

inline
int CheckThree(const RoleTemp &temp, RoleResult &result) {
  int threeCount = 0;
  for (int i = 0; i < 3; ++i) {
    if (temp.GetTripleBits() & (1 << (Pai::White + i))) {
      result.AddRole(static_cast<Role>(Role::White << i), 1);
      ++threeCount;
    }
  }
  return threeCount;
}

inline
void CheckThrees(const RoleTemp &temp, int threeCount, RoleResult &result) {
  if (threeCount == 2) {
    if (temp.GetHead() >= Pai::White) {
      result.AddRole(Role::LitleThree, 2);
    }
  } else if (threeCount == 3) {
    result.AddRole(Role::BigThree, 13);
  }
}

inline
void CheckWinds(const RoleTemp &temp, int windCount, RoleResult &result) {
  if (windCount == 3) {
    if (temp.GetHead() >= Pai::East && temp.GetHead() <= Pai::North) {
      result.AddRole(Role::LitleWind, 13);
    }
  } else if (windCount == 4) {
    result.AddRole(Role::BigWind, 13);
  }
}

inline
void CheckNotPeaceAndTanyaoRole(const RoleTemp &temp, RoleResult &result) {
  CheckThreeColorTriple(temp, result);
  const int threeCount = CheckThree(temp, result);
  const int windCount = CheckWind(temp, result);
  CheckThrees(temp, threeCount, result);  
  CheckWinds(temp, windCount, result);
}

inline
void CheckTyanta(const RoleTemp &temp, RoleResult &result) {
  if (temp.IsTyanta() && !result.HasRole(Role::OldHead) && !result.HasRole(Role::PureOldHead)) {
    if (!(temp.GetPaiKindBits() & ~RoleBits::NotWord)) {
      result.AddRole(Role::PureTyanta, temp.IsMenzen() ? 3 : 2);
    } else {
      result.AddRole(Role::Tyanta, temp.IsMenzen() ? 2 : 1);
    }
  }
}

inline
void CheckStraight(const RoleTemp &temp, RoleResult &result) {
  if ((temp.GetStraightBits() & 0x7) == 0x7 || (temp.GetStraightBits() & (0x7 << 3)) == (0x7 << 3)) {
    result.AddRole(Role::Straight, temp.IsMenzen() ? 2 : 1);
  }
}

inline
void CheckNotTanyaoRole(const RoleTemp &temp, RoleResult &result) {
  CheckTyanta(temp, result);
  CheckStraight(temp, result);
}

inline
void CheckToitoi(const RoleTemp &temp, RoleResult &result) {
  if (!temp.HasStraight()) {
    result.AddRole(Role::Toitoi, 2);
  }
}

inline
void CheckTriple(const RoleTemp &temp, RoleResult &result) {
  if (temp.GetDarkTripleCount() == 3) {
    result.AddRole(Role::ThreeTriple, 2);
  } else if (temp.GetDarkTripleCount() == 4) {
    result.AddRole(Role::FourTriple, 13);
  }
}

inline
void CheckQuadruple(const RoleTemp &temp, RoleResult &result) {
  if (temp.GetQuadrupleCount() == 3) {
    result.AddRole(Role::ThreeQuadruple, 2);
  } else if (temp.GetQuadrupleCount() == 4) {
    result.AddRole(Role::FourQuadruple, 13);
  }
}

inline
void CheckNotPeaceRole(const RoleTemp &temp, RoleResult &result) {
  CheckToitoi(temp, result);
  CheckTriple(temp, result);
  CheckQuadruple(temp, result);
}

inline
void CheckRole(const RoleTemp &temp, RoleResult &result) {
  result.AddHu(temp.GetHu());
  if (temp.IsMenzen()) {
    CheckMenzenRole(temp, result);
  }
  if (!(result.HasRole(Role::Peace))) {
    CheckNotPeaceRole(temp, result);
    if (!(result.HasRole(Role::Tanyao))) {
      CheckNotPeaceAndTanyaoRole(temp, result);
    }
  }
  if (!(result.HasRole(Role::Tanyao))) {
    CheckNotTanyaoRole(temp, result);
  }
}

inline
void CheckTanyaoBitsRole(PaiKindBitset paiKindBits, RoleResult &result) {
  if (!(paiKindBits & ~RoleBits::Tanyao)) {
    result.AddRole(Role::Tanyao, 1);
  } else if (!(paiKindBits & ~RoleBits::OldHead) && paiKindBits != RoleBits::OldHead) {
    result.AddRole(Role::OldHead, 2);
  } else if (!(paiKindBits & ~RoleBits::PureOldHead)) {
    result.AddRole(Role::PureOldHead, 13);
  }
}

inline
void CheckNineTreasure(PaiKindBitset paiKindBits, const PaiKindArray &kind, RoleResult &result) {
  if ((paiKindBits == RoleBits::PinzuPure && kind[Pai::P1] >= 3 && kind[Pai::P9] >= 3) ||
      (paiKindBits == RoleBits::SozuPure && kind[Pai::S1] >= 3 && kind[Pai::S9] >= 3)) {
    result.AddRole(Role::NineTreasure, 13);
  }
}

inline
void CheckSingleColorRole(const HandCommonRole &hand, const PaiKindArray &kind, const SquealRole &squeal, RoleResult &result) {
  const auto paiKindBits = hand.GetPaiKindBits() | squeal.GetPaiKindBits();
  if (!(paiKindBits & ~RoleBits::WordSingle)) {
    result.AddRole(Role::WordColor, 13);
  } else if (!(paiKindBits & ~RoleBits::GreenSingle)) {
    result.AddRole(Role::GreenColor, 13);
  } else if (!(paiKindBits & ~RoleBits::PinzuPure) || !(paiKindBits & ~RoleBits::SozuPure)) {
    result.AddRole(Role::PureColor, hand.IsMenzen() ? 6 : 5);
    if (hand.IsMenzen() && hand.GetHandSize() == 13) {
      CheckNineTreasure(paiKindBits, kind, result);
    }
  } else if (!(paiKindBits & ~RoleBits::ManzuSingle) || !(paiKindBits & ~RoleBits::PinzuSingle) || !(paiKindBits & ~RoleBits::SozuSingle)) {
    result.AddRole(Role::SingleColor, hand.IsMenzen() ? 2 : 1);
  }
}

inline
void CheckBitsRole(const PaiKindArray &kind, const HandCommonRole &hand, const SquealRole &squeal, RoleResult &result) {
  CheckTanyaoBitsRole(hand.GetPaiKindBits() | squeal.GetPaiKindBits(), result);
  CheckSingleColorRole(hand, kind, squeal, result);
}

inline
bool CheckKokusiMuso(const HandRole &hand, RoleResult &result) {
  if (hand.GetRoleType() == RoleType::KokusiMuso) {
    result.AddRole(Role::KokusiMuso, 13);
    return true;
  }
  return false;
}

inline
bool CheckSevenDouble(const HandRole &hand, RoleResult &result) {
  if (hand.GetRoleType() == RoleType::SevenDouble) {
    result.AddRole(Role::SevenDouble, 2);
    return true;
  }
  return false;
}

inline
void CheckReachRole(const HandCommonRole hand, RoleResult &result) {
  if (hand.IsReach()) {
    result.AddRole(Role::Reach, 1);
  } else if (hand.IsDoubleReach()) {
    result.AddRole(Role::DoubleReach, 2);
  }
  if (hand.IsOpen()) {
    result.AddRole(Role::Open, 1);
    if (hand.IsOpenRon()) {
      result.AddRole(Role::OpenRon, 11);
    }
  } else if (hand.IsFever()) {
    result.AddRole(Role::Fever, 1);
  } else if (hand.IsDoubleFever()) {
    result.AddRole(Role::DoubleFever, 6);
  }
}

inline
void CheckFirstRole(const HandCommonRole hand, RoleResult &result) {
  if (hand.IsFirstTumo()) {
    if (hand.IsTumoGoal()) {
      if (hand.IsParent()) {
        result.AddRole(Role::HeavenGoal, 13);
      } else {
        result.AddRole(Role::GroundGoal, 13);
      }
    } else {
      result.AddRole(Role::PersonGoal, 5);
    }
  }
}

inline
void PreCheckRole(const HandCommonRole hand, const SquealRole &squeal, RoleResult &result) {
  if (hand.IsMenzen()) {
    if (hand.IsTumoGoal()) {
      result.AddRole(Role::Tumo, 1);
    }
    CheckFirstRole(hand, result);
    CheckReachRole(hand, result);
  } else {
    if (hand.GetHandSize() == 1 && !squeal.GetDarkTripleCount()) {
      result.AddRole(Role::SpinalCord, 13);
    }
  }
  if (hand.IsTyankan()) {
    result.AddRole(Role::Tyankan, 1);
  }
  if (hand.IsRinsyanKaiho()) {
    result.AddRole(Role::RinsyanKaiho, 1);
  } else if (hand.IsHaitei()) {
    if (hand.IsTumoGoal()) {
      result.AddRole(Role::HaiteiMoyue, 1);
    } else {
      result.AddRole(Role::HaiteiRaoyui, 1);
    }
  }
  if (squeal.IsBenz()) {
    result.AddRole(Role::Benz, 2);
  }
}

inline
WaitPair GetTenpaiPatern(Pai pai, int size, PaiKindArray &kind) {
  --kind[pai];
  const auto pair = GetWaitPai(kind, {}, size == 13);
  //FEVERMJ_LOG("‘Ò‚¿ %x Œ` %d Žè”v %d\n", pair.waitPaiBits, pair.tenpaiPatern.size(), size);
  ++kind[pai];
  return pair;
}

inline
std::vector<WaitPair> GetReachPatern(Pai tumo, const HandVector &hand, PaiKindArray kind) {
  std::vector<WaitPair> patern;
  const int size = hand.size();
  for (int i = 0; i < size; ++i) {
    patern.push_back(GetTenpaiPatern(static_cast<Pai>(hand[i]), size, kind));
  }
  patern.push_back(GetTenpaiPatern(tumo, size, kind));
  return patern;
}
}}

#endif
