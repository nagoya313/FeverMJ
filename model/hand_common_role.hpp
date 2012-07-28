#ifndef FEVERMJ_MODEL_HAND_COMMON_ROLE_HPP_
#define FEVERMJ_MODEL_HAND_COMMON_ROLE_HPP_
#include <cstdint>
#include <vector>
#include "field.hpp"
#include "pai.hpp"
#include "player_state.hpp"
#include "role.hpp"
#include "wait_type.hpp"
#include "wind.hpp"
#include "../utility/algtorithm.hpp"
#include "../utility/log.hpp"

namespace FeverMJ { namespace Model {
class HandCommonRole {
 public:
  explicit HandCommonRole(Wind wind, int isTumo, int size, const PaiKindArray &kind, const Field &field, const PlayerState playerState)
      : selfWind(wind),
        isTumoGoal(isTumo),
        handSize(size),
        doraCount(GetDoraCount(kind, field, playerState)),
        isHaitei(field.IsPaiEmpty()), 
        paiKindBits(GetPaiKindBits(kind)),
        state(playerState) {} 

  bool IsReach() const {
    return state.IsReach();
  }

  bool IsDoubleReach() const {
    return state.IsDoubleReach();
  }

  bool IsOpen() const {
    return state.IsOpen();
  }

  bool IsOpenRon() const {
    return state.IsOpenRon();
  }

  bool IsFever() const {
    return state.IsFever();
  }

  bool IsDoubleFever() const {
    return state.IsDoubleFever();
  }

  bool IsTyankan() const {
    return state.IsTyankan();
  }

  bool IsRinsyanKaiho() const {
    return state.IsRinsyanKaiho();
  }

  bool IsHaitei() const {
    return isHaitei;
  }

  bool IsMenzen() const {
    return state.IsMenzen();
  }

  bool IsFirstTumo() const {
    return state.IsFirstTumo();
  }

  bool IsParent() const {
    return selfWind == Wind::East;
  }

  bool IsTumoGoal() const {
    return isTumoGoal;
  }

  int GetDoraCount() const {
    return doraCount;
  }

  std::uint32_t GetPaiKindBits() const {
    return paiKindBits;
  }

  Wind GetSelfWind() const {
    return selfWind;
  }

  int GetHandSize() const {
    return handSize;
  }

 private:
  static std::uint32_t GetPaiKindBits(const PaiKindArray &kind) {
    std::uint32_t bits = 0x0;
    for (int i = 0; i < paiKindMax; ++i) {
      if (kind[i]) {
        bits |= 1 << i;
      }
    }
    FEVERMJ_LOG("Ží—Þ %x\n", bits);
    return bits;
  }

  static int GetDoraCount(const PaiKindArray &kind, const Field &field, const PlayerState &playerState) {
    return boost::accumulate(field.GetDoraList(playerState.IsReachTenpai()), 0, [&kind](int lhs, Pai dora) {
      return lhs += kind[dora];
    });
  }

  Wind selfWind;
  bool isTumoGoal;
  int handSize;
  int doraCount;
  bool isHaitei;
  std::uint32_t paiKindBits = 0x0;
  PlayerState state;
};
}}

#endif
