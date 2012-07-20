#ifndef FEVERMJ_PLAYER_HPP_
#define FEVERMJ_PLAYER_HPP_
#include <cstdint>
#include <array>
#include <vector>
#include "field.hpp"
#include "hand.hpp"
#include "house.hpp"
#include "pai.hpp"
#include "player_state.hpp"
#include "squeal.hpp"
#include "wind.hpp"
#include "../utility/log.hpp"

namespace FeverMJ { namespace Model {
class Player {
 public:
  void GameStartInit(std::vector<Pai> &&firstPai, Wind wind) {
    point = 25000;
    Init(std::move(firstPai), wind);
  }

  void ContinueSetInit(std::vector<Pai> &&firstPai) {
    Init(std::move(firstPai), selfWind);
  }

  void NextSetInit(std::vector<Pai> &&firstPai) {
    Init(std::move(firstPai), GetPrevWind(selfWind));
  }

  void DeleteFirst() {
    playerState.DeleteFirst();
  }
  
  void SetFuritenPai(Pai pai) {
    assert(pai != Pai::Invalid);
    furitenList.push_back(pai);
  }

  Point Ron(Pai pai, Field &field) {
    if (hand.GetWaitPais() & (1 << pai) && !playerState.IsFuriten()) {
      FEVERMJ_LOG("ƒƒ“‚Á‚½H\n");
      return hand.IsRonGoal(pai, selfWind, squeal, field, playerState);
    }
    return {};
  }

  Point Tumo(Field &field) {
    return Tumo(field.GetTumoPai(), field);
  }

  Pai TumoCut() {
    const Pai pai = hand.TumoCut();
    DeleteFirst();
    riverList.push_back(pai);
    hand.CheckTenpai();
    CheckHuriten();
    return pai;
  }

  Pai HandCut(int i) {
    const Pai pai = hand.HandCut(i);
    DeleteFirst();
    riverList.push_back(pai);
    hand.CheckTenpai();
    CheckHuriten();
    notSelectableBits = 0x0;
    return pai;
  }

  Point EraseNorth(Field &field) {
    hand.EraseNorth();
    squeal.AddNorth();
    DeleteFirst();
    return RinsyanTumo(field);
  }

  void Ti(Pai squealPai, const std::pair<Pai, Pai> &pais) {
    assert(squealPai != Pai::Invalid);
    assert(pais.first != Pai::Invalid);
    assert(pais.second != Pai::Invalid);
    assert(pais.first < pais.second);
    hand.Ti(pais);
    squeal.AddTi(squealPai, pais);
    playerState.Squeal();
    DeleteFirst();
    notSelectableBits |= (1 << squealPai);
    if (pais.second - pais.first == 1) {
      const int number = GetNumber(squealPai);
      if (number < 6 && pais.first - squealPai == 1) {
        notSelectableBits |= 1 << (pais.second + 1);
      } else if (number > 2 && squealPai - pais.second == 1) {
        notSelectableBits |= 1 << (pais.first - 1);
      }
    }
  }

  void Pon(House house, Pai pai) {
    assert(pai != Pai::Invalid);
    hand.Pon(pai);
    squeal.AddPon(house, pai);
    playerState.Squeal();
    DeleteFirst();
    notSelectableBits |= 1 << pai;
  }

  Point DarkKan(Pai pai, Field &field) {
    assert(pai != Pai::Invalid);
    hand.DarkKan(pai);
    squeal.AddDarkKan(pai);
    DeleteFirst();
    hand.CheckTenpai();
    playerState.SetRinsyanKaiho();
    return RinsyanTumo(field);
  }

  Point AddKan(Pai pai, Field &field) {
    assert(pai != Pai::Invalid);
    hand.AddKan(pai);
    squeal.AddAddKan(pai);
    hand.CheckTenpai();
    playerState.SetRinsyanKaiho();
    return RinsyanTumo(field);
  }

  Point LightKan(House house, Pai pai, Field &field) {
    assert(pai != Pai::Invalid);
    hand.LightKan(pai);
    squeal.AddLightKan(house, pai);
    DeleteFirst();
    hand.CheckTenpai();
    playerState.SetRinsyanKaiho();
    playerState.Squeal();
    return RinsyanTumo(field);
  }

  std::vector<std::pair<Pai, Pai>> GetTiCandidate(Pai pai) const {
    assert(pai != Pai::Invalid);
    return hand.GetTiCandidate(pai);
  }

  bool IsParent() const {
    return selfWind == Wind::East;
  }

  int GetHandSize() const {
    return hand.GetHandSize();
  }

  int GetRiverSize() const {
    return riverList.size();
  }

  int GetSquealSize() const {
    return squeal.GetSquealSize();
  }

  Pai GetTumo() const {
    return hand.GetTumo();
  }

  Wind GetWind() const {
    return selfWind;
  }

  PlayerState GetPlayerState() const {
    return playerState;
  }

  Pai GetHandPai(int i) const {
    return hand.GetHand(i);
  }

  bool IsCutablePai(int i) const {
    return !(notSelectableBits & (1 << hand.GetHand(i)));
  }

  void PopBackRiver() {
    riverList.pop_back();
  }

  int GetRiverImageHandle(int i) const {
    return riverList[i];
  }

  int GetSquealImageHandle(int i) const {
    return squeal.GetSquealImageHandle(i);
  }

  bool IsDarkKanEnablePai(Pai pai) const {
    assert(pai != Pai::Invalid);
    return hand.GetPaiCount(pai) == 4;
  }

  bool IsAddKanEnablePai(Pai pai) const {
    assert(pai != Pai::Invalid);
    return squeal.IsAddKanEnablePai(pai);
  }

  bool IsDarkOrAddKanenable() const {
    return IsDarkKanEnable() || IsAddKanEnable();
  }

  bool IsPonEnable(Pai pai) const {
    assert(pai != Pai::Invalid);
    return hand.GetPaiCount(pai) >= 2;
  }

  bool IsLightKanEnable(Pai pai) const {
    assert(pai != Pai::Invalid);
    return hand.GetPaiCount(pai) == 3;
  }

  int GetPaiCount(Pai pai) const {
    assert(pai != Pai::Invalid);
    return hand.GetPaiCount(pai);
  }

  void AddPoint(int p) {
    point += p;
  }

  int GetPoint() const {
    return point;
  }

  bool IsLimitHandSink() const {
    if (squealedList.empty()) {
      for (const int pai : riverList) {
        if (IsTyuntyanPai(pai)) {
          return false;
        }
      }
      return true;
    }
    return false;
  }

  bool IsTenpai() const {
    return hand.GetWaitPais();
  }

 private:
  void Init(std::vector<Pai> &&firstPai, Wind wind) {
    hand.Init(std::move(firstPai));
    squeal.Init();
    riverList.clear();
    furitenList.clear();
    squealedList.clear();
    selfWind = wind;
    playerState.Init();
    notSelectableBits = 0x0;
    hand.CheckTenpai();
  }

  void CheckHuriten() {
    for (const int pai : riverList) {
      if (hand.GetWaitPais() & (1 << pai)) {
        playerState.SetFuriten();
        return;
      }
    }
    for (const Pai pai : squealedList) {
      if (hand.GetWaitPais() & (1 << pai)) {
        playerState.SetFuriten();
        return;
      }
    }
    for (const Pai pai : furitenList) {
      if (hand.GetWaitPais() & (1 << pai)) {
        playerState.SetFuriten();
        return;
      }
    }
    playerState.ResetFuriten();
  }

  bool IsDarkKanEnable() const {
    return hand.IsDarkKanEnable();
  }

  bool IsAddKanEnable() const {
    const int size = GetHandSize();
    for (int i = 0; i < size; ++i) {
      if (squeal.IsAddKanEnablePai(hand.GetHand(i))) {
        return true;
      }
    }
    if (squeal.IsAddKanEnablePai(hand.GetTumo())) {
      return true;
    }
    return false;
  }

  Point RinsyanTumo(Field &field) {
    return Tumo(field.GetRinsyan(), field);
  }

  Point Tumo(Pai pai, const Field &field) {
    assert(pai != Pai::Invalid);
    furitenList.clear();
    hand.Tumo(pai);
    FEVERMJ_LOG("‘Ò‚¿:%x\n", hand.GetWaitPais());
    if (hand.GetWaitPais() & (1 << hand.GetTumo())) {
      FEVERMJ_LOG("ƒcƒ‚‚Á‚½\n");
      return hand.IsTumoGoal(selfWind, squeal, field, playerState);
    }
    return {};
  }
  
  Hand hand;
  Squeal squeal;
  int point;
  std::vector<int> riverList;
  std::vector<Pai> furitenList;
  std::vector<Pai> squealedList;
  PlayerState playerState;
  Wind selfWind;
  std::uint32_t notSelectableBits;
};
}}

#endif
