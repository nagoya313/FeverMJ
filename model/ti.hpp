#ifndef FEVERMJ_MODEL_TI_HPP_
#define FEVERMJ_MODEL_TI_HPP_
#include <cassert>
#include <boost/optional.hpp>
#include "pai.hpp"
#include "../utility/algorithm.hpp"

namespace FeverMJ { namespace Model {
inline
boost::optional<TiPair> GetTiCandidateUpBothSideWait(const PaiKindArray &kind, Pai pai) {
  assert(IsStraightEnablePai(pai));
  if (GetNumber(pai) < 7 && kind[pai + 1] && kind[pai + 2]) {
    PaiKindArray temp = kind;
    temp[pai] = 0;
    --temp[pai + 1];
    --temp[pai + 2];
    if (GetNumber(pai) < 6) {
      temp[pai + 3] = 0;
    }
    if (boost::any_of(temp, [](int x) {return x;})) {
      return TiPair{static_cast<Pai>(pai + 1), static_cast<Pai>(pai + 2)};
    }
  }
  return boost::none;
}

inline
boost::optional<TiPair> GetTiCandidateDownBothSideWait(const PaiKindArray &kind, Pai pai) {
  assert(IsStraightEnablePai(pai));
  if (GetNumber(pai) > 1 && kind[pai - 1] && kind[pai - 2]) {
    PaiKindArray temp = kind;
    temp[pai] = 0;
    --temp[pai - 1];
    --temp[pai - 2];
    if (GetNumber(pai) > 2) {
      temp[pai - 3] = 0;
    }
    if (boost::any_of(temp, [](int x) {return x;})) {
      return TiPair{static_cast<Pai>(pai - 2), static_cast<Pai>(pai - 1)};
    }
  }
  return boost::none;
}

inline
boost::optional<TiPair> GetTiCandidateBetweenWait(const PaiKindArray &kind, Pai pai) {
  assert(IsStraightEnablePai(pai));
  const int number = GetNumber(pai);
  if (number != 0 && number != 8 && kind[pai - 1] && kind[pai + 1]) {
    PaiKindArray temp = kind;
    temp[pai] = 0;
    --temp[pai - 1];
    --temp[pai + 1];
    if (boost::any_of(temp, [](int x) {return x;})) {
      return TiPair{static_cast<Pai>(pai - 1), static_cast<Pai>(pai + 1)};
    }
  }
  return boost::none;
}

inline
std::vector<TiPair> GetTiCandidate(const PaiKindArray &kind, Pai pai) {
  std::vector<TiPair> tiCandidate;
  if (IsStraightEnablePai(pai)) {
    if (const auto pair = GetTiCandidateUpBothSideWait(kind, pai)) {
      tiCandidate.push_back(*pair);
    }
    if (const auto pair = GetTiCandidateDownBothSideWait(kind, pai)) {
      tiCandidate.push_back(*pair);
    }
    if (const auto pair = GetTiCandidateBetweenWait(kind, pai)) {
      tiCandidate.push_back(*pair);
    }
  }
  return tiCandidate;
}
}}

#endif
