#ifndef FEVERMJ_MODEL_PAI_DORA_HPP_
#define FEVERMJ_MODEL_PAI_DORA_HPP_
#include <array>
#include "pai.hpp"

namespace FeverMJ { namespace Model { namespace Pai {
constexpr std::array<Pai, Pai::max> dorahyoToDoraRange = {{
  Pai::Manzu9(), Pai::Manzu1(), 
  Pai::Pinzu2(), Pai::Pinzu3(), Pai::Pinzu4(), 
  Pai::Pinzu5(), Pai::Pinzu6(), Pai::Pinzu7(),
  Pai::Pinzu8(), Pai::Pinzu9(), Pai::Pinzu1(),
  Pai::Sozu2(), Pai::Sozu3(), Pai::Sozu4(), 
  Pai::Sozu5(), Pai::Sozu6(), Pai::Sozu7(),
  Pai::Sozu8(), Pai::Sozu9(), Pai::Sozu1(),
  Pai::Nan(), Pai::Sya(), Pai::Pe(), Pai::Ton(),
  Pai::Hatsu(), Pai::Chun(), Pai::Haku()
}};

constexpr Pai DorahyoToDora(Pai dorahyo) {
  return dorahyoToDoraRange[dorahyo.ID()];
}
}}}

#endif
