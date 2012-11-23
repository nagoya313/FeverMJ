#ifndef FEVERMJ_MODEL_YAMA_DORA_HPP_
#define FEVERMJ_MODEL_YAMA_DORA_HPP_
#include <cstddef>
#include <vector>
#include <boost/assert.hpp>
#include "haipai.hpp"
#include "pai_yama.hpp"
#include "../pai/pai.hpp"

namespace FeverMJ { namespace Model { namespace Yama {
class Dora {
 public:
  static constexpr std::size_t size = 10;
  
  void Add() {
    BOOST_ASSERT(num < size / 2);
    ++num;
  }
  
  std::size_t Count() const {
    BOOST_ASSERT(num < size / 2 + 1);
    return num;
  }
  
  Pai::Pai DorahyoAt(const PaiYama &yama, std::size_t index) const {
    BOOST_ASSERT(index < Count());
    return yama.At(offset + index);
  }
  
  std::vector<Pai::Pai> List(bool isReach, const PaiYama &yama) const {
    std::vector<Pai::Pai> list = {Pai::Pai::Manzu1(), Pai::Pai::Manzu9(),
                                  Pai::Pai::Pe()};
    const int max = isReach ? num * 2 : num;
    for (int i = 0; i < max; ++i) {
      list.push_back(Pai::DorahyoToDora(yama.At(offset + i)));
    }
    return list;
  }
 
 private:
  static constexpr std::size_t offset = Haipai::size * Haipai::playersNum;
  std::size_t num = 0;
};
}}}

#endif
