#ifndef FEVERMJ_CONTROLLER_INPUT_HPP_
#define FEVERMJ_CONTROLLER_INPUT_HPP_
#include <DxLib.h>
#include <boost/noncopyable.hpp>

namespace FeverMJ { namespace Controller {
class Input : boost::noncopyable {
 public:
  void Update() {
    before = current;
    current = GetMouseInput();
  }

  bool IsClecked() const {
    return current & ~before & MOUSE_INPUT_LEFT;
  }

  POINT GetPoint() const {
    int x;
    int y;
    GetMousePoint(&x, &y);
    return {x, y};
  }

 private:
  DWORD before = 0;
  DWORD current = 0;
};
}}

#endif
