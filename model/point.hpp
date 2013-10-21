#ifndef FEVERMJ_MODEL_POINT_HPP_
#define FEVERMJ_MODEL_POINT_HPP_
#include <cmath>
#include <cstdint>
#include "field.hpp"
#include "role_result.hpp"

namespace FeverMJ { namespace Model {
class Point {
 public:
  explicit Point(const RoleResult &result) : roleResult(result) {}

  int GetHan() const {
    return roleResult.GetHan();
  }

  int GetHu() const {
    return roleResult.GetHu();
  }

  int GetDoraCount() const {
    return roleResult.GetDoraCount();
  }

  std::uint64_t GetRole() const {
    return roleResult.GetRoleBits();
  }

  int GetTumoPoint(House goal, House pay, const Field &field) const {
    const bool isBreak = field.GetWareme() == pay || field.GetWareme() == goal;
    return field.GetParent() == goal || field.GetParent() == pay ?
           GetChildTumoParentPoint(isBreak) :
           GetChildTumoChildPoint(isBreak);
  }

  int GetRonPoint(House goal, House pay, const Field &field) const {
    const bool isBreak = field.GetWareme() == pay || field.GetWareme() == goal;
    return field.GetParent() == goal ? GetParentRonPoint(isBreak) : GetChildRonPoint(isBreak);
  }

  int GetBasicPoint() const {
    if (GetHan() >= 20) {
      return 8000 * (GetHan() / 10);
    } else if (GetHan() >= 13) {
      return 8000;
    } else if (GetHan() >= 11) {
      return 6000;
    } else if (GetHan() >= 8) {
      return 4000;
    } else if (GetHan() >= 6) {
      return 3000;
    } else if (GetHan() >= 5 || (GetHan() >=4 && GetHu() >= 30) || (GetHan() >= 3 && GetHu() >= 60)) {
      return 2000;
    } else {
      return GetHu() * std::pow(2, GetHan() + 2);
    }
  }

  void AddBounus(const PlayerState &playerState) {
    roleResult.AddBounus(playerState);
  }

 private:
  int GetChildRonPoint(bool isBreakHouse) const {
    return GetRoundedUp1000(GetRoundedUp100(GetBasicPoint() * 4) * (isBreakHouse ? 3 : 1));
  }

  int GetParentRonPoint(bool isBreakHouse) const {
    return GetRoundedUp1000(GetRoundedUp100(GetBasicPoint() * 6) * (isBreakHouse ? 3 : 1));
  }

  int GetChildTumoChildPoint(bool isBreakHouse) const {
    return GetRoundedUp1000(GetRoundedUp100(GetBasicPoint()) * (isBreakHouse ? 3 : 1));
  }

  int GetChildTumoParentPoint(bool isBreakHouse) const {
    return GetRoundedUp1000(GetRoundedUp100(GetBasicPoint() * 2) * (isBreakHouse ? 3 : 1));
  }

  static
  int GetRoundedUp100(int ten) {
    return !(ten % 100) ? ten : (ten / 100 + 1) * 100;
  }

  static
  int GetRoundedUp1000(int h) {
    return h % 1000 > 500 ? (h / 1000 + 1) * 1000 : (h / 1000) * 1000;
  }

  RoleResult roleResult;
};
}}

#endif
