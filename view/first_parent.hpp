#ifndef FEVERMJ_VIEW_FIRST_PARENT_HPP_
#define FEVERMJ_VIEW_FIRST_PARENT_HPP_
#include <cassert>
#include <DxLib.h>
#include <boost/noncopyable.hpp>
#include "../model/field.hpp"
#include "../model/house.hpp"
#include "../utility/pai_image.hpp"

namespace FeverMJ { namespace View {
class FirstParent : boost::noncopyable {
 public:
  void Draw(const Model::Field &field, const Utility::PaiImage &paiImage) {
    switch (field.GetFirstParent()) {
      case Model::House::Up:
        DrawGraph(96, 386, paiImage.GetFirstParentHandle(2), TRUE);
        break;
      case Model::House::Self:
        DrawGraph(608, 416, paiImage.GetFirstParentHandle(0), TRUE);
        break;
      case Model::House::Down:
        DrawGraph(640, 32, paiImage.GetFirstParentHandle(3), TRUE);
        break;
      default:
	    assert(false);
        break;
    }
  }
};
}}

#endif
