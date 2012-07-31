#ifndef FEVERMJ_VIEW_CLIENT_HPP_
#define FEVERMJ_VIEW_CLIENT_HPP_
#include <DxLib.h>
#include <boost/noncopyable.hpp>
#include "button.hpp"
#include "info.hpp"

namespace FeverMJ { namespace View {
class Client : boost::noncopyable {
 public:
  template <typename Act>
  void Init(Act act) {
    cancelButton.SetAction(act);
  }

  void Draw(const Controller::Input &input) {
    ClearDrawScreen();
    DrawBox(0, 0, screenWidth, screenHeight, defaultMatColor, true);
    DrawString(0, 0, "Ú‘±æ‚ÌIP‚ğ“ü‚ê‚é", GetColor(255, 255, 255));
    cancelButton.Update(input);
    ScreenFlip();
  }

 private:
  Button cancelButton{80, 200, "ƒLƒƒƒ“ƒZƒ‹"};
};
}}


#endif
