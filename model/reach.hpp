#ifndef FEVERMJ_MODEL_REACH_HPP_
#define FEVERMJ_MODEL_REACH_HPP_
#include <vector>
#include "river.hpp"
#include "squeal.hpp"
#include "tenpai_patern.hpp"
#include "../utility/algorithm.hpp"
#include "../utility/log.hpp"

namespace FeverMJ { namespace Model {
struct ReachIndex {
  std::uint32_t reach;
  std::uint32_t fever;
  std::uint32_t doubleFever;
};

enum class ReachState {
  Normal,
  ReachStart,
  OpenStart,
  FeverStart,
  DoubleFeverStart
};

using ReachPatern = std::vector<std::vector<TenpaiPatern>>;

inline
void SetReachEnableIndex(int i, ReachIndex &index) {
  index.reach |= 1 << i;
}

inline
std::uint32_t CheckFever(const std::vector<TenpaiPatern> &patern, const Squeal &squeal) {
  const bool sevenP = boost::all_of(patern, [&squeal](const TenpaiPatern &x) {
    return x.HasSevenPinzu() || squeal.HasSevenPinzu();});
  const bool sevenS = boost::all_of(patern, [&squeal](const TenpaiPatern &x) {
    return x.HasSevenSozu() || squeal.HasSevenSozu();});
  return static_cast<std::uint32_t>(sevenP) | (static_cast<std::uint32_t>(sevenS) << 1);
}

inline
void SetFeverEnableIndex(int i, ReachIndex &index) {
  index.fever |= 1 << i;
}

inline
void SetDoubleFeverEnableIndex(int i, ReachIndex &index) {
  index.fever |= 1 << i;
  index.doubleFever |= 1 << i;
}

inline
ReachIndex GetReachEnableIndex(const River &river, const Hand &hand, const Squeal &squeal) {
  ReachIndex index = {};
  int i = 0;
  for (const auto &patern : hand.GetReachPatern()) {
    if (!patern.tenpaiPatern.empty()) {
      const int ii = i == hand.GetHandSize() ? 14 : i;
      SetReachEnableIndex(ii, index);
      const auto pai = i == hand.GetHandSize() ? *hand.GetTumo() : hand.GetHand(i);
      const auto wait = patern.waitPaiBits;
      if (!(wait & (1 << pai)) && !river.IsFuriten(wait, hand)) {
        const auto f = CheckFever(patern.tenpaiPatern, squeal);
        if (f == 0x1 || f == 0x2) {
          SetFeverEnableIndex(ii, index);
        } else if (f == 0x3) {
          SetDoubleFeverEnableIndex(ii, index);
        }
      }
    }
    ++i;
  }
  FEVERMJ_LOG("\n%x\n", index.reach);
  return index;
}
}}

#endif
