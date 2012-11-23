#ifndef FEVERMJ_MODEL_YAMA_HAIPAI_HPP_
#define FEVERMJ_MODEL_YAMA_HAIPAI_HPP_
#include <cstddef>
#include <vector>
#include <boost/assert.hpp>
#include <boost/range/irange.hpp>
#include "pai_yama.hpp"
#include "../pai/pai.hpp"

namespace FeverMJ { namespace Model { namespace Yama {
class Haipai {
 public:
  static constexpr std::size_t size = 13;
  static constexpr std::size_t playersNum = 3;
  
  std::vector<Pai::Pai> Hoge(const PaiYama &yama) {
    BOOST_ASSERT(index < 3);
    std::vector<Pai::Pai> haipai;
    for (const int i : boost::irange(static_cast<std::size_t>(0), size)) {
      haipai.push_back(yama.At(index * size + i));
    }
    ++index;
    return haipai;
  }
 
 private:
  std::size_t index = 0;
};
}}}

#endif
