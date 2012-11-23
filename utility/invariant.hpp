#ifndef FEVERMJ_UTILITY_INVARIANT_HPP_
#define FEVERMJ_UTILITY_INVARIANT_HPP_
#include <type_traits>

namespace FeverMJ { namespace Utility {
template <typename T>
struct Invariant {
  explicit Invariant(const T &s) : self{s} {s.Invariant();}
  ~Invariant() {s.Invariant();}
 private:
  const T &self;
};
}}

#ifndef NDEBUG
#define FEVERMJ_CONSTRUCT_INVARIANT() Invariant();
#define FEVERMJ_METHOD_INVARIANT()\
  const ::FeverMJ::Utility::Invariant<std::decay<decltype(this)>::type>\
    invariant{this};
#else
#define FEVERMJ_CONSTRUCT_INVARIANT()
#define FEVERMJ_METHOD_INVARIANT() 
#endif

#endif

