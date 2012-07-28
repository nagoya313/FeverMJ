#ifndef FEVERMJ_MODEL_ROLE_TEMP_HPP_
#define FEVERMJ_MODEL_ROLE_TEMP_HPP_
#include <cassert>
#include <cstdint>
#include "hand_common_role.hpp"
#include "hand_role.hpp"
#include "pai.hpp"
#include "player_state.hpp"
#include "squeal_role.hpp"
#include "role.hpp"
#include "wind.hpp"

namespace FeverMJ { namespace Model {
class RoleTemp {
 public:
  explicit RoleTemp(const HandCommonRole &common, const HandRole &hand, const SquealRole &squeal)
      : isIpeko(hand.IsIpeko()),
        isRyanpeko(hand.IsRyanpeko()),
        tripleBits(hand.GetTripleBits() | squeal.GetTripleBits()),
        selfWind(common.GetSelfWind()),
        head(hand.GetHead()),
        isTyanta(hand.IsTyanta() && squeal.IsTyanta()),
        isMenzen(common.IsMenzen()),
        paiKindBits(common.GetPaiKindBits() | squeal.GetPaiKindBits()),
        straightBits(hand.GetStraightBits() | squeal.GetStraightBits()),
        hasStraight(hand.HasStraight() || squeal.HasStraight()),
        darkTripleCount(hand.GetDarkTripleCount() | squeal.GetDarkTripleCount()),
        quadrupleCount(squeal.GetQuadrupleCount()),
        huCount(hand.GetHu() + squeal.GetHu()) {}

  bool IsIpeko() const {
    return isIpeko;
  }

  bool IsRyanpeko() const {
    return isRyanpeko;
  }

  std::uint32_t GetTripleBits() const {
    return tripleBits;
  }

  Wind GetSelfWind() const {
    return selfWind;
  }

  Pai GetHead() const {
    return head;
  }

  bool IsTyanta() const {
    return isTyanta;
  }

  bool IsMenzen() const {
    return isMenzen;
  }

  std::uint32_t GetPaiKindBits() const {
    return paiKindBits;
  }

  std::uint32_t GetStraightBits() const {
    return straightBits;
  }

  bool HasStraight() const {
    return hasStraight;
  }

  int GetDarkTripleCount() const {
    return darkTripleCount;
  }

  int GetQuadrupleCount() const {
    return quadrupleCount;
  }

  int GetHu() const {
    return huCount;
  }

 private:
  bool isIpeko;
  bool isRyanpeko;
  std::uint32_t tripleBits;
  Wind selfWind;
  Pai head;
  bool isTyanta;
  bool isMenzen;
  std::uint32_t paiKindBits;
  std::uint32_t straightBits;
  bool hasStraight;
  int darkTripleCount;
  int quadrupleCount;
  int huCount;
};
}}

#endif
