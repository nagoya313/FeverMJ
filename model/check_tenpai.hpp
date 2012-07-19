#ifndef FEVERMJ_MODEL_CHECK_TENPAI_HPP_
#define FEVERMJ_MODEL_CHECK_TENPAI_HPP_
#include <cassert>
#include <cstdint>
#include <vector>
#include "pai.hpp"
#include "role.hpp"
#include "role_hand.hpp"
#include "../utility/algtorithm.hpp"

namespace FeverMJ { namespace Model {
inline
void EraseTriple(PaiKindArray &kind, RoleHand &hand) {
  for (int i = 0; i < paiKindMax; ++i) {
    if (kind[i] >= 3) {
      kind[i] -= 3;
      hand.AddTriple(static_cast<Pai>(i));
    }
  }
}

inline
void ForceEraseStraight(PaiKindArray &kind, RoleHand &hand) {
  for (int i = Pai::P1; i < Pai::S8; ++i) {
    if (kind[i] && GetNumber(i) < 7) {
      for (int j = 4; j > 0; --j) {
        if (kind[i] >= j) {
          if (kind[i + 1] >= j && kind[i + 2] >= j) {
            kind[i] -= j;
            kind[i + 1] -= j;
            kind[i + 2] -= j;
            for (int k  = 0; k < j; ++k) {
              hand.AddStraight(static_cast<Pai>(i));
            }
          } else {
            break;
          }
        }
      }
    }
  }
}

inline
void EraseStraight(PaiKindArray &kind, RoleHand &hand) {
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
          }
        }
      }
    }
  }
}

inline
void EraseReverseStraight(PaiKindArray &kind, RoleHand &hand) {
  for (int i = Pai::S9; i >= Pai::P3; --i) {
    if (kind[i] && GetNumber(i) > 1) {
      for (int j = 4; j > 0; --j) {
        if (kind[i] >= j) {
          if (kind[i - 1] >= j && kind[i - 2] >= j) {
            kind[i] -= j;
            kind[i - 1] -= j;
            kind[i - 2] -= j;
            for (int k = 0; k < j; ++k) {
              hand.AddStraight(static_cast<Pai>(i - 2));
            }
          }
        }
      }
    }
  }
}

inline
bool ErasedStraight(PaiKindArray &kind, RoleHand &hand) {
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
bool IsStraightEnablePai(Pai pai) {
  return pai >= Pai::P1 && pai <= Pai::S9;
}

inline
std::uint32_t GetStraightWaitPais(const std::array<Pai, 2> &waitPais, RoleHand &hand) {
  assert(waitPais[1] >= waitPais[0]);
  assert(waitPais[0] != Pai::Invalid);
  assert(waitPais[1] != Pai::Invalid);
  if (IsSameColor(waitPais[0], waitPais[1]) && IsStraightEnablePai(waitPais[0]) && IsStraightEnablePai(waitPais[1])) {
    if (waitPais[1] - waitPais[0] == 1) {
      if (GetNumber(waitPais[0]) > 0 && GetNumber(waitPais[1]) < 8) {
        hand.SetBothSideWait(static_cast<Pai>(waitPais[0] - 1));
        return 1 << (waitPais[0] - 1) | 1 << (waitPais[1] + 1);
      } else {
        hand.SetSingleSideWait(!GetNumber(waitPais[0]) ?
                               static_cast<Pai>(waitPais[0] + 2) :
                               static_cast<Pai>(waitPais[0] - 1));
        return 1 << (!GetNumber(waitPais[0]) ? waitPais[0] + 2 : waitPais[0] - 1);
      }
    } else if (waitPais[1] - waitPais[0] == 2) {
      hand.SetBetweenWait(static_cast<Pai>(waitPais[0] + 1));
      return 1 << (waitPais[0] + 1);
    }
  }
  return 0x0;
}

inline
std::uint32_t GetWaitPai(const PaiKindArray &kind, RoleHand &hand, std::vector<RoleHand> &tenpais) {
  std::array<Pai, 2> waitPais = {{Pai::Invalid, Pai::Invalid}};
  int count = 0;
  for (int i = 0; i < paiKindMax; ++i) {
    for (int j = 0; j < kind[i]; ++j) {
      waitPais[count++] = static_cast<Pai>(i);
      if (count > 2) {
        return 0x0;
      }
    }
  }
  assert(count == 2);
  if (const std::uint32_t straightWaitPais = GetStraightWaitPais(waitPais, hand)) {
    tenpais.push_back(hand);
    return straightWaitPais;
  }
  if (waitPais[0] == waitPais[1]) {
    hand.SetDoubleHeadWait(waitPais[0]);
    tenpais.push_back(hand);
    return 1 << waitPais[0];
  }
  return 0x0;
}

inline
std::uint32_t GetHighPriorityTripleForseEraseWaitPai(Pai head, PaiKindArray kind, std::vector<RoleHand> &tenpais) {
  RoleHand hand;
  hand.SetHead(head);
  EraseTriple(kind, hand);
  ForceEraseStraight(kind, hand);
  return GetWaitPai(kind, hand, tenpais);
}

inline
std::uint32_t GetHighPriorityStraightForseEraseWaitPai(Pai head, PaiKindArray kind, std::vector<RoleHand> &tenpais) {
  RoleHand hand;
  hand.SetHead(head);
  ForceEraseStraight(kind, hand);
  EraseTriple(kind, hand);
  return GetWaitPai(kind, hand, tenpais);
}

inline
std::uint32_t GetStraightOnlyForseEraseWaitPai(Pai head, PaiKindArray kind, std::vector<RoleHand> &tenpais) {
  RoleHand hand;
  hand.SetHead(head);
  ForceEraseStraight(kind, hand);
  return GetWaitPai(kind, hand, tenpais);
}

inline
std::uint32_t GetHighPriorityTripleWaitPai(Pai head, PaiKindArray kind, std::vector<RoleHand> &tenpais) {
  RoleHand hand;
  hand.SetHead(head);
  EraseTriple(kind, hand);
  EraseStraight(kind, hand);
  return GetWaitPai(kind, hand, tenpais);
}

inline
std::uint32_t GetHighPriorityStraightWaitPai(Pai head, PaiKindArray kind, std::vector<RoleHand> &tenpais) {
  RoleHand hand;
  hand.SetHead(head);
  EraseStraight(kind, hand);
  EraseTriple(kind, hand);
  return GetWaitPai(kind, hand, tenpais);
}

inline
std::uint32_t GetHighPriorityTripleReverseWaitPai(Pai head, PaiKindArray kind, std::vector<RoleHand> &tenpais) {
  RoleHand hand;
  hand.SetHead(head);
  EraseTriple(kind, hand);
  EraseReverseStraight(kind, hand);
  return GetWaitPai(kind, hand, tenpais);
}

inline
std::uint32_t GetHighPriorityStraightReverseWaitPai(Pai head, PaiKindArray kind, std::vector<RoleHand> &tenpais) {
  RoleHand hand;
  hand.SetHead(head);
  EraseReverseStraight(kind, hand);
  EraseTriple(kind, hand);
  return GetWaitPai(kind, hand, tenpais);
}

inline
std::uint32_t GetStraightOnlyWaitPai(Pai head, PaiKindArray kind, std::vector<RoleHand> &tenpais) {
  RoleHand hand;
  hand.SetHead(head);
  EraseStraight(kind, hand);
  return GetWaitPai(kind, hand, tenpais);
}

inline
std::uint32_t GetStraightOnlyReverseWaitPai(Pai head, PaiKindArray kind, std::vector<RoleHand> &tenpais) {
  RoleHand hand;
  hand.SetHead(head);
  EraseReverseStraight(kind, hand);
  return GetWaitPai(kind, hand, tenpais);
}

inline
std::uint32_t GetTenpaiHighPriorityTripleWithoutHead(Pai head, PaiKindArray kind, std::vector<RoleHand> &tenpais) {
  RoleHand hand;
  hand.SetHead(head);
  EraseTriple(kind, hand);
  if (!ErasedStraight(kind, hand)) {
    return 0x0;
  }
  if (boost::all_of(kind, [](int i) {return !i;})) {
    hand.SetSingleWait(head);
    tenpais.push_back(hand);
    return 1 << head;
  }
  return 0x0;
}

inline
std::uint32_t GetTenpaiHighPriorityStraightWithoutHead(Pai head, PaiKindArray kind, std::vector<RoleHand> &tenpais) {
  RoleHand hand;
  hand.SetHead(head);
  if (!ErasedStraight(kind, hand)) {
    return 0x0;
  }
  EraseTriple(kind, hand);
  if (boost::all_of(kind, [](int i) {return !i;})) {
    hand.SetSingleWait(head);
    tenpais.push_back(hand);
    return 1 << head;
  }
  return 0x0;
}

inline
std::uint32_t GetTenpaiStraightOnlyWithoutHead(Pai head, PaiKindArray kind, std::vector<RoleHand> &tenpais) {
  RoleHand hand;
  hand.SetHead(head);
  if (!ErasedStraight(kind, hand)) {
    return 0x0;
  } 
  if (boost::all_of(kind, [](int i) {return !i;})) {
    hand.SetSingleWait(head);
    tenpais.push_back(hand);
    return 1 << head;
  }
  return 0x0;
}

inline
std::uint32_t GetKokusiMusoWaitPais(std::uint32_t paiKindBits) {
  if (paiKindBits == RoleBits::OldHead) {
    return RoleBits::OldHead;
  }
  int mati = 0;
  if (!(paiKindBits &= ~RoleBits::OldHead)) {
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

inline
std::uint32_t GetEnableSevenDoubleWaitPai(PaiKindArray &kind, std::vector<RoleHand> &tenpais) {
  std::uint32_t waitPaiBits = 0x0;
  for (int i = 0; i < paiKindMax; ++i) {
    if (kind[i] >= 2) {
      kind[i] -= 2;
      waitPaiBits |= GetStraightOnlyWaitPai(static_cast<Pai>(i), kind, tenpais);
      waitPaiBits |= GetStraightOnlyReverseWaitPai(static_cast<Pai>(i), kind, tenpais);
      kind[i] += 2;
    }
  }
  return waitPaiBits;
}

inline
std::uint32_t GetHasTripleWaitPai(PaiKindArray &kind, std::vector<RoleHand> &tenpais) {
  std::uint32_t waitPaiBits = 0x0;
  for (int i = 0; i < paiKindMax; ++i) {
    if (kind[i] >= 2) {
      kind[i] -= 2;
      waitPaiBits |= GetHighPriorityTripleForseEraseWaitPai(static_cast<Pai>(i), kind, tenpais);
      waitPaiBits |= GetHighPriorityStraightForseEraseWaitPai(static_cast<Pai>(i), kind, tenpais);
      waitPaiBits |= GetHighPriorityTripleWaitPai(static_cast<Pai>(i), kind, tenpais);
      waitPaiBits |= GetHighPriorityStraightWaitPai(static_cast<Pai>(i), kind, tenpais);
      waitPaiBits |= GetHighPriorityTripleReverseWaitPai(static_cast<Pai>(i), kind, tenpais);
      waitPaiBits |= GetHighPriorityStraightReverseWaitPai(static_cast<Pai>(i), kind, tenpais);
      kind[i] += 2;
    }
    if (kind[i] >= 1) {
      --kind[i];
      waitPaiBits |= GetTenpaiHighPriorityTripleWithoutHead(static_cast<Pai>(i), kind, tenpais);
      waitPaiBits |= GetTenpaiHighPriorityStraightWithoutHead(static_cast<Pai>(i), kind, tenpais);
      ++kind[i];
    }
  }
  return waitPaiBits;
}

inline
std::uint32_t GetNotHasTripleWaitPai(PaiKindArray &kind, std::vector<RoleHand> &tenpais) {
  std::uint32_t waitPaiBits = 0x0;
  for (int i = 0; i < paiKindMax; ++i) {
    if (kind[i] >= 2) {
      kind[i] -= 2;
      waitPaiBits |= GetStraightOnlyForseEraseWaitPai(static_cast<Pai>(i), kind, tenpais);
      waitPaiBits |= GetStraightOnlyWaitPai(static_cast<Pai>(i), kind, tenpais);
      waitPaiBits |= GetStraightOnlyReverseWaitPai(static_cast<Pai>(i), kind, tenpais);
      kind[i] += 2;
    }
    if (kind[i] >= 1) {
      --kind[i];
      waitPaiBits |= GetTenpaiStraightOnlyWithoutHead(static_cast<Pai>(i), kind, tenpais);
      ++kind[i];
    }
  }
  return waitPaiBits;
}

inline
std::uint32_t GetWaitPai(PaiKindArray &kind, bool isHandCount13, std::vector<RoleHand> &tenpais) {
  std::uint32_t waitPaiBits = 0x0;
  tenpais.clear();
  bool hasTriple = false;
  std::uint32_t paiKindBits = 0x0;
  for (int i = 0; i < paiKindMax; ++i) {
    if (kind[i]) {
      paiKindBits |= 1 << i;
      if (kind[i] >= 3) {
        hasTriple = true;
      }
    }
  }
  if (isHandCount13) {  
    if ((waitPaiBits = GetKokusiMusoWaitPais(paiKindBits))) {
      RoleHand kokusi;
      kokusi.SetKokusiMuso(waitPaiBits);
      tenpais.push_back(kokusi);
      return waitPaiBits;
    } else {
      waitPaiBits = GetSevenDoubleWaitPais(kind);
      if (waitPaiBits) {
        RoleHand sevenDouble;
        sevenDouble.SetSevenDouble(waitPaiBits);
        tenpais.push_back(sevenDouble);
      }
    }
  }
  if (waitPaiBits) {
    return waitPaiBits | GetEnableSevenDoubleWaitPai(kind, tenpais);
  } else if (hasTriple) {
    return GetHasTripleWaitPai(kind, tenpais);
  } else {
    return GetNotHasTripleWaitPai(kind, tenpais);
  }
}
}}

#endif
