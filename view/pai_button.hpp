#ifndef FEVERMJ_VIEW_PAI_BUTTON_HPP_
#define FEVERMJ_VIEW_PAI_BUTTON_HPP_
#include <functional>
#include <string>
#include <DxLib.h>
#include <boost/noncopyable.hpp>
#include "../controller/input.hpp"
#include "../model/pai.hpp"
#include "../utility/pai_image.hpp"

namespace FeverMJ { namespace View {
class PaiButton {
 public:
  PaiButton() = default;

  explicit PaiButton(int l, int t) : left(l), top(t) {}

  void Update(const Controller::Input &input, const Utility::PaiImage &paiImage) {
    if (!hide) {
      const int imageHandle = isFlow ? isTenpai ? paiImage.GetUpHandle(pai) : paiImage.GetBackHandle(0) : paiImage.GetHandHandle(pai);
      if (selectable) {
        const bool on = OnCursor(input);
        DrawGraph(left, on ? top - 16 : top , imageHandle, TRUE);
        if (on && input.IsClecked()) {
          action();
        }
      } else {
        DrawGraph(left, top, imageHandle, TRUE);
      }
    }
  }

  template <typename Action>
  void SetClickAction(Action act) {
    action = act;
  }

  template <typename Action>
  void SetSelectAction(Action act) {
    select = act;
  }

  void SetPai(Model::Pai p) {
    pai = p;
  }

  void SetHide(bool h) {
    hide = h;
  }
  
  void SetFlow(bool f) {
    isFlow = f;
  }

  void SetTenpai(bool t) {
    isTenpai = t;
  }

  void SetSelectable(bool s) {
    selectable = s;
  }

  void SetPoint(const POINT &point) {
    left = point.x;
    top = point.y;
  }

 private:
  bool OnCursor(const Controller::Input &input) const {
    const POINT p = input.GetPoint();
    return p.x > left && p.x < left + 33 && p.y > top && p.y < top + 64;
  }

  int left;
  int top;
  Model::Pai pai = Model::Pai::Invalid;
  std::function<void ()> action = [] {};
  std::function<void ()> select = [] {};
  bool hide = false;
  bool isFlow = false;
  bool isTenpai = false;
  bool selectable = false;
};
}}

#endif
