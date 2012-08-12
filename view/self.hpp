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
  void Draw(const Controller::Input &input, const Model::Player &self, const Utility::PaiImage &paiImage) {
    switch (state) {
      case State::Kan:
        DrawKanHand(input, self, paiImage);
        break;
      case State::Ti:
        DrawTiHand(input, self, paiImage);
        break;
      default:
        DrawHand(input, self, paiImage);
        break;
    }
    DrawSqueal(self, paiImage);
    DrawRiver(self, paiImage);
  }

  void SetFlowSet() {
    state = State::Flow;
  }

  void SetStart() {
    state = State::None;
  }

  template <typename Action>
  void SetSelectKanMode(Action action) {
    state = State::Kan;
    kanAction = action;
  }

  template <typename Action>
  void SetSelectTiMode(Action action, const std::vector<Model::TiPair> &list) {
    state = State::Ti;
    tiAction = action;
    tiList = list;
  }

  template <typename Action>
  void SetSelectReachMode(std::uint32_t index, Action action) {
    state = State::Select;
    reachIndex = index;
    discardAction = action;
  }

  template <typename Action>
  void SetSelectMode(Action action) {
    state = State::Select;
    discardAction = action;
  }

  void SetWaitMode() {
    state = State::None;
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
    const bool checkReach = !(reachIndex && !(reachIndex & (1 << selectedIndex)));
    for (int i = 0; i < size; ++i) {
      const int y = state == State::Select && i == selectedIndex && self.IsCutablePai(i) && checkReach ? 496 : 512;
      DrawGraph(80 + 33 * i, y, paiImage.GetSelfHandHandle(self.GetHandPai(i)), TRUE);
    }
    if (const auto tumo = self.GetTumo()) {
      const int y = state == State::Select && selectedIndex == 14 && checkReach ? 496 : 512;
      DrawGraph(105 + 33 * size, y, paiImage.GetHandHandle(*tumo), TRUE);
    }
    if (state == State::Select && input.IsClecked()) {
      if (selectedIndex >= 0 && selectedIndex < size && self.IsCutablePai(selectedIndex) && checkReach) {
        discardAction(selectedIndex);
      } else if (selectedIndex == 14 && checkReach) {
        discardAction(-1);
      }
    }
  }

  Model::TiOptionalPair GetTiPair(const Model::PaiOptional &selectedPai) const {
    Model::TiOptionalPair tiPair;
    for (const auto &pair : tiList) {
      if (selectedPai == pair.second) {
        tiPair = {pair.second, pair.first};
      } else if (selectedPai == pair.first) {
        tiPair = {pair.first, pair.second};
      }
    }
    return tiPair;
  }

  void DrawTiHand(const Controller::Input &input, const Model::Player &self, const Utility::PaiImage &paiImage) {
    const int size = self.GetHandSize();
    const int selectedIndex = SelectedPaiIndexWithoutTumo(input.GetPoint(), size);
    const Model::PaiOptional selectedPai = selectedIndex >= 0 ?
                                           boost::make_optional(self.GetHandPai(selectedIndex)) : 
                                           boost::none;
    const auto tiPair = GetTiPair(selectedPai);
    int tiCount = 0;
    Model::TiOptionalPair erasePair;
    for (int i = 0; i < self.GetHandSize(); ++i) {
      int y = 512;
      const auto pai = self.GetHandPai(i);
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
    if (erasePair.first && erasePair.second && input.IsClecked()) {
      tiAction({*erasePair.first, *erasePair.second});
    }
  }

  void DrawKanHand(const Controller::Input &input, const Model::Player &self, const Utility::PaiImage &paiImage) {
    const int size = self.GetHandSize();
    const auto tumo = *self.GetTumo();
    const int selectedIndex = SelectedPaiIndex(input.GetPoint(), size);
    const boost::optional<Model::Pai> selectedPai = selectedIndex >= 0 && selectedIndex < size ? 
                                                    boost::make_optional(self.GetHandPai(selectedIndex)) : 
                                                    boost::none;
    const boost::optional<Model::Pai> darkKanPai = selectedPai && self.IsDarkKanEnablePai(*selectedPai) ?
                                                   *selectedPai : self.IsDarkKanEnablePai(tumo) ?
                                                   boost::make_optional(tumo) : boost::none;
    for (int i = 0; i < self.GetHandSize(); ++i) {
      const auto currentPai = self.GetHandPai(i);
      const int y = (darkKanPai && currentPai == darkKanPai) ||
                    (i == selectedIndex && self.IsAddKanEnablePai(currentPai)) ? 496 : 512;
      DrawGraph(80 + 33 * i, y, paiImage.GetHandHandle(currentPai), TRUE);
    }
    const int y = tumo == darkKanPai || (selectedIndex == 14 && self.IsAddKanEnablePai(tumo)) ? 496 : 512;
    DrawGraph(105 + 33 * size, y, paiImage.GetHandHandle(tumo), TRUE);
    if (input.IsClecked()) {
      if (selectedIndex >= 0 && selectedIndex < size) {
        const auto currentPai = self.GetHandPai(selectedIndex);
        if (currentPai == darkKanPai) {
          kanAction(currentPai, boost::none);
        } else if (self.IsAddKanEnablePai(currentPai)) {
          kanAction(boost::none, currentPai);
        }
      } else if (selectedIndex == 14) {
        if (darkKanPai && tumo == darkKanPai) {
          kanAction(tumo, boost::none);
        } else if (self.IsAddKanEnablePai(tumo)) {
          kanAction(boost::none, tumo);
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
      DrawGraph(x, 320 + 44 * (i / 6), paiImage.GetSelfRiverHandle(pai), TRUE);
      x += pai < Model::squealOffset ? 33 : 44;
    }
  }

  void DrawSqueal(const Model::Player &self, const Utility::PaiImage &paiImage) {
    int pos = 800;
    const int size = self.GetSquealSize();
    for (int i = 0; i < size; ++i) {
      const int pai = self.GetSquealImageHandle(i);
      pos -= pai < Model::squealOffset ? 33 : 44;
      if (pai >= Model::squealOffset * 2) {
        DrawGraph(pos, 519, paiImage.GetSelfSquealHandle(pai), TRUE);
      }
      DrawGraph(pos, pai < Model::squealOffset ? 541 : 551, paiImage.GetSelfSquealHandle(pai), TRUE);
    }
  }

  enum class State {
    None,
    Flow,
    Kan,
    Ti,
    Select
  } state = State::None;
  std::uint32_t reachIndex = 0x0;
  std::function<void (int)> discardAction{[](int) {}};
  std::function<void (const boost::optional<Model::Pai> &, const boost::optional<Model::Pai> &)> 
      kanAction{[](const boost::optional<Model::Pai> &, const boost::optional<Model::Pai> &) {}};
  std::function<void (const Model::TiPair &)> tiAction{[](const Model::TiPair &) {}};
  std::vector<std::pair<Model::Pai, Model::Pai>> tiList;
};
}}

#endif
