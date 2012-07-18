#ifndef FEVERMJ_VIEW_MAT_HPP_
#define FEVERMJ_VIEW_MAT_HPP_
#include <DxLib.h>
#include <boost/noncopyable.hpp>
#include "info.hpp"

namespace FeverMJ { namespace View {
class Mat : boost::noncopyable {
 public:
  void Draw() {
    DrawBox(0, 0, screenWidth, screenHeight, defaultMatColor, true);
  }
};
}}


#endif
