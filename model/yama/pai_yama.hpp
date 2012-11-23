#ifndef FEVERMJ_MODEL_YAMA_PAI_YAMA_HPP_
#define FEVERMJ_MODEL_YAMA_PAI_YAMA_HPP_
#include <cstddef>
#include <array>
#include <boost/assert.hpp>
#include "../pai/pai.hpp"

namespace FeverMJ { namespace Model { namespace Yama {
class PaiYama {
  static constexpr std::size_t paiKindNum = 27;
  static constexpr std::size_t paiNum = 4;
 public:
  static constexpr std::size_t size = paiKindNum * paiNum;
  
  void Shuffle() {
  }
  
  Pai::Pai At(std::size_t index) const {
    BOOST_ASSERT(index < yama.size());
    return yama[index];
  }
  
 private:
  std::array<Pai::Pai, paiKindNum * paiNum> yama = {{
    Pai::Pai::Manzu1(), Pai::Pai::Manzu1(), Pai::Pai::Manzu1(), Pai::Pai::Manzu1(),
    Pai::Pai::Manzu9(), Pai::Pai::Manzu9(), Pai::Pai::Manzu9(), Pai::Pai::Manzu9(),
    Pai::Pai::Pinzu1(), Pai::Pai::Pinzu1(), Pai::Pai::Pinzu1(), Pai::Pai::Pinzu1(),
    Pai::Pai::Pinzu2(), Pai::Pai::Pinzu2(), Pai::Pai::Pinzu2(), Pai::Pai::Pinzu2(),
    Pai::Pai::Pinzu3(), Pai::Pai::Pinzu3(), Pai::Pai::Pinzu3(), Pai::Pai::Pinzu3(),
    Pai::Pai::Pinzu4(), Pai::Pai::Pinzu4(), Pai::Pai::Pinzu4(), Pai::Pai::Pinzu4(),
    Pai::Pai::Pinzu5(), Pai::Pai::Pinzu5(), Pai::Pai::Pinzu5(), Pai::Pai::Pinzu5(),
    Pai::Pai::Pinzu6(), Pai::Pai::Pinzu6(), Pai::Pai::Pinzu6(), Pai::Pai::Pinzu6(),
    Pai::Pai::Pinzu7(), Pai::Pai::Pinzu7(), Pai::Pai::Pinzu7(), Pai::Pai::Pinzu7(),
    Pai::Pai::Pinzu8(), Pai::Pai::Pinzu8(), Pai::Pai::Pinzu8(), Pai::Pai::Pinzu8(),
    Pai::Pai::Pinzu9(), Pai::Pai::Pinzu9(), Pai::Pai::Pinzu9(), Pai::Pai::Pinzu9(),
    Pai::Pai::Sozu1(), Pai::Pai::Sozu1(), Pai::Pai::Sozu1(), Pai::Pai::Sozu1(),
    Pai::Pai::Sozu2(), Pai::Pai::Sozu2(), Pai::Pai::Sozu2(), Pai::Pai::Sozu2(),
    Pai::Pai::Sozu3(), Pai::Pai::Sozu3(), Pai::Pai::Sozu3(), Pai::Pai::Sozu3(),
    Pai::Pai::Sozu4(), Pai::Pai::Sozu4(), Pai::Pai::Sozu4(), Pai::Pai::Sozu4(),
    Pai::Pai::Sozu5(), Pai::Pai::Sozu5(), Pai::Pai::Sozu5(), Pai::Pai::Sozu5(),
    Pai::Pai::Sozu6(), Pai::Pai::Sozu6(), Pai::Pai::Sozu6(), Pai::Pai::Sozu6(),
    Pai::Pai::Sozu7(), Pai::Pai::Sozu7(), Pai::Pai::Sozu7(), Pai::Pai::Sozu7(),
    Pai::Pai::Sozu8(), Pai::Pai::Sozu8(), Pai::Pai::Sozu8(), Pai::Pai::Sozu8(),
    Pai::Pai::Sozu9(), Pai::Pai::Sozu9(), Pai::Pai::Sozu9(), Pai::Pai::Sozu9(),
    Pai::Pai::Ton(), Pai::Pai::Ton(), Pai::Pai::Ton(), Pai::Pai::Ton(),
    Pai::Pai::Nan(), Pai::Pai::Nan(), Pai::Pai::Nan(), Pai::Pai::Nan(),
    Pai::Pai::Sya(), Pai::Pai::Sya(), Pai::Pai::Sya(), Pai::Pai::Sya(),
    Pai::Pai::Pe(), Pai::Pai::Pe(), Pai::Pai::Pe(), Pai::Pai::Pe(),
    Pai::Pai::Haku(), Pai::Pai::Haku(), Pai::Pai::Haku(), Pai::Pai::Haku(),
    Pai::Pai::Hatsu(), Pai::Pai::Hatsu(), Pai::Pai::Hatsu(), Pai::Pai::Hatsu(),
    Pai::Pai::Chun(), Pai::Pai::Chun(), Pai::Pai::Chun(), Pai::Pai::Chun()
  }};
};
}}}

#endif
