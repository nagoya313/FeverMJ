#ifndef FEVERMJ_VIEW_PLAYERS_HPP_
#define FEVERMJ_VIEW_PLAYERS_HPP_
#include <DxLib.h>
#include <boost/noncopyable.hpp>
#include "down_house.hpp"
#include "self.hpp"
#include "up_house.hpp"
#include "../controller/input.hpp"
#include "../model/pai.hpp"
#include "../model/player.hpp"
#include "../utility/pai_image.hpp"

namespace FeverMJ { namespace View {
class Players : boost::noncopyable {
 public:
  void Draw(const Controller::Input &input, const Model::Players &players, const Utility::PaiImage &paiImage) {
    upHouseView.Draw(players[Model::House::Up], paiImage);
    selfView.Draw(input, players[Model::House::Self], paiImage);
    downHouseView.Draw(players[Model::House::Down], paiImage);
  }

  template <typename Action>
  void SetSelectKanMode(Action action) {
    selfView.SetSelectKanMode(action);
  }

  template <typename Action>
  void SetSelectTiMode(Action action, const std::vector<Model::TiPair> &tiList) {
    selfView.SetSelectTiMode(action, tiList);
  }

  template <typename Action>
  void SetSelectReachMode(std::uint32_t reachIndex, Action action) {
    selfView.SetSelectReachMode(reachIndex, action);
  }

  template <typename Action>
  void SetSelectMode(Action action) {
    selfView.SetSelectMode(action);
  }

  void SetWaitMode() {
    selfView.SetWaitMode();
  }

 private:
  UpHouse upHouseView;
  Self selfView;
  DownHouse downHouseView;
};
}}

#endif
