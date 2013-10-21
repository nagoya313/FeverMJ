#ifndef FEVERMJ_MODEL_HAND_ROLE_HPP_
#define FEVERMJ_MODEL_HAND_ROLE_HPP_
#include <cstdint>
#include <vector>
#include "pai.hpp"
#include "role.hpp"
#include "wait_type.hpp"
#include "wind.hpp"
#include "../utility/algorithm.hpp"
#include "../utility/log.hpp"

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
      tripleBits |= (1 << goalPai);
      int hu;
      if (isTumoGoal) {
        hu = 4;
        ++darkTripleCount;
      } else {
        hu = 2;
      }
      if (IsTyuntyanPai(goalPai)) {
        isTyanta = false;
        huCount += hu;
      } else {
        huCount += hu * 2;
      }
    } 
  }

  void CheckTriple(bool isTumoGoal, WaitType waitType, Pai goalPai) {
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
    GoalPaiAddTriple(isTumoGoal, waitType, goalPai);
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
    boost::optional<Pai> beforePai = boost::none;
	boost::optional<Pai> before2Pai = boost::none;
    const auto list = GoalPaiAddStraight(waitType, waitHint, goalPai, straightList);
    for (const auto pai : list) {
      const int number = GetNumber(pai);
      if (number > 0 && number < 6) {
        isTyanta = false;
      }
      if (!(number % 3)) {
        straightBits |= 1 << (3 * GetColor(pai) + number / 3);
      }
      if (beforePai && *beforePai == pai) {
        if (!isIpeko) {
		  isIpeko = true;
		} else {
		  if (!(before2Pai && *before2Pai == pai)) {
		    isIpeko = false;
			isRyanpeko = true;
		  }
		}
      }
	  before2Pai = beforePai;
	  beforePai = pai;
    }
    hasStraight = !list.empty();
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
