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
#include "river.hpp"
#include "squeal.hpp"
#include "wind.hpp"
#include "../utility/log.hpp"

namespace FeverMJ { namespace Model {
class Player {
 public:
  void GameStartInit(HandVector &&firstPai, Wind wind, House house) {
    beforePoint = point = 25000;
    squeal.SetHouse(house);
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

  boost::optional<Point> Ron(std::uint32_t role, Pai pai, Field &field) {
    FEVERMJ_LOG("ë“Çø %x ÉtÉäÉeÉì %d\n", hand.GetWait().waitPaiBits, river.IsFuriten(hand));
    if (hand.IsWaitPai(pai) && !river.IsFuriten(hand)) {
      FEVERMJ_LOG("ÉçÉìîªíË\n");
      goalPai = pai;
      return IsRonGoal(role, pai, hand, squeal, field, playerState);
    }
    return boost::none;
  }

  boost::optional<Point> Tumo(Field &field) {
    return Tumo(field, false);
  }

  Pai TumoCut(bool isReach) {
    const auto pai = hand.TumoCut();
    if (isReach) {
      SetReach();
    }
    DeleteFirst();
    hand.CheckTenpai(squeal.GetPaiKindArray());
    return pai;
  }

  Pai HandCut(int i, bool isReach) {
    const Pai pai = hand.HandCut(i);
    if (isReach) {
      SetReach();
    }
    DeleteFirst();
    hand.CheckTenpai(squeal.GetPaiKindArray());
    notSelectableBits = 0x0;
    return pai;
  }

  void EraseNorth() {
    hand.EraseNorth();
    squeal.AddNorth();
    DeleteFirst();
  }

  boost::optional<Point> EraseNorthTumo(Field &field) {
    hand.CheckTenpai(squeal.GetPaiKindArray());
    return Tumo(field, true);
  }

  boost::optional<Point> RinsyanTumo(Field &field) {
    hand.CheckTenpai(squeal.GetPaiKindArray());
    playerState.SetRinsyanKaiho();
    return Tumo(field, true);
  }

  void RemoveNorth() {
    squeal.RemoveNorth();
  }

  void Ti(Pai squealPai, const std::pair<Pai, Pai> &pais) {
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
    hand.Pon(pai);
    squeal.AddPon(house, pai);
    playerState.Squeal();
    DeleteFirst();
    notSelectableBits |= 1 << pai;
  }

  boost::optional<Point> DarkKan(Pai pai, Field &field) {
    hand.DarkKan(pai, playerState.IsOpen() || playerState.IsFever() || playerState.IsDoubleFever());
    squeal.AddDarkKan(pai);
    DeleteFirst();
    hand.CheckTenpai(squeal.GetPaiKindArray());
    playerState.SetRinsyanKaiho();
    return Tumo(field, true);
  }

  void AddKan(Pai pai) {
    hand.AddKan(pai);
    squeal.AddAddKan(pai);
  }

  boost::optional<Point> LightKan(House house, Pai pai, Field &field) {
    hand.LightKan(pai);
    squeal.AddLightKan(house, pai);
    DeleteFirst();
    hand.CheckTenpai(squeal.GetPaiKindArray());
    playerState.SetRinsyanKaiho();
    playerState.Squeal();
    return Tumo(field, true);
  }

  std::vector<TiPair> GetTiCandidate(Pai pai) const {
    return hand.GetTiCandidate(pai);
  }

  bool IsParent() const {
    return GetWind() == Wind::East;
  }

  int GetHandSize() const {
    return hand.GetHandSize();
  }

  int GetRiverSize() const {
    return river.GetSize();
  }

  int GetSquealSize() const {
    return squeal.GetSquealSize();
  }

  boost::optional<Pai> GetTumo() const {
    return hand.GetTumo();
  }

  Wind GetWind() const {
    return playerState.GetSelfWind();
  }

  Pai GetHandPai(int i) const {
    return hand.GetHand(i);
  }

  bool IsCutablePai(int i) const {
    return !(notSelectableBits & (1 << hand.GetHand(i)));
  }

  bool IsReachTenpai() const {
    return playerState.IsReachTenpai();
  }

  bool IsOpen() const {
    return playerState.IsOpen();
  }

  int GetSquealImageHandle(int i) const {
    return squeal.GetSquealImageHandle(i);
  }

  bool IsDarkKanEnablePai(Pai pai) const {
    return hand.GetPaiCount(pai) == 4;
  }

  bool IsAddKanEnablePai(Pai pai) const {
    return squeal.IsAddKanEnablePai(pai);
  }

  bool IsReachKanEnable() const {
    return hand.IsReachKanEnable();
  }

  bool IsDarkOrAddKanEnable() const {
    return IsDarkKanEnable() || IsAddKanEnable();
  }

  bool IsPonEnable(Pai pai) const {
    return hand.GetPaiCount(pai) >= 2;
  }

  bool IsLightKanEnable(Pai pai) const {
    return hand.GetPaiCount(pai) == 3;
  }

  int GetPaiCount(Pai pai) const {
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

  bool IsTenpai() const {
    return hand.IsTenpai();
  }

  bool IsTypeTenpai(const Field &field) {
    if (playerState.IsMenzen() && IsTenpai()) {
      FEVERMJ_LOG("ñ ëOíÆîv\n");
      return true;
    } else if (IsTenpai()) {
      for (int i = 0; i < paiKindMax; ++i) {
        hand.Tumo(static_cast<Pai>(i));
        if (hand.IsWaitPai(static_cast<Pai>(i))) {
          if (IsTumoGoal(hand, squeal, field, playerState)) {
            hand.TumoCut();
            FEVERMJ_LOG("ñ¬Ç´íÆîv\n");
            return true;
          }
        }
        hand.TumoCut();
      }
    }
    FEVERMJ_LOG("íÆîvé∏îs\n");
    return false;
  }

  bool IsFever() const {
    return playerState.IsFever() || playerState.IsDoubleFever();
  }

  ReachIndex GetReachEnableIndex() const {
    return playerState.IsMenzen() ? Model::GetReachEnableIndex(river, hand, squeal) : ReachIndex{};
  }

  void SetReachFirst() {
    playerState.SetFirst();
  }

  void SetOpenReach() {
    playerState.SetOpen();
    playerState.SetFirst();
    hand.SetOpenReach();
  }

  void SetFeverReach() {
    playerState.SetFever();
    playerState.SetFirst();
    hand.SetOpenReach();
  }

  void SetDoubleFeverReach() {
    playerState.SetDoubleFever();
    playerState.SetFirst();
    hand.SetOpenReach();
  }

  void ReachCancel() {
    AddPoint(1000);
  }

  Pai GetGoalPai() const {
    return goalPai;
  }

  void SetFlow(bool f, const Field &field) {
    if (f || !IsTypeTenpai(field)) {
      hand.SetFlow();
    } else {
      FEVERMJ_LOG("íÆîvîªíË\n");
      hand.SetTenpai();
    }
  }

  boost::optional<Point> CheckLimitHandSink(const Field &field) const {
    return river.CheckLimitHandSink(field, squeal);
  }

  void SetFuritenPai(Pai pai) {
    river.SetFuritenPai(pai);
  }

  void PushBackRiver(Pai pai, bool isReach) {
    if (isReach) {
      river.ReachPush(pai);
    } else {
      river.Push(pai);
    }
  }

  void PopBackRiver() {
    river.Pop();
  }

  void SetRiver() {
    river.Set();
  }

  int GetRiverImageHandle(int i) const {
    return river[i];
  }

 private:
  void Init(HandVector &&firstPai, Wind wind) {
    hand.Init(std::move(firstPai));
    squeal.Init();
    river.Init();
    playerState.Init(wind);
    notSelectableBits = 0x0;
    hand.CheckTenpai({});
    AddPoint(0);
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
    if (squeal.IsAddKanEnablePai(*hand.GetTumo())) {
      return true;
    }
    return false;
  }

  boost::optional<Point> Tumo(Field &field, bool isRinsyan) {
    const auto pai = isRinsyan ? field.GetRinsyan() : field.GetTumoPai();
    if (!playerState.IsReachTenpai()) {
      river.ClearFuriten();
    }
    hand.Tumo(pai);
    FEVERMJ_LOG("ë“Çø:%x\n", hand.GetWait().waitPaiBits);
    if (hand.IsWaitPai(*hand.GetTumo())) {
      goalPai = pai;
      FEVERMJ_LOG("ÉcÉÇ\n");
      return IsTumoGoal(hand, squeal, field, playerState);
    }
    return boost::none;
  }

  void SetReach() {
    if (playerState.IsFirstTumo()) {
      playerState.SetDoubleReach();
    } else {
      playerState.SetReach();
    }
    AddPoint(-1000);
  }
  
  Hand hand;
  Squeal squeal;
  River river;
  int beforePoint;
  int point;
  PlayerState playerState;
  std::uint32_t notSelectableBits;
  Pai goalPai;
};
}}

#endif
