#ifndef FEVERMJ_PLAYER_HPP_
#define FEVERMJ_PLAYER_HPP_
#include <cstdint>
#include <array>
#include <vector>
#include <boost/optional.hpp>
#include "check_goal.hpp"
#include "field.hpp"
#include "hand.hpp"
#include "house.hpp"
#include "pai.hpp"
#include "player_state.hpp"
#include "reach.hpp"
#include "squeal.hpp"
#include "wind.hpp"
#include "../utility/log.hpp"

namespace FeverMJ { namespace Model {
class Player {
 public:
  void GameStartInit(HandVector &&firstPai, Wind wind) {
    beforePoint = point = 25000;
    Init(std::move(firstPai), wind);
  }

  void ContinueSetInit(HandVector &&firstPai) {
    Init(std::move(firstPai), GetWind());
  }

  void NextSetInit(HandVector &&firstPai) {
    Init(std::move(firstPai), GetPrevWind(GetWind()));
  }

  void DeleteFirst() {
    playerState.DeleteFirst();
  }
  
  void SetFuritenPai(Pai pai) {
    assert(pai != Pai::Invalid);
    furitenList.push_back(pai);
  }

  boost::optional<Point> Ron(Pai pai, Field &field) {
    CheckHuriten();
    if (hand.IsWaitPai(pai) && !playerState.IsFuriten()) {
      FEVERMJ_LOG("ÉçÉìÇ¡ÇΩÅH\n");
      goalPai = pai;
      return IsRonGoal(pai, hand, squeal, field, playerState);
    }
    return boost::none;
  }

  boost::optional<Point> Tumo(Field &field) {
    return Tumo(field, false);
  }

  Pai TumoCut() {
    const auto pai = hand.TumoCut();
    DeleteFirst();
    riverList.push_back(pai);
    hand.CheckTenpai();
    return pai;
  }

  Pai HandCut(int i) {
    const Pai pai = hand.HandCut(i);
    DeleteFirst();
    riverList.push_back(pai);
    hand.CheckTenpai();
    notSelectableBits = 0x0;
    return pai;
  }

  boost::optional<Point> EraseNorth(Field &field) {
    hand.EraseNorth();
    squeal.AddNorth();
    DeleteFirst();
    return Tumo(field, true);
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

  boost::optional<Point> DarkKan(Pai pai, Field &field) {
    assert(pai != Pai::Invalid);
    hand.DarkKan(pai, playerState.IsOpen() || playerState.IsFever() || playerState.IsDoubleFever());
    squeal.AddDarkKan(pai);
    DeleteFirst();
    hand.CheckTenpai();
    playerState.SetRinsyanKaiho();
    return Tumo(field, true);
  }

  boost::optional<Point> AddKan(Pai pai, Field &field) {
    assert(pai != Pai::Invalid);
    hand.AddKan(pai);
    squeal.AddAddKan(pai);
    hand.CheckTenpai();
    playerState.SetRinsyanKaiho();
    return Tumo(field, true);
  }

  boost::optional<Point> LightKan(House house, Pai pai, Field &field) {
    assert(pai != Pai::Invalid);
    hand.LightKan(pai);
    squeal.AddLightKan(house, pai);
    DeleteFirst();
    hand.CheckTenpai();
    playerState.SetRinsyanKaiho();
    playerState.Squeal();
    return Tumo(field, true);
  }

  std::vector<TiPair> GetTiCandidate(Pai pai) const {
    assert(pai != Pai::Invalid);
    return hand.GetTiCandidate(pai);
  }

  bool IsParent() const {
    return GetWind() == Wind::East;
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
    return playerState.GetSelfWind();
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
    squealedList.push_back(static_cast<Pai>(*std::prev(riverList.end()) % squealOffset));
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

  bool IsReachKanEnable() const {
    return hand.IsReachKanEnable();
  }

  bool IsDarkOrAddKanEnable() const {
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
    beforePoint = point;
    point += p;
  }

  int GetBeforePoint() const {
    return beforePoint;
  }

  int GetPoint() const {
    return point;
  }

  int GetDiffPoint() const {
    return point - beforePoint;
  }

  boost::optional<Point> GetLimitHandSink(const Field &field) const {
    if (squealedList.empty()) {
      int doraCount = 0;
      const auto doraList = field.GetDoraList(false);
      for (const int pai : riverList) {
        if (IsTyuntyanPai(pai)) {
          return boost::none;
        }
        doraCount += boost::count(doraList, static_cast<Pai>(pai));
      }
      doraCount += squeal.GetNorthCount();
      return Point{RoleResult{doraCount}};
    }
    return boost::none;
  }

  bool IsTenpai() const {
    return hand.IsTenpai();
  }

  bool IsFever() const {
    return playerState.IsFever() || playerState.IsDoubleFever();
  }

  ReachIndex GetReachEnableIndex() const {
    return playerState.IsMenzen() ? Model::GetReachEnableIndex(riverList, hand, squeal) : ReachIndex{};
  }

  void SetReach() {
    if (playerState.IsFirstTumo()) {
      playerState.SetDoubleReach();
    } else {
      playerState.SetReach();
    }
    point -= 1000;
  }

  void SetOpenReach() {
    SetReach();
    playerState.SetOpen();
  }

  void SetFeverReach() {
    SetReach();
    playerState.SetFever();
  }

  void SetDoubleFeverReach() {
    SetReach();
    playerState.SetDoubleFever();
  }

  void SetReachRiver() {
    *std::prev(riverList.end()) += squealOffset;
    // Ç±Ç±Ç…èëÇ≠ÇÃÇÕâòÇ¢
    playerState.SetFirst();
    if (playerState.IsOpen() || playerState.IsFever() || playerState.IsDoubleFever()) {
      hand.SetOpenReach();
    }
  }

  Pai GetGoalPai() const {
    return goalPai;
  }

  void SetOpenRon() {
    playerState.SetOpenRon();
  }

  void ResetOpenRon() {
    playerState.ResetOpenRon();
  }

  void SetFlow(bool) {
    hand.SetFlow();
  }

 private:
  void Init(HandVector &&firstPai, Wind wind) {
    hand.Init(std::move(firstPai));
    squeal.Init();
    riverList.clear();
    furitenList.clear();
    squealedList.clear();
    playerState.Init(wind);
    notSelectableBits = 0x0;
    hand.CheckTenpai();
    AddPoint(0);
  }

  void CheckHuriten() {
    if (IsFever()) {
      playerState.ResetFuriten();
      return;
    }
    for (const int pai : riverList) {
      if (hand.IsWaitPai(static_cast<Pai>(pai % squealOffset))) {
        playerState.SetFuriten();
        return;
      }
    }
    for (const Pai pai : squealedList) {
      if (hand.IsWaitPai(pai)) {
        playerState.SetFuriten();
        return;
      }
    }
    for (const Pai pai : furitenList) {
      if (hand.IsWaitPai(pai)) {
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

  boost::optional<Point> Tumo(Field &field, bool isRinsyan) {
    const auto pai = isRinsyan ? field.GetRinsyan() : field.GetTumoPai();
    assert(pai != Pai::Invalid);
    if (!playerState.IsReachTenpai()) {
      furitenList.clear();
    }
    hand.Tumo(pai);
    FEVERMJ_LOG("ë“Çø:%x\n", hand.GetWait().waitPaiBits);
    if (hand.IsWaitPai(hand.GetTumo())) {
      goalPai = pai;
      FEVERMJ_LOG("ÉcÉÇÇ¡ÇΩ\n");
      return IsTumoGoal(hand, squeal, field, playerState);
    }
    return boost::none;
  }
  
  Hand hand;
  Squeal squeal;
  int beforePoint;
  int point;
  std::vector<int> riverList;
  std::vector<Pai> furitenList;
  std::vector<Pai> squealedList;
  PlayerState playerState;
  std::uint32_t notSelectableBits;
  Pai goalPai;
};
}}

#endif
