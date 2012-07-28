#ifndef FEVERMJ_MODEL_TUMO_MOUNTAIN_HPP_
#define FEVERMJ_MODEL_TUMO_MOUNTAIN_HPP_
#include <cassert>
#include <algorithm>
#include <deque>
#include <vector>
#include <DxLib.h>
#include "pai.hpp"

namespace FeverMJ { namespace Model {
class TumoMountain {
 public:
  void Init() {
    mountain.clear();
    for (int i = Pai::M1; i <= Pai::Center; ++i) {
      mountain.emplace_back(static_cast<Pai>(i));
      mountain.emplace_back(static_cast<Pai>(i));
      mountain.emplace_back(static_cast<Pai>(i));
      mountain.emplace_back(static_cast<Pai>(i));
    }
    std::random_shuffle(mountain.begin(),
                        mountain.end(),
                        [](int n) {return GetRand(n - 1);});
  }

  std::deque<Pai> PopOnePais() {
    assert(mountain.size() >= 18);
    std::deque<Pai> oneMountain(mountain.begin(),
                                std::next(mountain.begin(), 18));
    mountain.erase(mountain.begin(),
                   std::next(mountain.begin(), 18));
    return oneMountain;
  }

  HandVector PopFirstPais() {
    assert(mountain.size() >= 13);
    HandVector firstPai(mountain.begin(), std::next(mountain.begin(), 13));
    mountain.erase(mountain.begin(), std::next(mountain.begin(), 13));
    return firstPai;
  }

  Pai PopTumoPai() {
    assert(!mountain.empty());
    const Pai pai = mountain[0];
    mountain.pop_front();
    return pai;
  }

  int GetMountainSize() const {
    return mountain.size();
  }

  void PopBackPai() {
    assert(!mountain.empty());
    mountain.pop_back();
  }

 private:
  std::deque<Pai> mountain;
};
}}

#endif
