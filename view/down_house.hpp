#ifndef FEVERMJ_VIEW_DOWN_HOUSE_HPP_
#define FEVERMJ_VIEW_DOWN_HOUSE_HPP_
#include <DxLib.h>
#include <boost/noncopyable.hpp>
#include "../model/player.hpp"
#include "../utility/pai_image.hpp"

namespace FeverMJ { namespace View {
class DownHouse : boost::noncopyable {
 public:
  void Draw(const Model::Player &downHouse, const Utility::PaiImage &paiImage) {
    DrawTumo(downHouse, paiImage);
    DrawHand(downHouse, paiImage);
    DrawRiver(downHouse, paiImage);
  }

  void SetFlowSet() {
    isFlowSet = true;
  }

  void SetStart() {
    isFlowSet = false;
  }

 private:
  void DrawTumo(const Model::Player &downHouse, const Utility::PaiImage &paiImage) {
    const Model::Pai tumo = downHouse.GetTumo();
    if (tumo != Model::Pai::Invalid) {
      DrawGraph(752, 156, paiImage.GetBackHandle(1), TRUE);
    }
  }

  void DrawHand(const Model::Player &downHouse, const Utility::PaiImage &paiImage) {
    const int size = downHouse.GetHandSize();
    for (int i = 0; i < size; ++i) {
      if (isFlowSet) {
        DrawGraph(752, 192 + 22 * i,
                  downHouse.IsTenpai() ?
                  paiImage.GetLeftHandle(downHouse.GetHandPai(i)) :
                  paiImage.GetBackHandle(2), TRUE);
      } else {
        DrawGraph(752, 192 + 22 * i, paiImage.GetBackHandle(1), TRUE);
      }
    }
  }

  void DrawRiver(const Model::Player &downHouse, const Utility::PaiImage &paiImage) {
    const int size = downHouse.GetRiverSize();
    for (int i = 0; i < size / 6 + 1; ++i) {
      for (int j = 5; j >= 0; --j) {
        if (i * 6 + j < size) {
          const int imageHandle = paiImage.GetLeftHandle(downHouse.GetRiverImageHandle(i * 6 + j));
          DrawGraph(560 + 44 * i, 320 - 32 * j, imageHandle, TRUE);
        }
      }
    }
  }

  bool isFlowSet = false;
};
}}

#endif
