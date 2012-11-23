#ifndef FEVERMJ_UTILITY_OPERATORS_LESS_THAN_COMPARABLE_HPP_
#define FEVERMJ_UTILITY_OPERATORS_LESS_THAN_COMPARABLE_HPP_

namespace FeverMJ { namespace Utility { namespace Operators {
namespace LessThanComparable_ {
template <typename T>
class LessThanComparable {
  friend constexpr inline
  bool operator >(T lhs, T rhs) {return rhs < lhs;}
  friend constexpr inline
  bool operator >=(T lhs, T rhs) {return !(lhs < rhs);}
  friend constexpr inline
  bool operator <=(T lhs, T rhs) {return !(rhs < lhs);}
};
}

using LessThanComparable_::LessThanComparable;
}}}

#endif

