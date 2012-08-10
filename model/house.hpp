#ifndef FEVERMJ_MODEL_HOUSE_HPP_
#define FEVERMJ_MODEL_HOUSE_HPP_
#include <cassert>

namespace FeverMJ { namespace Model {
namespace House_ {  
enum House {
  Up,
  Self,
  Down
};
}

using House_::House;

inline
House GetUpHouse(House house) {
  return house == House::Up ? House::Down : house == House::Self ? House::Up : House::Self;
}

inline
House GetDownHouse(House house) {
  return house == House::Up ? House::Self : house == House::Self ? House::Down : House::Up;
}

inline
int GetHouseDistance(House begin, House house) {
  int i;
  for (i = 0; begin != house; ++i) {
    house = GetUpHouse(house);
  }
  return i;
}

inline
House GetOtherHouse(House house) {
  assert(house != House::Self);
  return house == House::Up ? House::Down : House::Up;
}
}}

#endif
