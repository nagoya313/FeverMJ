#ifndef FEVERMJ_MODEL_SQUEAL_HPP_
#define FEVERMJ_MODEL_SQUEAL_HPP_
#include <cassert>
#include <cstdint>
#include <vector>
#include "house.hpp"
#include "pai.hpp"
#include "role.hpp"
#include "squeal_role.hpp"
#include "../utility/algorithm.hpp"

namespace FeverMJ { namespace Model {
class Squeal {
 public:
  void Init() {
    kind = {};
    ponBits = tiBits = darkKanBits = lightKanBits = 0x0;
    squealImageList.clear();
  }

  void AddNorth() {
    ++kind[Pai::North];
    squealImageList.push_back(Pai::North);
  }

  int GetNorthCount() const {
    return kind[Pai::North];
  }

  void AddPon(House house, Pai pai) {
    assert(pai != Pai::Invalid);
    kind[pai] += 3;
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

  void AddTi(Pai squealPai, const TiPair &pais) {
    assert(squealPai != Pai::Invalid);
    assert(pais.first != Pai::Invalid);
    assert(pais.second != Pai::Invalid);
    assert(pais.first < pais.second);
    ++kind[squealPai];
    ++kind[pais.first];
    ++kind[pais.second];
    tiBits |= 1 << std::min(squealPai, pais.first);
    squealImageList.push_back(pais.second);
    squealImageList.push_back(pais.first);
    squealImageList.push_back(squealPai + squealOffset);
  }

  void AddDarkKan(Pai pai) {
    assert(pai != Pai::Invalid);
    kind[pai] += 4;
    darkKanBits |= 1 << pai;
    squealImageList.push_back(Pai::Invalid);
    squealImageList.push_back(pai);
    squealImageList.push_back(pai);
    squealImageList.push_back(Pai::Invalid);
  }

  void AddAddKan(Pai pai) {
    ponBits &= ~(1 << pai);
    ++kind[pai];
    lightKanBits |= 1 << pai;
    const auto it = boost::find(squealImageList, pai + squealOffset);
    assert(it != squealImageList.end());
    *it += squealOffset;
  }

  void AddLightKan(House house, Pai pai) {
    kind[pai] += 4;
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
    int doraCount = 0;
    for (int i = 0; i < paiKindMax; ++i) {
      doraCount += kind[i] * boost::count(doras, static_cast<Pai>(i));
    }
    return SquealRole{tiBits, ponBits, darkKanBits, lightKanBits, doraCount};
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

  PaiKindArray GetPaiKindArray() const {
    return kind;
  }

 private:
  PaiKindArray kind;
  std::uint32_t ponBits;
  std::uint32_t tiBits;
  std::uint32_t darkKanBits;
  std::uint32_t lightKanBits;
  std::vector<int> squealImageList;
};
}}

#endif
