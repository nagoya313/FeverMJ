#ifndef FEVERMJ_MODEL_HAND_HPP_
#define FEVERMJ_MODEL_HAND_HPP_
#include <cassert>
#include <array>
#include <vector>
#include <boost/phoenix.hpp>
#include <boost/optional.hpp>
#include "check_role.hpp"
#include "check_tenpai.hpp"
#include "field.hpp"
#include "pai.hpp"
#include "point.hpp"
#include "squeal.hpp"
#include "ti.hpp"
#include "wind.hpp"
#include "../utility/algorithm.hpp"
#include "../utility/log.hpp"

namespace FeverMJ { namespace Model {
class Hand {
 public:
  void Init(HandVector &&firstPai) {
    tumo = boost::none;
    hand = std::move(firstPai);
    boost::sort(hand);
    boost::fill(kind, 0);
    for (const auto pai : hand) {
      ++kind[pai];
    }
    wait = {};
  }

  void Tumo(Pai pai) {
    tumo = pai;
    ++kind[pai];
  }

  Pai TumoCut() {
    const auto pai = *tumo;
    assert(kind[pai]);
    --kind[pai];
    tumo = boost::none;
    return pai;
  }

  Pai HandCut(int i) {
    const auto pai = hand[i];
    assert(kind[pai]);
    --kind[pai];
    hand.erase(std::next(hand.begin(), i));
    if (tumo) {
      hand.insert(boost::lower_bound(hand, *tumo), *tumo);
      tumo = boost::none;
    }
    return static_cast<Pai>(pai);
  }

  void EraseNorth() {
    assert(kind[Pai::North]);
    --kind[Pai::North];
    if (tumo != Pai::North) {
      hand.erase(boost::equal_range(hand, Pai::North).first);
      hand.insert(boost::lower_bound(hand, *tumo), *tumo);
    }
    tumo = boost::none;
  }

  void Ti(const TiPair &tiPair) {
    assert(tiPair.first < tiPair.second);
    assert(kind[tiPair.first]);
    assert(kind[tiPair.second]);
    --kind[tiPair.first];
    --kind[tiPair.second];
    hand.erase(boost::equal_range(hand, tiPair.first).first);
    hand.erase(boost::equal_range(hand, tiPair.second).first);
  }

  void Pon(Pai pai) {
    assert(kind[pai] >= 2);
    kind[pai] -= 2;
    const auto pair = boost::equal_range(hand, pai);
    assert(pair.first != pair.second);
    hand.erase(pair.first, std::next(pair.first, 2));
  }

  void DarkKan(Pai pai, bool isOpen) {
    assert(kind[pai] == 4);
    kind[pai] -= 4;
    const auto pair = boost::equal_range(hand, isOpen ? pai + squealOffset : pai);
    assert(pair.first != pair.second);
    hand.erase(pair.first, pair.second);
    if (pai != tumo) {
      hand.insert(boost::lower_bound(hand, *tumo), *tumo);
    }
    tumo = boost::none;
  }

  void AddKan(Pai pai) {
    assert(kind[pai]);
    --kind[pai];
    if (pai != tumo) {
      hand.erase(boost::equal_range(hand, pai).first);
      hand.insert(boost::lower_bound(hand, *tumo), *tumo);
    }
    tumo = boost::none;
  }

  void LightKan(Pai pai) {
    assert(kind[pai] == 3);
    kind[pai] -= 3;
    const auto pair = boost::equal_range(hand, pai);
    assert(pair.first != pair.second);
    hand.erase(pair.first, pair.second);
  }

  boost::optional<Pai> GetTumo() const {
    return tumo;
  }

  Pai GetHand(int i) const {
    return static_cast<Pai>(hand[i]);
  }

  int GetHandSize() const {
    return hand.size();
  }

  int GetPaiCount(Pai pai) const {
    return kind[pai];
  }

  bool IsDarkKanEnable() const {
    using boost::phoenix::arg_names::arg1;
    return boost::any_of(kind, arg1 == 4);
  }

  bool IsReachKanEnable() const {
    return kind[*tumo] == 4 && boost::all_of(wait.tenpaiPatern, [this](const TenpaiPatern &x) {return x.IsReachKanEnable(*tumo);});
  }

  std::vector<TiPair> GetTiCandidate(Pai pai) const {
    return Model::GetTiCandidate(kind, pai);
  }

  void CheckTenpai(const PaiKindArray &squeal) {
    wait = Model::GetWaitPai(kind, squeal, hand.size() == 13);
  }

  bool IsTenpai() const {
    return wait.waitPaiBits;
  }

  bool IsWaitPai(Pai pai) const {
    return wait.waitPaiBits & (1 << pai);
  }

  WaitPair GetWait() const {
    return wait;
  }

  std::vector<WaitPair> GetReachPatern() const {
    return Model::GetReachPatern(*tumo, hand, kind);
  }

  void SetOpenReach() {
    using boost::phoenix::arg_names::arg1;
    boost::transform(hand, hand.begin(), arg1 + squealOffset);
  }

  void SetFlow() {
    boost::fill(hand, paiBack);
  }

  void SetTenpai() {
    using boost::phoenix::arg_names::arg1;
    boost::transform(hand, hand.begin(), boost::phoenix::if_else(arg1 < squealOffset, arg1 + squealOffset, arg1));
  }

  PaiKindArray GetPaiKindArray() const {
    return kind;
  }

 private:
  HandVector hand;
  PaiKindArray kind;
  boost::optional<Pai> tumo;
  WaitPair wait;
};
}}

#endif
