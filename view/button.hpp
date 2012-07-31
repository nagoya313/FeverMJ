#ifndef FEVERMJ_VIEW_BUTTON_HPP_
#define FEVERMJ_VIEW_BUTTON_HPP_
#include <functional>
#include <string>
#include <DxLib.h>
#include <boost/noncopyable.hpp>
#include "../controller/input.hpp"

namespace FeverMJ { namespace View {
class Button {
 public:
  explicit Button(int l, int t, const std::string &str)
      : left(l),
        top(t),
        length(GetDrawStringWidth(str.c_str(), str.length() + 16)),
        text(str) {}

  void Update(const Controller::Input &input) {
    if (!hide) {
      const bool on = OnCursor(input);
      DrawString(left + 8, top + 8, text.c_str(), GetColor(on));
      if (on && input.IsClecked()) {
        action();
      }
    }
  }

  template <typename Action>
  void SetAction(Action act) {
    action = act;
  }

  void SetHide(bool h) {
    hide = h;
  }

 private:
  bool OnCursor(const Controller::Input &input) const {
    const POINT p = input.GetPoint();
    return p.x > left && p.x < left + length && p.y > top && p.y < top + 48;
  }

  static DWORD GetColor(bool selected) {
    return selected ? ::GetColor(255, 255, 255) : ::GetColor(128, 128, 128);
  }

  int left;
  int top;
  int length;
  std::string text;
  std::function<void ()> action = [] {};
  bool hide = false;
};

class ToggleButton {
 public:
  explicit ToggleButton(int l, int t, const std::string &str)
      : left(l),
        top(t),
        length(GetDrawStringWidth(str.c_str(), str.length() + 16)),
        text(str) {}

  void Update(const Controller::Input &input) {
    if (!hide) {
      const bool on = OnCursor(input);
      DrawString(left + 8, top + 8, text.c_str(), GetColor(on));
      if (on && input.IsClecked()) {
        toggle = !toggle;
        action(toggle);
      }
    }
  }

  template <typename Action>
  void SetAction(Action act) {
    action = act;
  }

  void SetHide(bool h) {
    hide = h;
  }

  bool IsToggle() const {
    return toggle;
  }

 private:
  bool OnCursor(const Controller::Input &input) const {
    const POINT p = input.GetPoint();
    return p.x > left && p.x < left + length && p.y > top && p.y < top + 48;
  }

  DWORD GetColor(bool selected) {
    return selected || toggle ? ::GetColor(255, 255, 255) : ::GetColor(128, 128, 128);
  }

  int left;
  int top;
  int length;
  std::string text;
  std::function<void (int)> action = [](int) {};
  bool hide = false;
  bool toggle = false;
};
}}

#endif
