#ifndef FEVERMJ_MODEL_ONE_MOUNTAIN_HPP_
#define FEVERMJ_MODEL_ONE_MOUNTAIN_HPP_
#include <cassert>
#include <deque>
#include <vector>
#include "house.hpp"
#include "pai.hpp"

namespace FeverMJ { namespace Model {
class OneMountain {
 public:
  void Init(std::deque<Pai> &&m) {
    mountain = std::move(m);
    doraCount = 1;
  }

  Pai PopRinsyanPai() {
    assert(mountain.size() > 10);
    const Pai Pai = *std::prev(mountain.end());
    mountain.pop_back();
    return Pai;
  }

  void AddDora() {
    ++doraCount;
  }

  int GetDoraCount() const {
    return doraCount;
  }

  Pai GetDisplayDora(int i) const {
    return i < GetDoraCount() ? mountain[10 - 2 * i] : Pai::Invalid;
  }

  Pai GetDisplayReverceDora(int i) const {
    return i < GetDoraCount() ? mountain[10 - (2 * i + 1)] : Pai::Invalid;
  }

  DoraVector GetDoraList(bool withReverse) const {
    DoraVector doraList = {Pai::M1, Pai::M9, Pai::North};
    for (int i = 0; i < GetDoraCount(); ++i) {
      doraList.push_back(GetDora(GetDisplayDora(i)));
      if (withReverse) {
        doraList.push_back(GetDora(GetDisplayReverceDora(i)));
      }
    }
    return doraList;
  }

 private:
  int doraCount;
  std::deque<Pai> mountain;
};
}}

#endif
