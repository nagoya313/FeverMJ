#ifndef FEVERMJ_MODEL_YAMA_TSUMO_HPP_
#define FEVERMJ_MODEL_YAMA_TSUMO_HPP_
#include <boost/assert.hpp>
#include <cstddef>
#include "haipai.hpp"
#include "dora.hpp"
#include "rinsyan.hpp"

namespace FeverMJ { namespace Model { namespace Yama {
class Tsumo {
 public:
  Pai::Pai Tsumoru(const PaiYama &yama) {
    BOOST_ASSERT(index < offset + size);
    return yama.At(index++);
  }
  
  void Reduce() {
    --size;
  }
  
  std::size_t Size() const {
    return size;
  }
  
 private:
  static constexpr std::size_t
  offset = Haipai::size * Haipai::playersNum + Dora::size + Rinsyan::size;
  std::size_t size = PaiYama::size - offset;
  std::size_t index = offset;
};
}}}

#endif
