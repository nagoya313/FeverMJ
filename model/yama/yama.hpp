#ifndef FEVERMJ_MODEL_YAMA_YAMA_HPP_
#define FEVERMJ_MODEL_YAMA_YAMA_HPP_
#include <cstddef>
#include <vector>
#include <boost/assert.h>
#include "tsumo_type.hpp"
#include "../pai/pai.hpp"

namespace FeverMJ { namespace Model { namespace Yama {
class Yama {
 public:
  void Reset() {
    yama.Shuffle();
    Tsumo = {};
    dora = {};
    rinsyan = {};
  }
  
  Pai::Pai Tsumo(TsumoType type) {
    switch (type) {
      case Normal:
        return tsumo.Tsumo(yama);
      case Rinsyan:
        Tsumo.Reduce();
        return rinsyan.Tsumo(yama);
      case Nukipe:
        return rinsyan.Tsumo(yama);
      default:
        BOOST_ASSERT(false);
        return Pai::Pai::Pe();
    }
  }
  
  void AddDora() {
    dora.Add();
  }
  
  std::size_t DoraCount() const {
    dora.Count();
  }
  
  std::size_t TsumoCount() const {
    return tsumo.Count();
  }
  
  Pai::Pai DorahyoAt(const PaiYama &yama, std::size_t index) const {
    return dora.DorahyoAt(yama, index);
  }
  
  std::vector<Pai::Pai> DoraList(bool isReach) const {
    return dora.List(isReach, yama);
  }
  
 private:
  PaiYama yama;
  Tsumo tsumo;
  Dora dora;
  Rinsyan rinsyan;
};
}}}

#endif
