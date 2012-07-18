#ifndef FEVERMJ_VIEW_BREAK_HOUSE_HPP_
#define FEVERMJ_VIEW_BREAK_HOUSE_HPP_
#include <DxLib.h>
#include <boost/noncopyable.hpp>
#include "../model/field.hpp"
#include "../model/house.hpp"
#include "../utility/pai_image.hpp"

namespace FeverMJ { namespace View {
class BreakHouse : boost::noncopyable {
 public:
  void Draw(const Model::Field &field, const Utility::PaiImage &paiImage) {
    switch (field.GetBreakHouse()) {
      case 0:
        DrawGraph(288, 192, paiImage.GetBreakHouseHandle(), TRUE);
        break;
      case 1:
        DrawGraph(338, 280, paiImage.GetBreakHouseHandle(), TRUE);
        break;
      case 2:
        DrawGraph(520, 192, paiImage.GetBreakHouseHandle(), TRUE);
        break;
      default:
        assert(false);
    }
  }
};
}}

#endif
