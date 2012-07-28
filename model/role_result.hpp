#ifndef FEVERMJ_MODEL_ROLE_RESULT_HPP_
#define FEVERMJ_MODEL_ROLE_RESULT_HPP_
#include <cassert>
#include <cstdint>
#include "field.hpp"
#include "hand_common_role.hpp"
#include "pai.hpp"
#include "player_state.hpp"
#include "squeal.hpp"
#include "squeal_role.hpp"
#include "role.hpp"

namespace FeverMJ { namespace Model {
class RoleResult {
 public:
  explicit RoleResult(const HandCommonRole &hand, const SquealRole &squeal) 
      : hu(20), doraCount(hand.GetDoraCount() + squeal.GetDoraCount()) {}

  explicit RoleResult(int doraCount) : han(4), hu(40), doraCount(doraCount), roleBits(Role::LimitHandSink) {}

  void AddRole(Role role, int h) {
    roleBits |= role;
    han += h;
  }
  
  void AddHu(int h) {
    hu += h;
  }

  void AddBounus(const PlayerState &playerState) {
    if (playerState.IsReachFirstTumo()) {
      roleBits |= Role::First;
      ++han;
    }
    if (doraCount) {
      roleBits |= Role::Dora;
      if (roleBits & Role::Benz) {
        doraCount *= 3;
      }
      han += doraCount;
    }
  }

  int GetHan() const {
    return han;
  }

  int GetHu() const {
    return hu;
  }

  int GetDoraCount() const {
    return doraCount;
  }

  std::uint64_t GetRoleBits() const {
    return roleBits;
  }

  bool HasRole(Role role) const {
    return roleBits & role;
  }

  void SetHu(int h) {
    hu = h;
  }

  void RoundedUpHu() {
    hu = (hu + 9) / 10 * 10;
  }

 private:
  int han = 0;
  int hu;
  int doraCount;
  std::uint64_t roleBits = 0x0;
};
}}

#endif
