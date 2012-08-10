#ifndef FEVERMJ_VIEW_UP_HOUSE_HPP_
#define FEVERMJ_VIEW_UP_HOUSE_HPP_
#include <DxLib.h>
#include <boost/noncopyable.hpp>
#include "../model/field.hpp"
#include "../model/player.hpp"
#include "../utility/pai_image.hpp"

namespace FeverMJ { namespace View {
class UpHouse : boost::noncopyable {
 public:
  void Draw(const Model::Player &upHouse, const Utility::PaiImage &paiImage) {
    DrawHand(upHouse, paiImage);
    DrawTumo(upHouse, paiImage);
    DrawSqueal(upHouse, paiImage);
    DrawRiver(upHouse, paiImage);
  }

 private:
  void DrawTumo(const Model::Player &upHouse, const Utility::PaiImage &paiImage) {
    if (upHouse.GetTumo()) {
      const int size = upHouse.GetHandSize();
      DrawGraph(24, 78 + 22 * size, paiImage.GetBackHandle(1), TRUE);
    }
  }

  void DrawHand(const Model::Player &upHouse, const Utility::PaiImage &paiImage) {
    const int size = upHouse.GetHandSize();
    for (int i = 0; i < size; ++i) {
      const auto pai = upHouse.GetHandPai(i);
      const int handle = pai != Model::paiBack ?
                         pai >= Model::squealOffset ?
                         paiImage.GetRightHandle(pai % Model::squealOffset) : paiImage.GetBackHandle(1) : paiImage.GetBackHandle(2);
      DrawGraph(24, 64 + (pai >= Model::squealOffset ? 32 : 22) * i, handle, TRUE);
    }
  }

  void DrawRiver(const Model::Player &upHouse, const Utility::PaiImage &paiImage) {
    const int size = upHouse.GetRiverSize();
    int y = 160;
    for (int i = 0; i < size; ++i) {
      const auto pai = upHouse.GetRiverImageHandle(i);
      const int imageHandle = pai < Model::squealOffset ?
                              paiImage.GetRightHandle(pai) :
                              paiImage.GetUpHandle(pai % Model::squealOffset);
      DrawGraph(224 - 44 * (i / 6), y, imageHandle, TRUE);
      y += pai < Model::squealOffset ? 32 : 44;
      if (i % 6 == 5) {
        y = 160;
      }
    }
  }

  void DrawSqueal(const Model::Player &upHouse, const Utility::PaiImage &paiImage) {
    int pos = 600;
    const int size = upHouse.GetSquealSize();
    for (int i = 0; i < size; ++i) {
      const int pai = upHouse.GetSquealImageHandle(i);
      pos -= pai < Model::squealOffset ? 32 : 44;
    }
    for (int i = size - 1; i >= 0; --i) {
      const int pai = upHouse.GetSquealImageHandle(i);
      const int image = pai < 0 ?
                        paiImage.GetBackHandle(2) : pai < Model::squealOffset ?
                        paiImage.GetRightHandle(pai) : paiImage.GetUpHandle(pai % Model::squealOffset);
      if (pai >= Model::squealOffset * 2) {
        DrawGraph(33, pos, image, TRUE);
      }
      DrawGraph(0, pos, image, TRUE);
      pos += pai < Model::squealOffset ? 32 : 44;
    }
  }
};
}}

#endif
