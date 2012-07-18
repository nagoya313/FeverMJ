#ifndef FEVERMJ_VIEW_UP_HOUSE_HPP_
#define FEVERMJ_VIEW_UP_HOUSE_HPP_
#include <DxLib.h>
#include <boost/noncopyable.hpp>
#include "../model/player.hpp"
#include "../utility/pai_image.hpp"

namespace FeverMJ { namespace View {
class UpHouse : boost::noncopyable {
 public:
  void Draw(const Model::Player &upHouse, const Utility::PaiImage &paiImage) {
    DrawHand(upHouse, paiImage);
    DrawTumo(upHouse, paiImage);
    DrawRiver(upHouse, paiImage);
  }

  void SetFlowSet() {
    isFlowSet = true;
  }

  void SetStart() {
    isFlowSet = false;
  }

 private:
  void DrawTumo(const Model::Player &upHouse, const Utility::PaiImage &paiImage) {
    const int size = upHouse.GetHandSize();
    const Model::Pai tumo = upHouse.GetTumo();
    if (tumo != Model::Pai::Invalid) {
      DrawGraph(24, 78 + 22 * size, paiImage.GetBackHandle(1), TRUE);
    }
  }

  void DrawHand(const Model::Player &upHouse, const Utility::PaiImage &paiImage) {
    const int size = upHouse.GetHandSize();
    for (int i = 0; i < size; ++i) {
      if (isFlowSet) {
        DrawGraph(5, 64 + 22 * i, 
                  upHouse.IsTenpai() ? 
                  paiImage.GetRightHandle(upHouse.GetHandPai(i)) :
                  paiImage.GetBackHandle(2), TRUE);
      } else {
        DrawGraph(24, 64 + 22 * i, paiImage.GetBackHandle(1), TRUE);
      }
    }
  }

  void DrawRiver(const Model::Player &upHouse, const Utility::PaiImage &paiImage) {
    const int size = upHouse.GetRiverSize();
    for (int i = 0; i < size; ++i) {
      DrawGraph(224 - 44 * (i / 6), 160 + 32 * (i % 6),
                paiImage.GetRightHandle(upHouse.GetRiverImageHandle(i)), TRUE);
    }
  }

  bool isFlowSet = false;
};
}}

#endif
