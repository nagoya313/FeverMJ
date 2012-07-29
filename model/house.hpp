#ifndef FEVERMJ_MODEL_HOUSE_HPP_
#define FEVERMJ_MODEL_HOUSE_HPP_

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
}}

#endif
