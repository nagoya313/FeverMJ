#ifndef FEVERMJ_VIEW_SQUEAL_HPP_
#define FEVERMJ_VIEW_SQUEAL_HPP_
#include <cstdint>
#include <vector>
#include <DxLib.h>
#include <boost/noncopyable.hpp>
#include "button.hpp"
#include "../controller/input.hpp"

namespace FeverMJ { namespace View {
namespace MenuMode_ {
enum MenuMode : std::uint32_t {
   Kan = 0x0,
   EraseNorth,
   Tumo,
   Pon,
   Ti,
   Reach,
   OpenReach,
   Ron,
   Cancel,
   Fever,
   DoubleFever,
   ModeMax,
};
}

using MenuMode_::MenuMode;

class Squeal : boost::noncopyable {
 public:
  Squeal() {
    constexpr int poses[] = {200, 0, 600, 100, 0, 300, 450, 500, 700, 300, 500};
    const std::string texts[] = {
      "カン", "ペー", "ツモ", "ポン", "チー", "リーチ", "オープン", "ロン", "パス", "フィーバー", "Ｗフィーバー"
    };
    for (std::uint32_t i = 0; i < MenuMode::ModeMax; ++i) {
      buttons.emplace_back(poses[i], i < 9 ? 0 : 32, texts[i]);
    }
  }

  void Draw(const Controller::Input &input) {
    for (auto &button : buttons) {
      button.Update(input);
    }
  }

  template <typename Action>
  void SetMode(Action action, MenuMode mode) {
    buttons[mode].SetAction(action);
    buttons[mode].SetHide(false);
  }

  void AllReset() {
    for (auto &button : buttons) {
      button.SetHide(true);
    }
  }

 private:
  std::vector<Button> buttons;
};
}}

#endif
