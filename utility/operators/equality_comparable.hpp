#ifndef FEVERMJ_UTILITY_OPERATORS_EQUALITY_COMPARABLE_HPP_
#define FEVERMJ_UTILITY_OPERATORS_EQUALITY_COMPARABLE_HPP_

namespace FeverMJ { namespace Utility { namespace Operators {
namespace EqualityComparable_ {
template <typename T>
class EqualityComparable {
  friend constexpr inline
  bool operator !=(T lhs, T rhs) {return !(lhs == rhs);}
};
}

using EqualityComparable_::EqualityComparable;
}}}

#endif

