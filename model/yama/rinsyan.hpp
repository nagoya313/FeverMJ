#ifndef FEVERMJ_MODEL_YAMA_RINSYAN_TYPE_HPP_
#define FEVERMJ_MODEL_YAMA_RINSYAN_HPP_
#include <cstddef>
#include <boost/assert.hpp>
#include "haipai.hpp"
#include "pai_yama.hpp"
#include "dora.hpp"
#include "../pai/pai.hpp"

namespace FeverMJ { namespace Model { namespace Yama {
class Rinsyan {
 public:
  static constexpr size_t size = 8;
  
  Pai::Pai Tsumo(const PaiYama &yama) {
    BOOST_ASSERT(index < offset + size);
    return yama.At(index++);
  }
  
  std::size_t Size() const {
    return offset + size - index;
  }

 private:
  static constexpr
  std::size_t offset = Haipai::size * Haipai::playersNum + Dora::size;
  std::size_t index = offset;
};
}}}

#endif
