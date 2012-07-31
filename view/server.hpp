#ifndef FEVERMJ_VIEW_SERVER_HPP_
#define FEVERMJ_VIEW_SERVER_HPP_
#include <DxLib.h>
#include <boost/noncopyable.hpp>
#include "button.hpp"
#include "info.hpp"

namespace FeverMJ { namespace View {
class Server : boost::noncopyable {
 public:
  template <typename Act>
  void Init(Act act) {
    cancelButton.SetAction(act);
  }

  void Draw(const Controller::Input &input, int waitCount) {
    ClearDrawScreen();
    DrawBox(0, 0, screenWidth, screenHeight, defaultMatColor, true);
    DrawString(0, 0, "接続待機中", GetColor(255, 255, 255));
    DrawFormatString(0, 40, GetColor(255, 255, 255), "%d人接続", waitCount);
    cancelButton.Update(input);
    ScreenFlip();
  }

 private:
  Button cancelButton{80, 200, "キャンセル"};
};
}}


#endif
