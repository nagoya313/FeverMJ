#ifndef FEVERMJ_MODEL_SQUEAL_HPP_
#define FEVERMJ_MODEL_SQUEAL_HPP_
#include <cassert>
#include <cstdint>
#include <vector>
#include "house.hpp"
#include "pai.hpp"
#include "role.hpp"
#include "squeal_role.hpp"
#include "../utility/algtorithm.hpp"

namespace FeverMJ { namespace Model {
class Squeal {
 public:
  void Init() {
    northCount = ponBits = tiBits = darkKanBits = lightKanBits = 0;
    squealImageList.clear();
  }

  void AddNorth() {
    ++northCount;
    assert(northCount >= 0 && northCount <= 4);
    squealImageList.push_back(Pai::North);
  }

  void AddPon(House house, Pai pai) {
    assert(pai != Pai::Invalid);
    ponBits |= 1 << pai;
    if (house == House::Down) {
      squealImageList.push_back(pai + squealOffset);
    } else {
      squealImageList.push_back(pai);
    }
    squealImageList.push_back(pai);
    if (house == House::Up) {
      squealImageList.push_back(pai + squealOffset);
    } else {
      squealImageList.push_back(pai);
    }
  }

  void AddTi(Pai squealPai, const std::pair<Pai, Pai> &pais) {
    assert(squealPai != Pai::Invalid);
    assert(pais.first != Pai::Invalid);
    assert(pais.second != Pai::Invalid);
    assert(pais.first < pais.second);
    tiBits |= 1 << std::min(squealPai, pais.first);
    squealImageList.push_back(pais.second);
    squealImageList.push_back(pais.first);
    squealImageList.push_back(squealPai + squealOffset);
  }

  void AddDarkKan(Pai pai) {
    assert(pai != Pai::Invalid);
    darkKanBits |= 1 << pai;
    squealImageList.push_back(Pai::Invalid);
    squealImageList.push_back(pai);
    squealImageList.push_back(pai);
    squealImageList.push_back(Pai::Invalid);
  }

  void AddAddKan(Pai pai) {
    ponBits &= ~(1 << pai);
    lightKanBits |= 1 << pai;
    const auto it = boost::find(squealImageList, pai + squealOffset);
    assert(it != squealImageList.end());
    *it += squealOffset;
  }

  void AddLightKan(House house, Pai pai) {
    lightKanBits |= 1 << pai;
    if (house == House::Down) {
      squealImageList.push_back(pai + squealOffset);
    } else {
      squealImageList.push_back(pai);
    }
    squealImageList.push_back(pai);
    squealImageList.push_back(pai);
    if (house == House::Up) {
      squealImageList.push_back(pai + squealOffset);
    } else {
      squealImageList.push_back(pai);
    }
  }

  void AddRon(Pai pai) {
    assert(pai != Pai::Invalid);
    ponBits |= 1 << pai;
  }

  bool IsAddKanEnablePai(Pai pai) const {
    return ponBits & (1 << pai);
  }

  SquealRole GetSquealRole(const DoraVector &doras) const {
    return SquealRole{tiBits, ponBits, darkKanBits, lightKanBits, northCount, doras};
  }

  int GetSquealImageHandle(int i) const {
    return squealImageList[i];
  }

  int GetSquealSize() const {
    return squealImageList.size();
  }

  bool HasSevenPinzu() const {
    return darkKanBits & (1 << Pai::P7);
  }

  bool HasSevenSozu() const {
    return darkKanBits & (1 << Pai::S7);
  }

  int GetNorthCount() const {
    return northCount;
  }

 private:
  int northCount;
  std::uint32_t ponBits;
  std::uint32_t tiBits;
  std::uint32_t darkKanBits;
  std::uint32_t lightKanBits;
  std::vector<int> squealImageList;
};
}}

#endif
