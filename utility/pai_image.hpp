#ifndef FEVERMJ_VIEW_UTILITY_IMAGE_HPP_
#define FEVERMJ_VIEW_UTILITY_IMAGE_HPP_
#include <cassert>
#include <DxLib.h>
#include "../model/pai.hpp"

namespace FeverMJ { namespace Utility {
class PaiImage {
 public:
  PaiImage() {
    LoadDivGraph("image/tehai.png", Model::paiKindMax, 12, 3, 40, 64, handImage);
    LoadDivGraph("image/ue.png", Model::paiKindMax, 12, 3, 40, 64, upImage);
    LoadDivGraph("image/sita.png", Model::paiKindMax, 12, 3, 40, 64, downImage);
    LoadDivGraph("image/migi.png", Model::paiKindMax, 10, 3, 48, 56, rightImage);
    LoadDivGraph("image/hidari.png", Model::paiKindMax, 10, 3, 48, 56, leftImage);
    LoadDivGraph("image/se.png", 4, 2, 2, 64, 64, backImage);
    LoadDivGraph("image/titya.png", 4, 2, 2, 64, 64, firstParentImage);
    LoadDivGraph("image/ribo.png", 2, 2, 1, 128, 128, reachBarImage);
    breakHouseImage = LoadGraph("image/wareme.png");
  }

  int GetSelfHandHandle(int pai) const {
    return pai != Model::paiBack ?
           pai >= Model::squealOffset ?
           GetUpHandle(pai % Model::squealOffset) : GetHandHandle(pai) : GetBackHandle(0);
  }

  int GetSelfRiverHandle(int pai) const {
    return pai < Model::squealOffset ? GetUpHandle(pai) : GetRightHandle(pai % Model::squealOffset);
  }

  int GetSelfSquealHandle(int pai) const {
    return pai < 0 ?
           GetBackHandle(0) : pai < Model::squealOffset ?
           GetUpHandle(pai) : GetRightHandle(pai % Model::squealOffset);
  }

  int GetHandHandle(int pai) const {
    assert(pai < Model::paiKindMax);
    return handImage[pai];
  }

  int GetUpHandle(int pai) const {
    assert(pai < Model::paiKindMax);
    return upImage[pai];
  }

  int GetDownHandle(int pai) const {
    assert(pai < Model::paiKindMax);
    return downImage[pai];
  }

  int GetRightHandle(int pai) const {
    assert(pai < Model::paiKindMax);
    return rightImage[pai];
  }

  int GetLeftHandle(int pai) const {
    assert(pai < Model::paiKindMax);
    return leftImage[pai];
  }

  int GetBackHandle(int i) const {
    assert(i < 4);
    return backImage[i];
  }

  int GetFirstParentHandle(int i) const {
    assert(i < 4);
    return firstParentImage[i];
  }

  int GetReachBarHandle(int i) const {
    assert(i < 2);
    return reachBarImage[i];
  }

  int GetBreakHouseHandle() const {
    return breakHouseImage;
  }

 private:
  int handImage[Model::paiKindMax];
  int upImage[Model::paiKindMax];
  int downImage[Model::paiKindMax];
  int rightImage[Model::paiKindMax];
  int leftImage[Model::paiKindMax];
  int backImage[4];
  int firstParentImage[4];
  int reachBarImage[2];
  int breakHouseImage;
};
}}

#endif
