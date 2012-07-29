#ifndef FEVERMJ_MODEL_CHECK_TENPAI_HPP_
#define FEVERMJ_MODEL_CHECK_TENPAI_HPP_
#include <cassert>
#include <cstdint>
#include <vector>
#include "pai.hpp"
#include "role.hpp"
#include "tenpai_patern.hpp"
#include "../utility/algorithm.hpp"

namespace FeverMJ { namespace Model {
struct WaitPair {
  std::uint32_t waitPaiBits;
  std::vector<TenpaiPatern> tenpaiPatern;
};

inline
void EraseTriple(PaiKindArray &kind, TenpaiPatern &hand) {
  for (int i = 0; i < paiKindMax; ++i) {
    if (kind[i] >= 3) {
      kind[i] -= 3;
      hand.AddTriple(static_cast<Pai>(i));
    }
  }
}

inline
bool ErasedStraight(PaiKindArray &kind, TenpaiPatern &hand) {
  for (int i = Pai::P1; i < Pai::S8; ++i) {
    if (kind[i] && GetNumber(i) < 7) {
      for (int j = 4; j > 0; --j) {
        if (kind[i] >= j) {
          if (kind[i + 1] >= j && kind[i + 2] >= j) {
            kind[i] -= j;
            kind[i + 1] -= j;
            kind[i + 2] -= j;
            for (int k = 0; k < j; ++k) {
              hand.AddStraight(static_cast<Pai>(i));
            }
          } else {
            return false;
          }
        }
      }
    }
  }
  return true;
}

inline
std::uint32_t GetKokusiMusoWaitPais(std::uint32_t paiKindBits) {
  if (paiKindBits == RoleBits::OldHead) {
    return RoleBits::OldHead;
  }
  std::uint32_t mati = 0x0;
  if (!(paiKindBits & ~RoleBits::OldHead)) {
    const std::uint32_t find = (paiKindBits & RoleBits::OldHead) ^ RoleBits::OldHead;
    for (int i = 0; i < paiKindMax; ++i) {
      if (find & (1 << i)) {
        if (mati) {
          return 0x0;
        }
        mati = 1 << i;
      }
    }
  }
  return mati;
}

inline
std::uint32_t GetSevenDoubleWaitPais(const PaiKindArray &kind) {
  int mati = Pai::Invalid;
  int i = 0;
  for (const int size : kind) {
    if (size) {
      if (size > 2) {
        return 0x0;
      } else if (size == 1) {
        if (mati != Pai::Invalid) {
          return 0x0;
        }
        mati = i;
      }
    }
    ++i;
  }
  assert(mati != Pai::Invalid);
  return 1 << mati;
}

template <typename Erase, typename Wait>
std::uint32_t GetTenpai(Erase erase, Wait wait, Pai head, PaiKindArray kind, std::vector<TenpaiPatern> &tenpais) {
  TenpaiPatern hand;
  hand.SetHead(head);
  if (!erase(kind, hand)) {
    return 0x0;
  }
  if (boost::all_of(kind, [](int i) {return !i;})) {
    const auto waitBits = wait(hand);
    tenpais.push_back(hand);
    return waitBits;
  }
  return 0x0;
}

const auto EraseHighPriorityTriple = [](PaiKindArray &kind, TenpaiPatern &hand) {
  EraseTriple(kind, hand);
  return ErasedStraight(kind, hand);
};

const auto EraseHighPriorityStraight = [](PaiKindArray &kind, TenpaiPatern &hand) {
  if (!ErasedStraight(kind, hand)) {
    return false;
  }
  EraseTriple(kind, hand);
  return true;
};

const auto EraseStraightOnly = [](PaiKindArray &kind, TenpaiPatern &hand) {
  return ErasedStraight(kind, hand);
};

template <typename Erase>
std::uint32_t GetTenpaiSingle(Erase erase, Pai head, PaiKindArray &kind, std::vector<TenpaiPatern> &tenpais) {
  return GetTenpai(erase, [head](TenpaiPatern &hand) {
    hand.SetSingleWait(head);
    return 1 << head;
  }, head, kind, tenpais);
}

template <typename Erase>
std::uint32_t GetTenpaiBetween(Erase erase, Pai head, Pai wait, PaiKindArray &kind, std::vector<TenpaiPatern> &tenpais) {
  return GetTenpai(erase, [wait](TenpaiPatern &hand) {
    hand.SetBetweenWait(wait);
    return 1 << wait;
  }, head, kind, tenpais);
}

template <typename Erase>
std::uint32_t GetTenpaiSingleSide(Erase erase, Pai head, Pai wait, PaiKindArray &kind, std::vector<TenpaiPatern> &tenpais) {
  return GetTenpai(erase, [wait](TenpaiPatern &hand) {
    hand.SetSingleSideWait(wait);
    return 1 << wait;
  }, head, kind, tenpais);
}

template <typename Erase>
std::uint32_t GetTenpaiBothSide(Erase erase, Pai head, Pai wait, PaiKindArray &kind, std::vector<TenpaiPatern> &tenpais) {
  return GetTenpai(erase, [wait](TenpaiPatern &hand) {
    hand.SetBothSideWait(wait);
    return 1 << wait | 1 << (wait + 3);
  }, head, kind, tenpais);
}

template <typename Erase>
std::uint32_t GetTenpaiDoubleHead(Erase erase, Pai head, Pai wait, PaiKindArray &kind, std::vector<TenpaiPatern> &tenpais) {
  return GetTenpai(erase, [wait](TenpaiPatern &hand) {
    hand.SetDoubleHeadWait(wait);
    return 1 << wait;
  }, head, kind, tenpais);
}

inline
std::uint32_t GetEnableSevenDoubleWaitPai(PaiKindArray &kind, std::vector<TenpaiPatern> &tenpais) {
  std::uint32_t waitPaiBits = 0x0;
  for (int i = 0; i < paiKindMax; ++i) {
    if (kind[i] >= 2) {
      kind[i] -= 2;
      for (int j = Pai::P1; j < Pai::East; ++j) {
        if (IsSameColor(j, j + 2) && IsStraightEnablePai(j) && kind[j] && kind[j + 2]) {
          --kind[j];
          --kind[j + 2];
          waitPaiBits |= GetTenpaiBetween(EraseStraightOnly, static_cast<Pai>(i), static_cast<Pai>(j + 1), kind, tenpais);
          ++kind[j];
          ++kind[j + 2];
        }
        if (IsSameColor(j, j + 1) && IsStraightEnablePai(j) && kind[j] && kind[j + 1]) {
          --kind[j];
          --kind[j + 1];
          if (GetNumber(j) > 0 && GetNumber(j + 1) < 8) {
            waitPaiBits |= GetTenpaiBothSide(EraseStraightOnly, static_cast<Pai>(i), static_cast<Pai>(j - 1), kind, tenpais);
          } else {
            const Pai wait = !GetNumber(j) ? static_cast<Pai>(j + 2) : static_cast<Pai>(j - 1);
            waitPaiBits |= GetTenpaiSingleSide(EraseStraightOnly, static_cast<Pai>(i), wait, kind, tenpais);
          }
          ++kind[j];
          ++kind[j + 1];
        }
      }
      kind[i] += 2;
    }
  }
  return waitPaiBits;
}

inline
std::uint32_t GetHasTripleWaitPai(PaiKindArray &kind, std::vector<TenpaiPatern> &tenpais) {
  std::uint32_t waitPaiBits = 0x0;
  for (int i = 0; i < paiKindMax; ++i) {
    if (kind[i] >= 2) {
      kind[i] -= 2;
      for (int j = 0; j < paiKindMax; ++j) {
        if (IsStraightEnablePai(j) && IsSameColor(j, j + 2) && kind[j] && kind[j + 2]) {
          --kind[j];
          --kind[j + 2];
          waitPaiBits |= GetTenpaiBetween(EraseHighPriorityTriple, static_cast<Pai>(i), static_cast<Pai>(j + 1), kind, tenpais);
          waitPaiBits |= GetTenpaiBetween(EraseHighPriorityStraight, static_cast<Pai>(i), static_cast<Pai>(j + 1), kind, tenpais);
          ++kind[j];
          ++kind[j + 2];
        }
        if (IsStraightEnablePai(j) && IsSameColor(j, j + 1) && kind[j] && kind[j + 1]) {
          --kind[j];
          --kind[j + 1];
          if (GetNumber(j) > 0 && GetNumber(j + 1) < 8) {
            waitPaiBits |= GetTenpaiBothSide(EraseHighPriorityTriple, static_cast<Pai>(i), static_cast<Pai>(j - 1), kind, tenpais);
            waitPaiBits |= GetTenpaiBothSide(EraseHighPriorityStraight, static_cast<Pai>(i), static_cast<Pai>(j - 1), kind, tenpais);
          } else {
            const Pai wait = !GetNumber(j) ? static_cast<Pai>(j + 2) : static_cast<Pai>(j - 1);
            waitPaiBits |= GetTenpaiSingleSide(EraseHighPriorityTriple, static_cast<Pai>(i), wait, kind, tenpais);
            waitPaiBits |= GetTenpaiSingleSide(EraseHighPriorityStraight, static_cast<Pai>(i), wait, kind, tenpais);
          }
          ++kind[j];
          ++kind[j + 1];
        }
        if (kind[j] >= 2) {
          kind[j] -= 2;
          waitPaiBits |= GetTenpaiDoubleHead(EraseHighPriorityTriple, static_cast<Pai>(i), static_cast<Pai>(j), kind, tenpais);
          waitPaiBits |= GetTenpaiDoubleHead(EraseHighPriorityStraight, static_cast<Pai>(i), static_cast<Pai>(j), kind, tenpais);
          kind[j] += 2;
        }
      }
      kind[i] += 2;
    }
    if (kind[i] >= 1) {
      --kind[i];
      waitPaiBits |= GetTenpaiSingle(EraseHighPriorityTriple, static_cast<Pai>(i), kind, tenpais);
      waitPaiBits |= GetTenpaiSingle(EraseHighPriorityStraight, static_cast<Pai>(i), kind, tenpais);
      ++kind[i];
    }
  }
  return waitPaiBits;
}

inline
std::uint32_t GetNotHasTripleWaitPai(PaiKindArray &kind, std::vector<TenpaiPatern> &tenpais) {
  std::uint32_t waitPaiBits = 0x0;
  for (int i = 0; i < paiKindMax; ++i) {
    if (kind[i] >= 2) {
      kind[i] -= 2;
      for (int j = 0; j < paiKindMax; ++j) {
        if (IsStraightEnablePai(j) && IsSameColor(j, j + 2) && kind[j] && kind[j + 2]) {
          --kind[j];
          --kind[j + 2];
          waitPaiBits |= GetTenpaiBetween(EraseStraightOnly, static_cast<Pai>(i), static_cast<Pai>(j + 1), kind, tenpais);
          ++kind[j];
          ++kind[j + 2];
        }
        if (IsStraightEnablePai(j) && IsSameColor(j, j + 1) && kind[j] && kind[j + 1]) {
          --kind[j];
          --kind[j + 1];
          if (GetNumber(j) > 0 && GetNumber(j + 1) < 8) {
            waitPaiBits |= GetTenpaiBothSide(EraseStraightOnly, static_cast<Pai>(i), static_cast<Pai>(j - 1), kind, tenpais);
          } else {
            const Pai wait = !GetNumber(j) ? static_cast<Pai>(j + 2) : static_cast<Pai>(j - 1);
            waitPaiBits |= GetTenpaiSingleSide(EraseStraightOnly, static_cast<Pai>(i), wait, kind, tenpais);
          }
          ++kind[j];
          ++kind[j + 1];
        }
        if (kind[j] >= 2) {
          kind[j] -= 2;
          waitPaiBits |= GetTenpaiDoubleHead(EraseStraightOnly, static_cast<Pai>(i), static_cast<Pai>(j), kind, tenpais);
          kind[j] += 2;
        }
      }
      kind[i] += 2;
    }
    if (kind[i] >= 1) {
      --kind[i];
      waitPaiBits |= GetTenpaiSingle(EraseStraightOnly, static_cast<Pai>(i), kind, tenpais);
      ++kind[i];
    }
  }
  return waitPaiBits;
}

inline
WaitPair GetWaitPai(PaiKindArray &kind, const PaiKindArray &squeal, bool isHandCount13) {
  std::uint32_t waitPaiBits = 0x0;
  std::vector<TenpaiPatern> tenpaiPatern;
  bool hasTriple = false;
  std::uint32_t paiKindBits = 0x0;
  std::uint32_t enableWaitBits = ~static_cast<std::uint32_t>(0x0);
  for (int i = 0; i < paiKindMax; ++i) {
    if (kind[i]) {
      paiKindBits |= 1 << i;
      if (kind[i] >= 3) {
        hasTriple = true;
      }
    }
    if (kind[i] + squeal[i] == 4) {
      enableWaitBits &= ~(1 << i);
    }
  }
  if (isHandCount13) {
    if ((waitPaiBits = GetKokusiMusoWaitPais(paiKindBits))) {
      TenpaiPatern kokusi;
      kokusi.SetKokusiMuso(waitPaiBits);
      tenpaiPatern.push_back(kokusi); 
      return {waitPaiBits & enableWaitBits, tenpaiPatern};
    } else {
      waitPaiBits = GetSevenDoubleWaitPais(kind);
      if (waitPaiBits) {
        TenpaiPatern sevenDouble;
        sevenDouble.SetSevenDouble(waitPaiBits);
        tenpaiPatern.push_back(sevenDouble);
      }
    }
  }
  if (waitPaiBits) {
    waitPaiBits |= GetEnableSevenDoubleWaitPai(kind, tenpaiPatern);
  } else if (hasTriple) {
    waitPaiBits = GetHasTripleWaitPai(kind, tenpaiPatern);
  } else {
    waitPaiBits = GetNotHasTripleWaitPai(kind, tenpaiPatern);
  }
  return {waitPaiBits & enableWaitBits, tenpaiPatern};
}
}}

#endif
