#ifndef FEVERMJ_MODEL_HAND_HPP_
#define FEVERMJ_MODEL_HAND_HPP_
#include <cassert>
#include <array>
#include <vector>
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
    tumo = Pai::Invalid;
    hand = std::move(firstPai);
    boost::sort(hand);
    kind = {};
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
    const auto pai = tumo;
    --kind[pai];
    tumo = Pai::Invalid;
    return static_cast<Pai>(pai);
  }

  Pai HandCut(int i) {
    const auto pai = hand[i];
    --kind[pai];
    hand.erase(std::next(hand.begin(), i));
    if (tumo != Pai::Invalid) {
      hand.insert(boost::lower_bound(hand, tumo), tumo);
      tumo = Pai::Invalid;
    }
    return static_cast<Pai>(pai);
  }

  void EraseNorth() {
    assert(kind[Pai::North]);
    --kind[Pai::North];
    if (tumo != Pai::North) {
      hand.erase(boost::equal_range(hand, Pai::North).first);
      hand.insert(boost::lower_bound(hand, tumo), tumo);
    }
    tumo = Pai::Invalid;
  }

  void Ti(const TiPair &tiPair) {
    assert(tiPair.first != Pai::Invalid);
    assert(tiPair.second != Pai::Invalid);
    assert(tiPair.first < tiPair.second);
    --kind[tiPair.first];
    --kind[tiPair.second];
    hand.erase(boost::equal_range(hand, tiPair.first).first);
    hand.erase(boost::equal_range(hand, tiPair.second).first);
  }

  void Pon(Pai pai) {
    assert(pai != Pai::Invalid);
    assert(kind[pai] >= 2);
    kind[pai] -= 2;
    const auto pair = boost::equal_range(hand, pai);
    assert(pair.first != pair.second);
    hand.erase(pair.first, std::next(pair.first, 2));
  }

  void DarkKan(Pai pai, bool isOpen) {
    assert(pai != Pai::Invalid);
    assert(kind[pai] == 4);
    kind[pai] -= 4;
    const auto pair = isOpen ? boost::equal_range(hand, pai + squealOffset) : boost::equal_range(hand, pai);
    assert(pair.first != pair.second);
    hand.erase(pair.first, pair.second);
    if (pai != tumo) {
      hand.insert(boost::lower_bound(hand, tumo), tumo);
    }
    tumo = Pai::Invalid;
  }

  void AddKan(Pai pai) {
    assert(pai != Pai::Invalid);
    --kind[pai];
    if (pai != tumo) {
      hand.erase(boost::equal_range(hand, pai).first);
      hand.insert(boost::lower_bound(hand, tumo), tumo);
    }
    tumo = Pai::Invalid;
  }

  void LightKan(Pai pai) {
    assert(pai != Pai::Invalid);
    assert(kind[pai] == 3);
    kind[pai] -= 3;
    const auto pair = boost::equal_range(hand, pai);
    assert(pair.first != pair.second);
    hand.erase(pair.first, pair.second);
  }

  Pai GetTumo() const {
    return tumo;
  }

  Pai GetHand(int i) const {
    return static_cast<Pai>(hand[i]);
  }

  int GetHandSize() const {
    return hand.size();
  }

  int GetPaiCount(Pai pai) const {
    assert(pai != Pai::Invalid);
    return kind[pai];
  }

  bool IsDarkKanEnable() const {
    for (const int size : kind) {
      if (size == 4) {
        return true;
      }
    }
    return false;
  }

  bool IsReachKanEnable() const {
    return kind[tumo] == 4 && boost::all_of(wait.tenpaiPatern, [this](const TenpaiPatern &x) {return x.IsReachKanEnable(tumo);});
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
    return Model::GetReachPatern(tumo, hand, kind);
  }

  void SetOpenReach() {
    for (auto &pai : hand) {
      pai += squealOffset;
    }
  }

  void SetFlow() {
    for (auto &pai : hand) {
      pai = Pai::Invalid;
    }
  }

  void SetTenpai() {
    for (auto &pai : hand) {
      pai += squealOffset;
    }
  }

  PaiKindArray GetPaiKindArray() const {
    return kind;
  }

 private:
  HandVector hand;
  PaiKindArray kind;
  Pai tumo;
  WaitPair wait;
};
}}

#endif
