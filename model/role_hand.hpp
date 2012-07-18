#ifndef FEVERMJ_MODEL_ROLE_HAND_HPP_
#define FEVERMJ_MODEL_ROLE_HAND_HPP_
#include <cstdint>
#include <vector>
#include "pai.hpp"
#include "role.hpp"
#include "wind.hpp"
#include "../utility/algtorithm.hpp"

namespace FeverMJ { namespace Model {
namespace WaitType_ {
enum WaitType {
  NotTenpai = 0,
  Single = 1 << 0,
  BothSide = 1 << 1,
  Between = 1 << 2,
  SingleSide = 1 << 3,
  DoubleHead = 1 << 4
};
}

using WaitType_::WaitType;

class RoleHand {
 public:
  void SetHead(Pai pai) {
    head = pai;
  }

  void SetSevenDouble(std::uint32_t waitPai) {
    isSevenDouble = true;
    waitPais = waitPai;
  }

  void SetKokusiMuso(std::uint32_t waitPai) {
    isKokusiMuso = true;
    waitPais = waitPai;
  }

  std::uint32_t GetWaitPais() const {
    return waitPais;
  }

  void AddTriple(Pai pai) {
    assert(pai != Pai::Invalid);
    assert(pai <= Pai::Center);
    tripleList.push_back(pai);
  }

  void AddStraight(Pai pai) {
    assert(pai != Pai::Invalid);
    assert(pai <= Pai::Center);
    straightList.push_back(pai);
  }

  void SetSingleWait(Pai pai) {
    assert(head == pai);
    waitType = WaitType::Single;
    waitPais |= 1 << pai;
  }

  void SetSingleSideWait(Pai pai) {
    assert(pai != Pai::Invalid);
    assert(pai <= Pai::Center);
    waitType = WaitType::SingleSide;
    waitPais |= 1 << pai;
    straightList.push_back(GetNumber(pai) == 2 ? static_cast<Pai>(pai - 2) : pai);
  }

  void SetBetweenWait(Pai pai) {
    assert(pai != Pai::Invalid);
    assert(pai <= Pai::Center);
    waitType = WaitType::Between;
    waitPais |= (1 << pai);
    straightList.push_back(static_cast<Pai>(pai - 1));
  }

  void SetBothSideWait(Pai pai) {
    assert(pai != Pai::Invalid);
    assert(pai <= Pai::Center);
    waitType = WaitType::BothSide;
    waitPais |= (1 << pai) | (1 << (pai + 3));
    both = pai;
  }

  void SetDoubleHeadWait(Pai pai) {
    assert(pai != Pai::Invalid);
    assert(pai <= Pai::Center);
    waitType = WaitType::DoubleHead;
    waitPais |= (1 << pai);
    triple = pai;
  }

  RoleHandState GetRoleState(bool isTumoGoal, Pai goalPai, Wind selfWind) {
    RoleHandState role;
    CheckHu(role, selfWind);
    CheckTripleRoleState(isTumoGoal, role);
    CheckStraightRoleState(goalPai, role);
    role.head = head;
    role.isKokusiMuso = isKokusiMuso;
    role.isSevenDouble = isSevenDouble;
    if (role.isKokusiMuso && role.isSevenDouble) {
      if (IsTyuntyanPai(head)) {
        role.isTyanta = false;
      }
    }
    return role;
  }

 private:
  void CheckHu(RoleHandState &role, Wind selfWind) const {
    if (isKokusiMuso || isSevenDouble) {
      return;
    }
    if (waitType != WaitType::BothSide && waitType != WaitType::DoubleHead) {
      role.huCount += 2;
    }
    if (IsRolePai(head)) {
      role.huCount += 2;
    } else {
      if (head == Pai::East) {
        role.huCount += 2;
      }
      if (head == Pai::East + selfWind) {
        role.huCount += 2;
      }
    }
  }

  void CheckTripleRoleState(bool isTumoGoal, RoleHandState &role) const {
    int tripleCount = 0;
    for (const Pai pai : tripleList) {
      if (IsTyuntyanPai(pai)) {
        role.isTyanta = false;
        role.huCount += 4;
      } else {
        role.huCount += 8;
      }
      role.tripleBits |= 1 << pai;
      ++tripleCount;
    }
    if (triple != Pai::Invalid) {
      int hu = 2;
      if (isTumoGoal) {
        hu = 4;
        ++tripleCount;
      }
      if (IsTyuntyanPai(triple)) {
        role.isTyanta = false;
        role.huCount += hu;
      } else {
        role.huCount += hu * 2;
      }
      role.tripleBits |= 1 << triple;
    }
    role.darkTripleCount = tripleCount;
  }

  void CheckStraightRoleState(Pai goalPai, RoleHandState &role) {
    if (waitType == WaitType::BothSide) {
      straightList.push_back(both == goalPai ? goalPai : static_cast<Pai>(goalPai - 2));
    }
    boost::sort(straightList);
    Pai beforePai = Pai::Invalid;
    for (const Pai pai : straightList) {
      const int number = GetNumber(pai);
      if (number > 0 && number < 6) {
        role.isTyanta = false;
      }
      if (!(number % 3)) {
        role.straightBits |= 1 << (3 * GetColor(pai) + number / 3);
      }
      if (pai != beforePai) {
        ++role.straightKindCount;
        beforePai = pai;
      } else {
        beforePai = Pai::Invalid;
      }
      ++role.straightCount;
    }
  }

  Pai head = Pai::Invalid;
  std::vector<Pai> tripleList;
  std::vector<Pai> straightList;
  Pai triple = Pai::Invalid;
  Pai both = Pai::Invalid;
  WaitType waitType = WaitType::NotTenpai;
  std::uint32_t waitPais = 0x0;
  bool isSevenDouble = false;
  bool isKokusiMuso = false;
};
}}

#endif
