#ifndef FEVERMJ_VIEW_REACH_HPP_
#define FEVERMJ_VIEW_REACH_HPP_
#include <DxLib.h>
#include <boost/noncopyable.hpp>
#include "../model/field.hpp"
#include "../model/house.hpp"
#include "../utility/pai_image.hpp"

namespace FeverMJ { namespace View {
class Reach : boost::noncopyable {
 public:
  void Draw(const Model::Players &players, const Utility::PaiImage &paiImage) {
    if (players[Model::House::Up].GetPlayerState().IsReach()) {
      DrawGraph(296, 232, paiImage.GetReachBarHandle(0), TRUE);
    }
    if (players[Model::House::Self].GetPlayerState().IsReach()) {
      DrawGraph(378, 288, paiImage.GetReachBarHandle(1), TRUE);
    }
    if (players[Model::House::Down].GetPlayerState().IsReach()) {
      DrawGraph(528, 232, paiImage.GetReachBarHandle(0), TRUE);
    }
  }
};
}}

#endif
