#ifndef FEVERMJ_VIEW_SELF_HPP_
#define FEVERMJ_VIEW_SELF_HPP_
#include <functional>
#include <DxLib.h>
#include <boost/noncopyable.hpp>
#include "../controller/input.hpp"
#include "../model/pai.hpp"
#include "../model/player.hpp"
#include "../utility/pai_image.hpp"

namespace FeverMJ { namespace View {
class Self : boost::noncopyable {
 public:
  void Draw(const Controller::Input &input,
            const Model::Field &field,
            const Model::Player &self,
            const Utility::PaiImage &paiImage) {
    if (isKan) {
      DrawKanHand(input, self, paiImage);
    } else if (isTi) {
      DrawTiHand(input, self, paiImage);
    } else {
      DrawHand(input, self, paiImage);
    }
    DrawSqueal(field, self, paiImage);
    DrawRiver(self, paiImage);
  }

  void SetFlowSet() {
    isFlowSet = true;
  }

  void SetStart() {
    isFlowSet = false;
  }

  template <typename Action>
  void SetSelectKanMode(Action action) {
    isKan = true;
    isTi = false;
    isSelect = false;
    kanAction = action;
  }

  template <typename Action>
  void SetSelectTiMode(Action action, const std::vector<Model::TiPair> &list) {
    isKan = false;
    isTi = true;
    isSelect = false;
    tiAction = action;
    tiList = list;
  }

  template <typename Action>
  void SetSelectReachMode(std::uint32_t index, Action action) {
    isKan = false;
    isTi = false;
    isSelect = true;
    reachIndex = index;
    discardAction = action;
  }

  template <typename Action>
  void SetSelectMode(Action action) {
    isKan = false;
    isTi = false;
    isSelect = true;
    discardAction = action;
  }

  void SetWaitMode() {
    isKan = false;
    isTi = false;
    isSelect = false;
    reachIndex = 0x0;
  }

 private:
  static int SelectedPaiIndex(const POINT &point, int size) {
    if (point.y > 512 && point.y < 576) {
      const int index =  (point.x - 80) / 33;
      if (index >= size) {
        return (point.x - 105) / 33 == size ? 14 : -1;
      }
      return index;
    }
    return -1;
  }

  static int SelectedPaiIndexWithoutTumo(const POINT &point, int size) {
    if (point.y > 512 && point.y < 576) {
      const int index =  (point.x - 80) / 33;
      return index >= size ? -1 : index;
       
    }
    return -1;
  }

  void DrawHand(const Controller::Input &input, const Model::Player &self, const Utility::PaiImage &paiImage) {
    const int size = self.GetHandSize();
    const int selectedIndex = SelectedPaiIndex(input.GetPoint(), size);
    //FEVERMJ_LOG("ÉäÅ[É` %x\n, ", reachIndex);
    const bool checkReach = !(reachIndex && !(reachIndex & (1 << selectedIndex)));
    for (int i = 0; i < size; ++i) {
      const int y = isSelect && i == selectedIndex && self.IsCutablePai(i) && checkReach ? 496 : 512;
      const auto pai = self.GetHandPai(i);
      const int handle = pai != Model::Pai::Invalid ?
                         pai >= Model::squealOffset ?
                         paiImage.GetUpHandle(pai % Model::squealOffset) : paiImage.GetHandHandle(pai) : paiImage.GetBackHandle(0);
      DrawGraph(80 + 33 * i, y, handle, TRUE);
    }
    const Model::Pai tumo = self.GetTumo();
    if (tumo != Model::Pai::Invalid) {
      const int y = isSelect && selectedIndex == 14 && checkReach ? 496 : 512;
      DrawGraph(105 + 33 * size, y, paiImage.GetHandHandle(tumo), TRUE);
    }
    if (isSelect && input.IsClecked()) {
      if (selectedIndex >= 0 && selectedIndex < size && self.IsCutablePai(selectedIndex) && checkReach) {
        discardAction(selectedIndex);
      } else if (selectedIndex == 14 && checkReach) {
        discardAction(-1);
      }
    }
  }

  void DrawTiHand(const Controller::Input &input, const Model::Player &self, const Utility::PaiImage &paiImage) {
    const int size = self.GetHandSize();
    const int selectedIndex = SelectedPaiIndexWithoutTumo(input.GetPoint(), size);
    const auto selectedPai = selectedIndex >= 0 ? self.GetHandPai(selectedIndex) : Model::Pai::Invalid;
    std::pair<int, int> tiPair = {-1, -1};
    for (const auto &pair : tiList) {
      if (selectedPai == pair.second) {
        tiPair = {pair.second, pair.first};
      } else if (selectedPai == pair.first) {
        tiPair = {pair.first, pair.second};
      }
    }
    int tiCount = 0;
    std::pair<Model::Pai, Model::Pai> erasePair = {Model::Pai::Invalid, Model::Pai::Invalid};
    for (int i = 0; i < self.GetHandSize(); ++i) {
      int y = 512;
      const Model::Pai pai = self.GetHandPai(i);
      if (!tiCount) {
        if (pai == tiPair.first) {
          tiCount = 1;
          erasePair.first = pai;
          y -= 16;
        } else if (pai == tiPair.second) {
          tiCount = 2;
          erasePair.first = pai;
          y -= 16;
        }
      }
      if (tiCount == 2 && pai == tiPair.first) {
        tiCount = 3;
        erasePair.second = pai;
        y -= 16;
      }
      if (tiCount == 1 && pai == tiPair.second) {
        tiCount = 3;
        erasePair.second = pai;
        y -= 16;
      }
      DrawGraph(80 + 33 * i, y, paiImage.GetHandHandle(pai), TRUE);
    }
    if (erasePair.first != Model::Pai::Invalid && erasePair.second != Model::Pai::Invalid && input.IsClecked()) {
      tiAction(erasePair);
    }
  }

  void DrawKanHand(const Controller::Input &input, const Model::Player &self, const Utility::PaiImage &paiImage) {
    const int size = self.GetHandSize();
    const auto tumo = self.GetTumo();
    const int selectedIndex = SelectedPaiIndex(input.GetPoint(), size);
    const auto selectedPai = selectedIndex >= 0 && selectedIndex < size ? 
                             self.GetHandPai(selectedIndex) : 
                             Model::Pai::Invalid;
    const auto darkKanPai = selectedPai != Model::Pai::Invalid && self.IsDarkKanEnablePai(selectedPai) ?
                            selectedPai : selectedIndex == 14 && self.IsDarkKanEnablePai(tumo) ?
                            tumo : 
                            Model::Pai::Invalid;
    for (int i = 0; i < self.GetHandSize(); ++i) {
      const auto currentPai = self.GetHandPai(i);
      const int y = currentPai == darkKanPai || (i == selectedIndex && self.IsAddKanEnablePai(currentPai)) ? 496 : 512;
      DrawGraph(80 + 33 * i, y, paiImage.GetHandHandle(currentPai), TRUE);
    }
    const int y = tumo == darkKanPai || (selectedIndex == 14 && self.IsAddKanEnablePai(tumo)) ? 496 : 512;
    DrawGraph(105 + 33 * size, y, paiImage.GetHandHandle(tumo), TRUE);
    if (input.IsClecked()) {
      if (selectedIndex >= 0 && selectedIndex < size) {
        const auto currentPai = self.GetHandPai(selectedIndex);
        if (currentPai == darkKanPai) {
          kanAction(currentPai, Model::Pai::Invalid);
        } else if (self.IsAddKanEnablePai(currentPai)) {
          kanAction(Model::Pai::Invalid, currentPai);
        }
      } else if (selectedIndex == 14) {
        if (tumo == darkKanPai) {
          kanAction(tumo, Model::Pai::Invalid);
        } else if (self.IsAddKanEnablePai(tumo)) {
          kanAction(Model::Pai::Invalid, tumo);
        }
      }
    }
  }

  void DrawRiver(const Model::Player &self, const Utility::PaiImage &paiImage) {
    const int size = self.GetRiverSize();
    int x = 320;
    for (int i = 0; i < size; ++i) {
      if (!(i % 6)) {
        x = 320;
      }
      const auto pai = self.GetRiverImageHandle(i);
      DrawGraph(x, 320 + 44 * (i / 6),
                pai < Model::squealOffset ? paiImage.GetUpHandle(pai) : paiImage.GetRightHandle(pai % Model::squealOffset), TRUE);
      x += self.GetRiverImageHandle(i) < Model::squealOffset ? 33 : 44;
    }
  }

  void DrawSqueal(const Model::Field &field, const Model::Player &self, const Utility::PaiImage &paiImage) {
    int pos = field.GetFirstParentHouse() == Model::House::Self ? 738 : 800;
    const int size = self.GetSquealSize();
    for (int i = 0; i < size; ++i) {
      const int pai = self.GetSquealImageHandle(i);
      pos -= pai < Model::squealOffset ? 33 : 44;
      const int image = pai < 0 ?
                        paiImage.GetBackHandle(0) : pai < Model::squealOffset ?
                        paiImage.GetUpHandle(pai) : paiImage.GetRightHandle(pai % Model::squealOffset);
      if (pai >= Model::squealOffset * 2) {
        DrawGraph(pos, 514, image, TRUE);
      }
      DrawGraph(pos, pai < Model::squealOffset ? 536 : 546, image, TRUE);
    }
  }

  bool isFlowSet = false;
  bool isKan = false;
  bool isTi = false;
  bool isSelect = false;
  std::uint32_t reachIndex = 0x0;
  std::function<void (int)> discardAction{[](int) {}};
  std::function<void (Model::Pai, Model::Pai)> kanAction{[](Model::Pai, Model::Pai) {}};
  std::function<void (const Model::TiPair &)> tiAction{[](const Model::TiPair &) {}};
  std::vector<std::pair<Model::Pai, Model::Pai>> tiList;
};
}}

#endif
