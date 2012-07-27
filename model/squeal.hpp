#ifndef FEVERMJ_MODEL_SQUEAL_HPP_
#define FEVERMJ_MODEL_SQUEAL_HPP_
#include <cassert>
#include <cstdint>
#include <algorithm>
#include <vector>
#include "house.hpp"
#include "pai.hpp"
#include "role.hpp"

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
    const auto it = std::find(squealImageList.begin(),
                              squealImageList.end(),
                              pai + squealOffset);
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

  RoleSquealState GetRoleState(const DoraVector &doras) const {
    RoleSquealState role = {};
    role.isTyanta = true;
    int darkKanCount = 0;
    int lightKanCount = 0;
    for (int i = 0; i < paiKindMax; ++i) {
      if (tiBits & (1 << i)) {
        CheckTiRoleState(doras, i, role);
      }
      if (ponBits & (1 << i)) {
        CheckPonRoleState(doras, i, role);
      } else if (darkKanBits & (1 << i)) {
        ++darkKanCount;
        CheckKanRoleState(doras, i, 16, role);
      } else if (lightKanBits & (1 << i)) {
        ++lightKanCount;
        CheckKanRoleState(doras, i, 8, role);
      }
    }
    role.doraCount += northCount;
    role.darkTripleCount = darkKanCount;
    role.quadrupleCount = darkKanCount + lightKanCount;
    role.tripleBits = ponBits | darkKanBits | lightKanBits;
    role.paiKindBits |= role.tripleBits;
    assert(role.darkTripleCount >= 0 && role.darkTripleCount <= 4);
    assert(role.quadrupleCount >= 0 && role.quadrupleCount <= 4);
    return role;
  }

  int GetSquealImageHandle(int i) const {
    return squealImageList[i];
  }

  int GetSquealSize() const {
    return squealImageList.size();
  }

 private:
  void CheckTiRoleState(const DoraVector &doras, int pai, RoleSquealState &role) const {
    role.paiKindBits |= (1 << pai) | (1 << (pai + 1)) | (1 << (pai + 2));
    for (int i = 0; i < 3; ++i) {
      for (const Pai dora : doras) {
        if (pai + i == dora) {
          ++role.doraCount;
        }
      }
    }
    const int number = GetNumber(pai);
    if (number > 0 && number < 6) {
      role.isTyanta = false;
    }
    if (!(number % 3)) {
      role.straightBits |= 1 << (3 * GetColor(pai) + number / 3);
    }
    role.hasStraight = true;
  }

  void CheckPonKanRoleState(int pai, int hu, RoleSquealState &role) const {
    if (IsTyuntyanPai(pai)) {
      role.isTyanta = false;
      role.huCount += hu;
    } else {   
      role.huCount += hu * 2;
    }
  }

  void CheckPonRoleState(const DoraVector &doras, int pai, RoleSquealState &role) const {
    CheckPonKanRoleState(pai, 2, role);
    for (const Pai dora : doras) {
      if (pai == dora) {
        role.doraCount += 3;
      }
    }
  }

  void CheckKanRoleState(const DoraVector &doras, int pai, int hu, RoleSquealState &role) const {
    CheckPonKanRoleState(pai, hu, role);
    for (const Pai dora : doras) {
      if (pai == dora) {
        role.doraCount += 4;
      }
    }
    if (pai == Pai::S8) {
      role.isBenz = true;
    }
  }

  int northCount;
  std::uint32_t ponBits;
  std::uint32_t tiBits;
  std::uint32_t darkKanBits;
  std::uint32_t lightKanBits;
  std::vector<int> squealImageList;
};
}}

#endif
