#ifndef FEVERMJ_MODEL_WIND_HPP_
#define FEVERMJ_MODEL_WIND_HPP_

namespace FeverMJ { namespace Model {
namespace Wind_ {
enum Wind {
  East,
  South,
  West
};
}

using Wind_::Wind;

inline
Wind GetNextWind(Wind wind) {
  return wind == Wind::East ?
         Wind::South : wind == Wind::South ?
                       Wind::West : Wind::East;
}

inline
Wind GetPrevWind(Wind wind) {
  return wind == Wind::East ?
         Wind::West : wind == Wind::South ?
                      Wind::East : Wind::South;
}
}}

#endif
