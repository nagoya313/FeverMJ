#ifndef FEVERMJ_MODEL_SQUEAL_ROLE_HPP_
#define FEVERMJ_MODEL_SQUEAL_ROLE_HPP_
#include <cassert>
#include <cstdint>
#include <algorithm>
#include "pai.hpp"
#include "role.hpp"

namespace FeverMJ { namespace Model {
class SquealRole {
 public:
  explicit SquealRole(std::uint32_t tiBits,
                      std::uint32_t ponBits,
                      std::uint32_t darkKanBits,
                      std::uint32_t lightKanBits,
                      int northCount,
                      const DoraVector &doras) {
    int darkKanCount = 0;
    int lightKanCount = 0;
    for (int i = 0; i < paiKindMax; ++i) {
      if (tiBits & (1 << i)) {
        CheckTiRoleState(doras, i);
      }
      if (ponBits & (1 << i)) {
        CheckPonRoleState(doras, i);
      } else if (darkKanBits & (1 << i)) {
        ++darkKanCount;
        CheckKanRoleState(doras, i, 16);
      } else if (lightKanBits & (1 << i)) {
        ++lightKanCount;
        CheckKanRoleState(doras, i, 8);
      }
    }
    doraCount += northCount;
    darkTripleCount = darkKanCount;
    quadrupleCount = darkKanCount + lightKanCount;
    tripleBits = ponBits | darkKanBits | lightKanBits;
    paiKindBits |= tripleBits;
    assert(darkTripleCount >= 0 && darkTripleCount <= 4);
    assert(quadrupleCount >= 0 && quadrupleCount <= 4);
  }

  int GetDoraCount() const {
    return doraCount;
  }

  int GetHu() const {
    return huCount;
  }

  bool IsTyanta() const {
    return isTyanta;
  }

  std::uint32_t GetTripleBits() const {
    return tripleBits;
  }

  std::uint32_t GetStraightBits() const {
    return straightBits;
  }

  bool HasStraight() const {
    return hasStraight;
  }

  int GetDarkTripleCount() const {
    return darkTripleCount;
  }

  int GetQuadrupleCount() const {
    return quadrupleCount;
  }

  std::uint32_t GetPaiKindBits() const {
    return paiKindBits;
  }

  bool IsBenz() const {
    return isBenz;
  }

 private:
  void CheckTiRoleState(const DoraVector &doras, int pai) {
    paiKindBits |= (1 << pai) | (1 << (pai + 1)) | (1 << (pai + 2));
    for (int i = 0; i < 3; ++i) {
      for (const Pai dora : doras) {
        if (pai + i == dora) {
          ++doraCount;
        }
      }
    }
    const int number = GetNumber(pai);
    if (number > 0 && number < 6) {
      isTyanta = false;
    }
    if (!(number % 3)) {
      straightBits |= 1 << (3 * GetColor(pai) + number / 3);
    }
    hasStraight = true;
  }

  void CheckPonKanRoleState(int pai, int hu) {
    if (IsTyuntyanPai(pai)) {
      isTyanta = false;
      huCount += hu;
    } else {   
      huCount += hu * 2;
    }
  }

  void CheckPonRoleState(const DoraVector &doras, int pai) {
    CheckPonKanRoleState(pai, 2);
    for (const auto dora : doras) {
      if (pai == dora) {
        doraCount += 3;
      }
    }
  }

  void CheckKanRoleState(const DoraVector &doras, int pai, int hu) {
    CheckPonKanRoleState(pai, hu);
    for (const auto dora : doras) {
      if (pai == dora) {
        doraCount += 4;
      }
    }
    if (pai == Pai::S8) {
      isBenz = true;
    }
  }

  int huCount = 0;
  int doraCount = 0;
  int darkTripleCount = 0;
  int quadrupleCount = 0;
  std::uint32_t tripleBits = 0x0;
  std::uint32_t straightBits = 0x0;
  std::uint32_t paiKindBits = 0x0;
  bool hasStraight = false;
  bool isTyanta = true;
  bool isBenz = false;
};
}}

#endif
