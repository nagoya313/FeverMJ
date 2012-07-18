#ifndef FEVERMJ_VIEW_DORA_MOUNTAIN_HPP_
#define FEVERMJ_VIEW_DORA_MOUNTAIN_HPP_
#include <DxLib.h>
#include <boost/noncopyable.hpp>
#include "../model/field.hpp"
#include "../utility/pai_image.hpp"

namespace FeverMJ { namespace View {
class DoraMountain : boost::noncopyable {
 public:
  void Draw(const Model::Field &field, const Utility::PaiImage &paiImage) {
    for (int i = 0; i < 5; ++i) {
      DrawGraph(338 + 33 * (i % 6), 200, paiImage.GetBackHandle(0), TRUE);
      const auto displayDora = field.GetDisplayDora(i);
      DrawGraph(338 + 33 * (i % 6), 184, displayDora != Model::Pai::Invalid ?
                                         paiImage.GetUpHandle(displayDora) :
                                         paiImage.GetBackHandle(0), TRUE);
    }
  }
};
}}

#endif
