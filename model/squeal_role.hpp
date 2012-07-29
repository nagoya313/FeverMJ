#ifndef FEVERMJ_MODEL_SQUEAL_ROLE_HPP_
#define FEVERMJ_MODEL_SQUEAL_ROLE_HPP_
#include <cassert>
#include <cstdint>
#include <algorithm>
#include "pai.hpp"
#include "role.hpp"
#include "../utility/algorithm.hpp"

namespace FeverMJ { namespace Model {
class SquealRole {
 public:
  explicit SquealRole(PaiKindBitset tiBits,
                      PaiKindBitset ponBits,
                      PaiKindBitset darkKanBits,
                      PaiKindBitset lightKanBits,
                      int dora) : doraCount(dora) {
    int darkKanCount = 0;
    int lightKanCount = 0;
    for (int i = 0; i < paiKindMax; ++i) {
      if (tiBits & (1 << i)) {
        CheckTiRoleState(i);
      }
      if (ponBits & (1 << i)) {
        CheckPonRoleState(i);
      } else if (darkKanBits & (1 << i)) {
        ++darkKanCount;
        CheckKanRoleState(i, 16);
      } else if (lightKanBits & (1 << i)) {
        ++lightKanCount;
        CheckKanRoleState(i, 8);
      }
    }
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

  PaiKindBitset GetTripleBits() const {
    return tripleBits;
  }

  PaiKindBitset GetStraightBits() const {
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

  PaiKindBitset GetPaiKindBits() const {
    return paiKindBits;
  }

  bool IsBenz() const {
    return isBenz;
  }

 private:
  void CheckTiRoleState(int pai) {
    paiKindBits |= (1 << pai) | (1 << (pai + 1)) | (1 << (pai + 2));
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

  void CheckPonRoleState(int pai) {
    CheckPonKanRoleState(pai, 2);
  }

  void CheckKanRoleState(int pai, int hu) {
    CheckPonKanRoleState(pai, hu);
    if (pai == Pai::S8) {
      isBenz = true;
    }
  }

  int huCount = 0;
  int doraCount = 0;
  int darkTripleCount = 0;
  int quadrupleCount = 0;
  PaiKindBitset tripleBits = 0x0;
  PaiKindBitset straightBits = 0x0;
  PaiKindBitset paiKindBits = 0x0;
  bool hasStraight = false;
  bool isTyanta = true;
  bool isBenz = false;
};
}}

#endif
