#ifndef FEVERMJ_VIEW_FIRST_PARENT_HPP_
#define FEVERMJ_VIEW_FIRST_PARENT_HPP_
#include <DxLib.h>
#include <boost/noncopyable.hpp>
#include "../model/field.hpp"
#include "../model/house.hpp"
#include "../utility/pai_image.hpp"

namespace FeverMJ { namespace View {
class FirstParent : boost::noncopyable {
 public:
  void Draw(const Model::Field &field, const Utility::PaiImage &paiImage) {
    switch (field.GetFirstParentHouse()) {
      case Model::House::Up:
        DrawGraph(-16, 537, paiImage.GetFirstParentHandle(2), TRUE);
        break;
      case Model::House::Self:
        DrawGraph(738, 549, paiImage.GetFirstParentHandle(0), TRUE);
        break;
      case Model::House::Down:
        DrawGraph(752, -2, paiImage.GetFirstParentHandle(3), TRUE);
        break;
      default:
        break;
    }
  }
};
}}

#endif
