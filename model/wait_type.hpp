#ifndef FEVERMJ_MODEL_WAIT_TYPE_HPP_
#define FEVERMJ_MODEL_WAIT_TYPE_HPP_
#include <cstdint>

namespace FeverMJ { namespace Model {
namespace WaitType_ {
enum WaitType : std::uint32_t {
  NotTenpai = 0,
  Single = 1 << 0,
  BothSide = 1 << 1,
  Between = 1 << 2,
  SingleSide = 1 << 3,
  DoubleHead = 1 << 4,
  Rising = 1 << 5
};
}

using WaitType_::WaitType;

namespace RoleType_ {
enum RoleType : std::uint32_t {
  Normal = 0,
  SevenDouble = 1,
  KokusiMuso = 2
};
}

using RoleType_::RoleType;
}}

#endif
