#ifndef FEVERMJ_VIEW_TITLE_HPP_
#define FEVERMJ_VIEW_TITLE_HPP_
#include <DxLib.h>
#include <boost/noncopyable.hpp>
#include "button.hpp"
#include "info.hpp"

namespace FeverMJ { namespace View {
class Title : boost::noncopyable {
 public:
  template <typename Act1, typename Act2, typename Act3>
  void Init(Act1 act1, Act2 act2, Act3 act3) {
    offlineButton.SetAction(act1);
    serverButton.SetAction(act2);
    clientButton.SetAction(act3);
  }

  void Draw(const Controller::Input &input) {
    ClearDrawScreen();
    DrawBox(0, 0, screenWidth, screenHeight, defaultMatColor, true);
    DrawString(0, 0, "�t�B�[�o�[�H�����I�����C����", GetColor(255, 255, 255));
    offlineButton.Update(input);
    serverButton.Update(input);
    clientButton.Update(input);
    ScreenFlip();
  }

 private:
  Button offlineButton{80, 80, "�I�t���C���e�X�g"};
  Button serverButton{80, 120, "�T�[�o�[���[�h�e�X�g"};
  Button clientButton{80, 160, "�N���C�A���g���[�h�e�X�g"};
};
}}


#endif
