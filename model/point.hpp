#ifndef FEVERMJ_MODEL_POINT_HPP_
#define FEVERMJ_MODEL_POINT_HPP_
#include <cmath>
#include <cstdint>
#include "player_state.hpp"
#include "role.hpp"

namespace FeverMJ { namespace Model {
class Point {
 public:
  Point() = default;

  explicit Point(const RoleResult &result) 
      : han(result.hanCount), hu(result.huCount), dora(result.doraCount), roleList(result.roleBits) {}

  int GetHan() const {
    return han;
  }

  int GetHu() const {
    return hu;
  }

  int GetDoraCount() const {
    return dora;
  }

  std::uint64_t GetRole() const {
    return roleList;
  }

  void CheckBounus(const PlayerState &playerState) {
    if (playerState.IsReachFirstTumo()) {
      roleList |= Role::First;
      ++han;
    }
    if (dora) {
      roleList |= Role::Dora;
      if (roleList & Role::Benz) {
        dora *= 3;
      }
      han += dora;
    }
  }

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

  int GetBasicPoint() const {
    if (han >= 20) {
      return 8000 * (han / 10);
    } else if (han >= 13) {
      return 8000;
    } else if (han >= 11) {
      return 6000;
    } else if (han >= 8) {
      return 4000;
    } else if (han >= 6) {
      return 3000;
    } else if (han >= 5 || (han >=4 && hu >= 40) || (han >= 3 && hu >= 80)) {
      return 2000;
    } else {
      return hu * std::pow(2, han + 2);
    }
  }

 private:
  int GetRoundedUp100(int ten) const {
    return !(ten % 100) ? ten : (ten / 100 + 1) * 100;
  }

  int GetRoundedUp1000(int h) const {
    return h % 1000 > 500 ? (h / 1000 + 1) * 1000 : (h / 1000) * 1000;
  }

  int han = 0;
  int hu = 0;
  int dora = 0;
  std::uint64_t roleList = 0x0;
};
}}

#endif
