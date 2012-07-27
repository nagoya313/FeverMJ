#ifndef FEVERMJ_VIEW_GAME_HPP_
#define FEVERMJ_VIEW_GAME_HPP_
#include <array>
#include <boost/noncopyable.hpp>
#include "break_house.hpp"
#include "dora_mountain.hpp"
#include "first_parent.hpp"
#include "mat.hpp"
#include "players.hpp"
#include "reach.hpp"
#include "result.hpp"
#include "squeal.hpp"
#include "status.hpp"
#include "../controller/input.hpp"
#include "../model/field.hpp"
#include "../model/pai.hpp"
#include "../model/player.hpp"
#include "../model/point.hpp"
#include "../utility/pai_image.hpp"

namespace FeverMJ { namespace View {
class Game : boost::noncopyable {
 public:
  void Draw(const Controller::Input &input, const Model::Field &field, const Model::Players &players, const Utility::PaiImage &paiImage) {
    ClearDrawScreen();
    matView.Draw();
    doraMaountainView.Draw(field, paiImage);
    firstParentView.Draw(field, paiImage);
    breakHouseView.Draw(field, paiImage);
    reachView.Draw(players, paiImage);
    playersView.Draw(input, field, players, paiImage);
    squealView.Draw(input);
    statusView.Draw(field, players);
    resultView.Draw(field, players, input, paiImage);
    notSquealButton.Update(input);
    ScreenFlip();
  }

  template <typename Action>
  void SetMenuMode(Action action, MenuMode mode) {
    squealView.SetMode(action, mode);
  }

  template <typename Action>
  void SetSelectKanMode(Action action) {
    squealView.AllReset();
    playersView.SetSelectKanMode(action);
  }

  template <typename Action>
  void SetSelectTiMode(Action action, const std::vector<Model::TiPair> &tiList) {
    squealView.AllReset();
    playersView.SetSelectTiMode(action, tiList);
  }

  template <typename Action>
  void SetSelectReachMode(std::uint32_t reachIndex, Action action) {
    squealView.AllReset();
    playersView.SetSelectReachMode(reachIndex, action);
  }

  template <typename Action>
  void SetSelectMode(Action action) {
    playersView.SetSelectMode(action);
  }

  void SetWaitMode() {
    playersView.SetWaitMode();
    squealView.AllReset();
  }

  template <typename Action>
  void SetFlowSet(Action action, const std::array<int, 3> &variationPoints) {
    squealView.AllReset();
    statusView.SetResult(true);
    resultView.SetFlowSet(action);
    notSquealButton.SetHide(true);
  }

  template <typename Action>
  void SetSelectTumo(Action action, const Model::Point &point, Model::House goalHouse) {
    statusView.SetResult(true);
    resultView.SetSelectTumo(action, point, goalHouse);
    notSquealButton.SetHide(true);
  }

  template <typename Action>
  void SetSelectRon(Action action, const Model::Point &point, Model::House goalHouse) {
    statusView.SetResult(true);
    resultView.SetSelectRon(action, point, goalHouse);
    notSquealButton.SetHide(true);
  }

  void SetStart() {
    statusView.SetResult(false);
    resultView.SetStart();
    notSquealButton.SetHide(false);
  }

  bool NotSquealButtonIsToggle() const {
    return notSquealButton.IsToggle();
  }

 private:
  Mat matView;
  DoraMountain doraMaountainView;
  FirstParent firstParentView;
  BreakHouse breakHouseView;
  Status statusView;
  Result resultView;
  Players playersView;
  Reach reachView;
  Squeal squealView;
  View::ToggleButton notSquealButton{550, 440, "–Â‚©‚È‚¢"};
};
}}

#endif
