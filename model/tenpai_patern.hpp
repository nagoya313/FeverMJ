#ifndef FEVERMJ_MODEL_TENPAI_PATERN_HPP_
#define FEVERMJ_MODEL_TENPAI_PATERN_HPP_
#include <cstdint>
#include <vector>
#include "hand_role.hpp"
#include "pai.hpp"
#include "role.hpp"
#include "wait_type.hpp"
#include "wind.hpp"

namespace FeverMJ { namespace Model {
class TenpaiPatern {
 public:
  void SetHead(Pai pai) {
    head = pai;
  }

  void SetSevenDouble(std::uint32_t waitPai) {
    roleType = RoleType::SevenDouble;
    waitPais = waitPai;
    waitType = WaitType::Single;
  }

  void SetKokusiMuso(std::uint32_t waitPai) {
    roleType = RoleType::KokusiMuso;
    waitPais = waitPai;
    waitType = waitPais == RoleBits::OldHead ? WaitType::Rising : WaitType::Single;
  }

  WaitType GetWaitType() const {
    return waitType;
  }

  void AddTriple(Pai pai) {
    assert(pai != Pai::Invalid);
    assert(pai <= Pai::Center);
    tripleBits |= (1 << pai);
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
  }

  void SetBetweenWait(Pai pai) {
    assert(pai != Pai::Invalid);
    assert(pai <= Pai::Center);
    waitType = WaitType::Between;
    waitPais |= (1 << pai);
  }

  void SetBothSideWait(Pai pai) {
    assert(pai != Pai::Invalid);
    assert(pai <= Pai::Center);
    waitType = WaitType::BothSide;
    waitPais |= (1 << pai) | (1 << (pai + 3));
    waitHint = pai;
  }

  void SetDoubleHeadWait(Pai pai) {
    assert(pai != Pai::Invalid);
    assert(pai <= Pai::Center);
    waitType = WaitType::DoubleHead;
    waitPais |= (1 << pai);
  }

  bool IsGoalEnable(Pai pai) const {
    return waitPais & (1 << pai);
  }

  HandRole GetHandRole(bool isTumoGoal, Pai goalPai, Wind selfWind) const {
    return HandRole{isTumoGoal, selfWind, roleType, waitType, head, waitHint, goalPai, tripleBits, straightList};
  }

  bool IsReachKanEnable(Pai pai) const {
    return tripleBits & (1 << pai);
  }

  bool HasSevenPinzu() const {
    return tripleBits & (1 << Pai::P7);
  }

  bool HasSevenSozu() const {
    return tripleBits & (1 << Pai::S7);
  }

 private:
  std::uint32_t tripleBits = 0x0;
  std::vector<Pai> straightList;
  Pai head = Pai::Invalid;
  Pai waitHint = Pai::Invalid;
  WaitType waitType = WaitType::NotTenpai;
  RoleType roleType = RoleType::Normal;
  std::uint32_t waitPais = 0x0;
  bool isSevenDouble = false;
  bool isKokusiMuso = false;
};
}}

#endif
