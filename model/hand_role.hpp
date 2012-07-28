#ifndef FEVERMJ_MODEL_HAND_ROLE_HPP_
#define FEVERMJ_MODEL_HAND_ROLE_HPP_
#include <cstdint>
#include <vector>
#include "pai.hpp"
#include "role.hpp"
#include "wait_type.hpp"
#include "wind.hpp"
#include "../utility/algtorithm.hpp"

namespace FeverMJ { namespace Model {
class HandRole {
 public:
  explicit HandRole(bool isTumoGoal,
                    Wind selfWind,
                    RoleType role,
                    WaitType waitType,
                    Pai h,
                    Pai waitHint,
                    Pai goalPai,
                    std::uint32_t triple,
                    const std::vector<Pai> &straightList) : roleType(role), head(h), tripleBits(triple) {
    CheckHu(waitType, selfWind);
    CheckTriple(isTumoGoal, waitType, goalPai);
    CheckStraightRoleState(waitType, waitHint, goalPai, straightList);
    if (roleType == RoleType::Normal && IsTyuntyanPai(head)) {
      isTyanta = false;
    }
  }

  std::uint32_t GetTripleBits() const {
    return tripleBits;
  }

  std::uint32_t GetStraightBits() const {
    return straightBits;
  }

  bool IsIpeko() const {
    return isIpeko;
  }

  bool IsRyanpeko() const {
    return isRyanpeko;
  }

  int GetDarkTripleCount() const {
    return darkTripleCount;
  }

  int GetHu() const {
    return huCount;
  }

  bool IsTyanta() const {
    return isTyanta;
  }

  bool HasStraight() const {
    return hasStraight;
  }

  Pai GetHead() const {
    return head;
  }

  RoleType GetRoleType() const {
    return roleType;
  }

 private:
  void CheckHu(WaitType waitType, Wind selfWind) {
    if (roleType == RoleType::Normal) {
      if (waitType != WaitType::BothSide && waitType != WaitType::DoubleHead) {
        huCount += 2;
      }
      if (IsRolePai(head)) {
        huCount += 2;
      } else {
        if (head == Pai::East) {
          huCount += 2;
        }
        if (head == Pai::East + selfWind) {
          huCount += 2;
        }
      }
    }
  }

  void GoalPaiAddTriple(bool isTumoGoal, WaitType waitType, Pai goalPai) {
    if (waitType == WaitType::DoubleHead) {
      if (isTumoGoal) {
        tripleBits |= (1 << goalPai);
      } else {
        if (IsTyuntyanPai(goalPai)) {
          isTyanta = false;
          huCount += 2;
        } else {
          huCount += 4;
        }
      }
    } 
  }

  void CheckTriple(bool isTumoGoal, WaitType waitType, Pai goalPai) {
    GoalPaiAddTriple(isTumoGoal, waitType, goalPai);
    for (int i = 0; i < paiKindMax; ++i) {
      if (tripleBits & (1 << i)) {
        if (IsTyuntyanPai(i)) {
          isTyanta = false;
          huCount += 4;
        } else {
          huCount += 8;
        }
        ++darkTripleCount;
      }
    }
  }

  std::vector<Pai> GoalPaiAddStraight(WaitType waitType, Pai waitHint, Pai goalPai, const std::vector<Pai> &straightList) {
    auto list = straightList;
    switch (waitType) {
      case WaitType::BothSide:
        list.push_back(waitHint == goalPai ? goalPai : static_cast<Pai>(goalPai - 2));
        break;
      case WaitType::SingleSide:
        list.push_back(GetNumber(goalPai) == 2 ? static_cast<Pai>(goalPai - 2) : goalPai);
        break;
      case WaitType::Between:
        list.push_back(static_cast<Pai>(goalPai - 1));
      default:
        break;
    }
    boost::sort(list);
    return list;
  }

  void CheckStraightRoleState(WaitType waitType, Pai waitHint, Pai goalPai, const std::vector<Pai> &straightList) {
    Pai beforePai = Pai::Invalid;
    const auto list = GoalPaiAddStraight(waitType, waitHint, goalPai, straightList);
    int straightKindCount = 0;
    int straightCount = 0;
    for (const auto pai : list) {
      const int number = GetNumber(pai);
      if (number > 0 && number < 6) {
        isTyanta = false;
      }
      if (!(number % 3)) {
        straightBits |= 1 << (3 * GetColor(pai) + number / 3);
      }
      if (pai != beforePai) {
        ++straightKindCount;
        beforePai = pai;
      } else {
        beforePai = Pai::Invalid;
      }
      ++straightCount;
    }
    if (straightCount - straightKindCount == 1) {
      isIpeko = true;
    } else if (straightCount - straightKindCount == 2) {
      isRyanpeko = true;
    }
    hasStraight = straightCount;
  }
  
  RoleType roleType;
  Pai head;
  std::uint32_t tripleBits;
  std::uint32_t straightBits = 0x0;
  bool isIpeko = false;
  bool isRyanpeko = false;
  int darkTripleCount = 0;
  int huCount = 0;
  bool isTyanta = true;
  bool hasStraight = false;
};
}}

#endif