#ifndef FEVERMJ_VIEW_DOWN_HOUSE_HPP_
#define FEVERMJ_VIEW_DOWN_HOUSE_HPP_
#include <DxLib.h>
#include <boost/noncopyable.hpp>
#include "../model/field.hpp"
#include "../model/player.hpp"
#include "../utility/pai_image.hpp"

namespace FeverMJ { namespace View {
class DownHouse : boost::noncopyable {
 public:
  void Draw(const Model::Player &downHouse, const Utility::PaiImage &paiImage) {
    DrawTumo(downHouse, paiImage);
    DrawHand(downHouse, paiImage);
    DrawSqueal(downHouse, paiImage);
    DrawRiver(downHouse, paiImage);
  }

 private:
  void DrawTumo(const Model::Player &downHouse, const Utility::PaiImage &paiImage) {
    if (downHouse.GetTumo()) {
      const int size = downHouse.GetHandSize();
      DrawGraph(752, 456 - 22 * size - 14, paiImage.GetBackHandle(3), TRUE);
    }
  }

  void DrawHand(const Model::Player &downHouse, const Utility::PaiImage &paiImage) {
    const int size = downHouse.GetHandSize();
    for (int i = size - 1; i >= 0; --i) {
      const auto pai = downHouse.GetHandPai(i);
      const int handle = pai != Model::paiBack ?
                         pai >= Model::squealOffset ?
                         paiImage.GetLeftHandle(pai % Model::squealOffset) : paiImage.GetBackHandle(3) : paiImage.GetBackHandle(2);
      DrawGraph(752, 456 - (pai >= Model::squealOffset ? 32 : 22) * i, handle, TRUE);
    }
  }

  void DrawRiver(const Model::Player &downHouse, const Utility::PaiImage &paiImage) {
    const int size = downHouse.GetRiverSize();
    for (int i = 0; i < size / 6 + 1; ++i) {
      int y = 352;
      for (int j = 5; j >= 0; --j) {
        if (i * 6 + j < size) {
          y -= downHouse.GetRiverImageHandle(i * 6 + j) < Model::squealOffset ? 32 : 44;
        }
      }
      for (int j = 5; j >= 0; --j) {
        if (i * 6 + j < size) {
          const auto pai = downHouse.GetRiverImageHandle(i * 6 + j);
          const int imageHandle = pai < Model::squealOffset ? 
                                  paiImage.GetLeftHandle(pai) :
                                  paiImage.GetDownHandle(pai % Model::squealOffset);
          DrawGraph(560 + 44 * i, y, imageHandle, TRUE);
          y += pai < Model::squealOffset ? 32 : 44;
        }
      }
    }
  }

  void DrawSqueal(const Model::Player &downHouse, const Utility::PaiImage &paiImage) {
    int pos = 0;
    const int size = downHouse.GetSquealSize();
    for (int i = 0; i < size; ++i) {
      const int pai = downHouse.GetSquealImageHandle(i);
      const int image = pai < 0 ?
                        paiImage.GetBackHandle(2) : pai < Model::squealOffset ?
                        paiImage.GetLeftHandle(pai) : paiImage.GetUpHandle(pai % Model::squealOffset);
      if (pai >= Model::squealOffset * 2) {
        DrawGraph(734, pos, image, TRUE);
      }
      DrawGraph(pai < Model::squealOffset ? 755 : 767, pos, image, TRUE);
      pos += pai < Model::squealOffset ? 32 : 44;
    }
  }
};
}}

#endif
