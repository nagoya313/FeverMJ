#ifndef FEVERMJ_MODEL_OPEN_REACH_HPP_
#define FEVERMJ_MODEL_OPEN_REACH_HPP_

namespace FeverMJ { namespace Model {

inline
PaiKindArray GetOneWaitHasSingle(std::uint32_t single) {
  for (int i = 0; i < paiKindMax; ++i) {
    if (single & (1 << i)) {
      PaiKindArray temp = {};
      temp[i] = 1;
      return temp;
    }
  }
  assert(false);
  return {};
}

inline
PaiKindArray GetOneWaitHasSingleSide(std::uint32_t singleSide) {
  for (int i = 0; i < paiKindMax; ++i) {
    if (singleSide & (1 << i)) {
      PaiKindArray temp = {};
      if (GetNumber(i) == 2) {
        temp[i - 1] = 1;
        temp[i - 2] = 1;
      } else {
        assert(GetNumber(i) == 6);
        temp[i + 1] = 1;
        temp[i + 2] = 1;
      }
      return temp;
    }
  }
  assert(false);
  return {};
}

inline
PaiKindArray GetOneWaitHasBetween(std::uint32_t between) {
  for (int i = 0; i < paiKindMax; ++i) {
    if (between & (1 << i)) {
      PaiKindArray temp = {};
      temp[i - 1] = 1;
      temp[i + 1] = 1;
      return temp;
    }
  }
  assert(false);
  return {};
}

inline
PaiKindArray GetOneWaitOpenReach(std::uint32_t single, std::uint32_t singleSide, std::uint32_t between) {
  if (single) {
    return GetOneWaitHasSingle(single);
  } else if (singleSide) {
    return GetOneWaitHasSingleSide(singleSide);
  } else if (between) {
    return GetOneWaitHasBetween(between);
  }
  assert(false);
  return {};
}

inline
PaiKindArray GetTwoWaitHasBothSide(std::uint32_t bothSide) {
  for (int i = 0; i < paiKindMax; ++i) {
    if (bothSide & (1 << i)) {
      PaiKindArray temp = {};
      temp[i + 1] = 1;
      temp[i + 2] = 1;
      return temp;
    }
  }
  assert(false);
  return {};
}

inline
PaiKindArray GetTwoWaitHasDoubleHead(std::uint32_t doubleHead) {
  for (int i = 0; i < paiKindMax; ++i) {
    if (doubleHead & (1 << i)) {
      PaiKindArray temp = {};
      temp[i] = 2;
    }
  }
  assert(false);
  return {};
}

inline
PaiKindArray GetTwoWaitHasSingle(std::uint32_t single) {
  for (int i = 0; i < paiKindMax; ++i) {
    if (single & (1 << i)) {
      PaiKindArray temp = {};
      temp[i] = 1;
      temp[i + 1] = 1;
      temp[i + 2] = 1;
      temp[i + 3] = 1;
      return temp;
    }
  }
  assert(false);
  return {};
}

inline
PaiKindArray GetTwoWaitHasBetween(std::uint32_t single, std::uint32_t between, const PaiKindArray &kind) {
  for (int i = 0; i < paiKindMax; ++i) {
    if (single & (1 << i)) {
      PaiKindArray temp = {};
      temp[i] = 1;
      if (GetNumber(i) > 1 && kind[i - 2] >= 3) {
        temp[i - 2] = 3;
      } else if (GetNumber(i) < 7 && kind[i + 2] >= 3) {
        temp[i + 2] = 3;
      } else if (GetNumber(i) > 4 && kind[i - 5] >= 3) {
        temp[i - 2] = 1;
        temp[i - 3] = 1;
        temp[i - 4] = 1;
        temp[i - 5] = 3;
      } else if (GetNumber(i) < 4 && kind[i + 5] >= 3) {
        temp[i + 2] = 1;
        temp[i + 3] = 1;
        temp[i + 4] = 1;
        temp[i + 5] = 3;
      }
      return temp;
    }
  }
  assert(false);
  return {};
}

inline
PaiKindArray GetTwoWaitOpenReach(std::uint32_t single,
                                 std::uint32_t doubleHead,
                                 std::uint32_t between,
                                 std::uint32_t bothSide,
                                 const PaiKindArray &kind) {
  if (bothSide) {
    return GetTwoWaitHasBothSide(bothSide);
  } else if (doubleHead) {
    return GetTwoWaitHasDoubleHead(doubleHead);
  } else if (between) {
    return GetTwoWaitHasBetween(single, between, kind);
  } else if (single) {
    return GetTwoWaitHasSingle(single);
  }
  assert(false);
  return {};
}

inline
void GetThreeWaitHasBothSideAndBetween(int i, std::uint32_t between, const PaiKindArray &kind, PaiKindArray &temp) {
  if (GetNumber(i) < 6 && (between & (1 << (i + 1)))) {
    if (kind[i + 2] == 4) {
      temp[i] = 2;
      temp[i + 2] = 4;
    } else if (kind[i + 5] >= 3) {
      temp[i] = 2;
      temp[i + 2] = 2;
      temp[i + 3] = 1;
      temp[i + 4] = 1;
      temp[i + 5] = 3;
    } else if (GetNumber(i) > 0 && kind[i - 1] >= 3) {
      temp[i - 1] = 3;
      temp[i] = 1;
      temp[i + 1] = 2;
      temp[i + 2] = 1;
    }
  } else if (GetNumber(i) < 5 && (between & (1 << (i + 2)))) {
    if (kind[i + 1] == 4) {
      temp[i + 1] = 4;
      temp[i + 3] = 2;
    } else if (kind[i - 2] >= 3) {
      temp[i - 2] = 3;
      temp[i - 1] = 1;
      temp[i] = 1;
      temp[i + 1] = 2;
      temp[i + 3] = 2;
    } else if (GetNumber(i) < 4 && kind[i + 4] >= 3) {
      temp[i + 1] = 1;
      temp[i + 2] = 2;
      temp[i + 3] = 1;
      temp[i + 4] = 3;
    }
  } 
}

inline
void GetThreeWaitHasBothSideAndSingle(int i, std::uint32_t single, const PaiKindArray &kind, PaiKindArray &temp) {
  if (GetNumber(i) < 8 && (single & (1 << (i + 1)))) {
    if (GetNumber(i) < 7 && kind[i + 2] >= 3) {
      temp[i + 2] = 3;
    }
  } else if (GetNumber(i) < 7 && (single & (1 << (i + 2)))) {
    if (kind[i + 1] >= 3) {
      temp[i + 1] = 3;
    }
  } else if (GetNumber(i) < 6 && (single & (1 << (i + 3)))) {
    temp[i + 3] = 2;
    temp[i + 4] = 1;
    temp[i + 5] = 1;
    temp[i + 6] = 1;
  } else if (GetNumber(i) > 2 && (single & (1 << (i - 3)))) {
    temp[i] = 2;
    temp[i - 1] = 1;
    temp[i - 2] = 1;
    temp[i - 3] = 1;
  } 
}

inline
void GetThreeWaitHasBothSideAndDoubleHead(int i, std::uint32_t doubleHead, const PaiKindArray &kind, PaiKindArray &temp) {
  if (doubleHead & (1 << i)) {
    if (kind[i] >= 3) {
      temp[i] = 3;
    } else {
      temp[i + 1] = 3;
      temp[i + 2] = 3;
    }
  } else if (GetNumber(i) < 6 && (doubleHead & (1 << (i + 3)))) {
    if (kind[i + 3] >= 3) {
      temp[i + 3] = 3;
    } else {
      temp[i + 1] = 3;
      temp[i + 2] = 3;
    }
  }
  for (int j = 0; j < paiKindMax; ++j) {
    if (doubleHead & (1 << j) && !temp[j]) {
      temp[j] = 2;
    }
  }
}

inline
PaiKindArray GetThreeWaitHasSingleSide(std::uint32_t singleSide) {
  for (int i = 0; i < paiKindMax; ++i) {
    PaiKindArray temp = {};
    temp[i] = 1;
    if (singleSide & (1 << i)) {
      if (GetNumber(i) == 2) {
        temp[i - 1] = 4;
        temp[i - 2] = 2;
      } else {
        assert(GetNumber(i) == 6);
        temp[i + 1] = 4;
        temp[i + 2] = 2;
      }
      return temp;
    }
  }
  assert(false);
  return {};
}

inline
PaiKindArray GetThreeWaitHasBothSide(std::uint32_t single,
                                     std::uint32_t doubleHead,
                                     std::uint32_t between,
                                     std::uint32_t bothSide,
                                     const PaiKindArray &kind) {
  PaiKindArray temp = {};
  for (int i = 0; i < paiKindMax; ++i) {
    if (bothSide & (1 << i)) {
      temp[i + 1] = 1;
      temp[i + 2] = 1;
      if (doubleHead) {
        GetThreeWaitHasBothSideAndDoubleHead(i, doubleHead, kind, temp);
      } else if (between) {
        GetThreeWaitHasBothSideAndBetween(i, between, kind, temp);
      } else if (single) {
        GetThreeWaitHasBothSideAndSingle(i, single, kind, temp);
      } else {
        temp[i + 3] = 1;
        temp[i + 4] = 1;
        temp[i + 5] = 1;
      }
      return temp;
    }
  }
  return temp;
}

inline
PaiKindArray GetThreeWaitHasSingle(std::uint32_t single,
                                   std::uint32_t doubleHead,
                                   std::uint32_t between,
                                   const PaiKindArray &kind) {
  PaiKindArray temp = {};
  for (int i = 0; i < paiKindMax; ++i) {
    if (single & (1 << i)) {
      const int number = GetNumber(i);
      temp[i] = 1;
      if (number > 1 && number < 7 && (between & (1 << (i - 1))) && (between & (1 << (i + 1)))) {
        temp[i - 2] = 3;
        temp[i + 2] = 3;
      } else if (number > 0 && number < 8 && (doubleHead & (1 << (i - 1))) && (doubleHead & (1 << (i + 1)))) {
        temp[i - 1] = 2;
        temp[i] = 3;
        temp[i + 1] = 2;
      } else {
        temp[i] = 1;
        temp[i + 1] = 1;
        temp[i + 2] = 1;
        temp[i + 3] = 1;
        if (GetNumber(i) > 4 && kind[i - 5] >= 3) {
          temp[i - 5] = 3;
          temp[i - 4] = 1;
          temp[i - 3] = 1;
          temp[i - 2] = 1;
        } else if (!GetNumber(i) && kind[i + 8] >= 3) {
          temp[i + 5] = 1;
          temp[i + 6] = 1;
          temp[i + 7] = 1;
          temp[i + 8] = 3;
        } else if (GetNumber(i) > 1 && kind[i - 2] >= 3) {
          temp[i - 2] = 3;
        } else if (GetNumber(i) < 4 && kind[i + 5] >= 3) {
          temp[i + 5] = 3;
        } else {
          temp[i + 4] = 1;
          temp[i + 5] = 1;
          temp[i + 6] = 1;
        }
      }
      return temp;
    }
  }
  return temp;
}

inline
PaiKindArray GetThreeWaitHasDoubleHead(std::uint32_t doubleHead) {
  PaiKindArray temp = {};
  for (int i = 0; i < paiKindMax; ++i) {
    if (doubleHead & (1 << i)) {
      temp[i] = 2;
      if (GetNumber(i) < 6 && doubleHead & (1 << (i + 3))) {
        temp[i + 1] = 2;
        temp[i + 2] = 2;
      }
    }
  }
  return temp;
}

inline
PaiKindArray GetThreeWaitOpenReach(std::uint32_t single,
                                   std::uint32_t doubleHead,
                                   std::uint32_t between,
                                   std::uint32_t singleSide,
                                   std::uint32_t bothSide,
                                   const PaiKindArray &kind) {
  if (singleSide && !doubleHead) {
    return GetThreeWaitHasSingleSide(singleSide);
  } else if (bothSide) {
    return GetThreeWaitHasBothSide(single, doubleHead, between, bothSide, kind);
  } else if (single) {
    return GetThreeWaitHasSingle(single, doubleHead, between, kind);
  } else if (doubleHead) {
    return GetThreeWaitHasDoubleHead(doubleHead);
  }
  assert(false);
  return {};
}

inline
PaiKindArray GetFourWaitOpenReach(std::uint32_t single,
                                  std::uint32_t doubleHead,
                                  std::uint32_t between,
                                  std::uint32_t singleSide,
                                  std::uint32_t bothSide,
                                  const PaiKindArray &kind) {
  PaiKindArray temp = {};
  for (int i = 0; i < paiKindMax; ++i) {
    if (singleSide & (1 << i)) {
      const int n = GetNumber(i);
      if (n == 2) {
        if (kind[i - 2] >= 3 && (single & (1 << (i - 1)))) {
          temp[i - 2] = 3;
          temp[i - 1] = 1;
          temp[i] = 1;
          temp[i + 1] = 1;
          temp[i + 2] = 1;
          return temp;
        }
      } else {
        if (kind[i + 2] >= 3 && (single & (1 << (i + 1)))) {
          temp[i + 2] = 3;
          temp[i + 1] = 1;
          temp[i] = 1;
          temp[i - 1] = 1;
          temp[i - 2] = 1;
          return temp;
        }
      }
    }
  }
  for (int i = 0; i < paiKindMax; ++i) {
    if (doubleHead & (1 << i)) {
      if ((bothSide & (1 << i)) && GetNumber(i) < 7 && kind[i + 1] == 4 && kind[i + 2] == 4) {
        temp[i + 1] = 1;
        temp[i + 2] = 1;
        return temp;
      } else {
        temp[i] = 2;
        if (GetNumber(i) < 7 && doubleHead & (1 << (i + 2)) && (single & (1 << (i + 1)))) {
          temp[i + 1] = 3;
          temp[i + 2] = 2;
          temp[i + 3] = 2;
          temp[i + 4] = 2;
          temp[i + 5] = 2;
          return temp;
        } else if (GetNumber(i) < 4 && (single & (1 << (i + 2))) && (single & (1 << (i + 5)))) {
          temp[i + 1] = 2;
          temp[i + 2] = 3;
          temp[i + 3] = 1;
          temp[i + 4] = 1;
          temp[i + 5] = 1;
          return temp;
        } else if (GetNumber(i) > 3 && (single & (1 << (i - 1))) && (single & (1 << (i - 4)))) {
          temp[i - 4] = 1;
          temp[i - 3] = 1;
          temp[i - 2] = 1;
          temp[i - 1] = 3;
          temp[i + 1] = 2;
          return temp;
        } else if (GetNumber(i) < 6 && doubleHead & (1 << (i + 3))) {
          temp[i + 1] = 2;
          temp[i + 2] = 2;
          if (GetNumber(i) < 4 && (doubleHead & (1 << (i + 5))) && (single & (1 << (i + 4)))) {
            temp[i + 3] = 2;
            temp[i + 4] = 3;
            temp[i + 5] = 2;
            return temp;
          }
        }
      }
    }
  }
  for (int i = 0; i < paiKindMax; ++i) {
    if (bothSide & (1 << i)) {
      temp[i + 1] = 1;
      temp[i + 2] = 1;
      if (GetNumber(i) < 3 && (bothSide & (1 << (i + 6)))) {
        temp[i + 3] = 1;
        temp[i + 5] = 1;
        if (doubleHead) {
          temp[i] = 1;
          temp[i + 4] = 1;
          temp[i + 6] = 1;
          for (int j = 0; j < paiKindMax; ++j) {
            if (doubleHead & (1 << j)) {
              temp[j] += 2;
            }
          }
        } else {
          if (kind[i + 2] >= 3) {
            temp[i + 2] = 3;
            temp[i + 4] = 1;
          } else {
            temp[i + 4] = 3;
          }
        }
      } else if (GetNumber(i) < 7 && (bothSide & (1 << (i + 2)))) {
        if (kind[i + 4] >= 3) {
          temp[i + 3] = 2;
          temp[i + 4] = 3;
        } else {
          temp[i + 1] = 3;
          temp[i + 2] = 2;
          temp[i + 3] = 1;
          temp[i + 4] = 1;
        }
      } else if (GetNumber(i) < 7 && (bothSide & (1 << (i + 1)))) {
        if (kind[i + 2] == 4) {
          if (kind[i + 1] >= 2) {
            temp[i + 1] = 2;
            temp[i + 3] = 1;
          } else {
            temp[i + 1] = 1;
            temp[i + 3] = 2;
          }
          temp[i + 2] = 4;
        }
      } else if (GetNumber(i) > 0 && (single & (1 << (i - 1)))) {
        temp[i - 1] = 1;
        temp[i] = 1;
        temp[i + 1] = 4;
        temp[i + 3] = 1;
      } else if (GetNumber(i) < 8 && (single & (1 << (i + 1)))) {
        if (kind[i + 2] == 4) {
          temp[i + 2] = 4;
          temp[i + 3] = 1;
          temp[i + 4] = 1;
          temp[i + 5] = 1;
        } else {
          temp[i + 2] = 3;
          if (GetNumber(i) < 7 &&  (doubleHead & (1 << (i + 2)))) {
            temp[i + 3] = 3;
          } else {
            temp[i + 3] = 1;
            temp[i + 4] = 1;
            temp[i + 5] = 1;
          }
        }
      } else if (GetNumber(i) < 4 && (single & (1 << (i + 5)))) {
        temp[i + 2] = 1;
        temp[i + 3] = 1;
        temp[i + 4] = 3;
        temp[i + 5] = 1;
      } else if (GetNumber(i) < 7 && (doubleHead & (1 << (i + 1)))) {
        if (kind[i] >= 3) {
          temp[i] = 3;
          if (single & (1 << (i + 2))) {
            temp[i + 1] = 3;
          } else {
            temp[i + 1] = 2;
          }
          temp[i + 2] = 2;
        } else if (GetNumber(i) > 2 && kind[i - 3] >= 3) { 
          temp[i - 3] = 3;
          temp[i - 2] = 1;
          temp[i - 1] = 1;
          temp[i] = 1;
          temp[i + 1] = 2;
          temp[i + 2] = 2;
        } else if (GetNumber(i) < 3 && kind[i + 6] >= 3) { 
          temp[i + 6] = 3;
          temp[i + 5] = 1;
          temp[i + 4] = 1;
          temp[i + 3] = 1;
          temp[i + 2] = 2;
          temp[i + 1] = 2;
        } else {
          temp[i + 1] = 2;
          temp[i + 2] = 2;
          temp[i + 3] = 3;
        }
      } else if (doubleHead & (1 << i)) {
        if (kind[i] >= 3) {
          temp[i] = 3;
        } else {
          temp[i + 1] = 3;
          temp[i + 2] = 3;
        }
        for (int j = 0; j < paiKindMax; ++j) {
          if (bothSide & (1 << j)) {
            temp[j + 1] = 1;
            temp[j + 2] = 1;
            if (doubleHead & (1 << j)) {
              if (kind[j] >= 3) {
                temp[j] = 3;
              } else {
                temp[j + 1] = 3;
                temp[j + 2] = 3;
              }
            } else if (GetNumber(j) < 6 && (doubleHead & (1 << (j + 3)))) {
              if (kind[j + 3] >= 3) {
                temp[j + 3] = 3;
              } else {
                temp[j + 1] = 3;
                temp[j + 2] = 3;
              }
            }
          }
        }
      } else if (GetNumber(i) < 6 && (doubleHead & (1 << (i + 3)))) {
        if (kind[i + 3] >= 3) {
          temp[i + 3] = 3;
        } else {
          temp[i + 1] = 3;
          temp[i + 2] = 3;
        }
        for (int j = 0; j < paiKindMax; ++j) {
          if (bothSide & (1 << j)) {
            temp[j + 1] = 1;
            temp[j + 2] = 1;
            if (doubleHead & (1 << j)) {
              if (kind[j] >= 3) {
                temp[j] = 3;
              } else {
                temp[j + 1] = 3;
                temp[j + 2] = 3;
              }
            } else if (GetNumber(j) < 6 && (doubleHead & (1 << (j + 3)))) {
              if (kind[j + 3] >= 3) {
                temp[j + 3] = 3;
              } else {
                temp[j + 1] = 3;
                temp[j + 2] = 3;
              }
            }
          }
        }
      }
      return temp;
    }
  }
  for (int i = 0; i < paiKindMax; ++i) {
    if (single & (1 << i)) {
      const int number = GetNumber(i);
      temp[i] = 1;
      if (number > 0 && (between & (1 << (i - 1)))) {
        if (number < 5 && (between & (1 << (i + 4)))) {
          temp[i - 2] = 3;
          temp[i + 1] = 1;
          temp[i + 2] = 1;
          temp[i + 3] = 1;
          temp[i + 5] = 3;
        } else {
          temp[i - 2] = 3;
          temp[i + 1] = 1;
          temp[i + 2] = 1;
          temp[i + 3] = 1;
          temp[i + 4] = 1;
          temp[i + 5] = 1;
          temp[i + 6] = 1;
        }
      } else if (!number && (between & (1 << (i + 7)))) {
        temp[i + 1] = 1;
        temp[i + 2] = 1;
        temp[i + 3] = 1;
        temp[i + 4] = 1;
        temp[i + 5] = 1;
        temp[i + 6] = 1;
        temp[i + 8] = 3;
      }
      return temp;
    }
  }
  for (int i = 0; i < paiKindMax; ++i) {
    if (doubleHead & (1 << i)) {
      temp[i] = 2;
      if (GetNumber(i) < 4 && doubleHead & (1 << (i + 1)) && doubleHead & (1 << (i + 3)) && doubleHead & (1 << (i + 4))) {
        temp[i + 2] = 2;
      }
    }
  }
  return temp;
}

inline
PaiKindArray GetSevenWaitOpenReach(std::uint32_t single, const PaiKindArray &kind) {
  for (int i = 0; i < paiKindMax; ++i) {
    if (GetNumber(i) == 1 && (single & (1 << i)) && kind[i - 1] == 3 && kind[i + 4] == 3) {
      PaiKindArray temp = {};
      temp[i - 1] = 3;
      temp[i] = 1;
      temp[i + 1] = 1;
      temp[i + 2] = 1;
      temp[i + 3] = 1;
      temp[i + 4] = 3;
      return temp;
    }
  }
  return {{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4}};
}

inline
PaiKindArray GetEightWaitOpenReach(std::uint32_t single) {
  for (int i = 0; i < paiKindMax; ++i) {
    if (single & (1 << i)) {
      PaiKindArray temp = {};
      temp[i - 1] = 3;
      temp[i] = 1;
      temp[i + 1] = 1;
      temp[i + 2] = 1;
      temp[i + 3] = 1;
      temp[i + 4] = 3;
      return temp;
    }
  }
  assert(false);
  return {};
}
}}

#endif
