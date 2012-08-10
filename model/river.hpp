#ifndef FEVERMJ_MODEL_REVER_HPP_
#define FEVERMJ_MODEL_REVER_HPP_
#include <cassert>
#include <vector>
#include <boost/optional.hpp>
#include "hand.hpp"
#include "pai.hpp"
#include "point.hpp"
#include "squeal.hpp"

namespace FeverMJ { namespace Model {
class River {
 public:
  void Init() {
    squealed.clear();
    river.clear();
    furiten.clear();
    isReachCall = false;
  }

  void Push(Pai pai) {
    river.push_back(isReachCall ? pai + squealOffset : pai);
  }

  void ReachPush(Pai pai) {
    river.push_back(pai + squealOffset);
    isReachCall = true;
  }

  void Pop() {
    squealed.push_back(static_cast<Pai>(river.back() % squealOffset));
    river.pop_back();
  }

  void Set() {
    isReachCall = false;
  }

  void SetFuritenPai(Pai pai) {
    furiten.push_back(pai);
  }

  bool IsFuriten(const Hand &hand) const {
    return boost::any_of(river, [&hand](int x) {
      return hand.IsWaitPai(static_cast<Pai>(x % squealOffset));
    }) |
    boost::any_of(squealed, [&hand](Pai x) {
      return hand.IsWaitPai(x);
    }) |
    boost::any_of(furiten, [&hand](Pai x) {
      return hand.IsWaitPai(x);
    });
  }

  bool IsFuriten(std::uint32_t wait, const Hand &hand) const {
    return boost::any_of(river, [wait, &hand](int pai) {
      return wait & (1 << (pai % squealOffset));
    }) |
    boost::any_of(squealed, [wait, &hand](Pai pai) {
      return wait & (1 << pai);
    });
  }

  boost::optional<Point> CheckLimitHandSink(const Field &field, const Squeal &squeal) const {
    if (squealed.empty()) {
      int doraCount = 0;
      const auto doraList = field.GetDoraList(false);
      for (const int pai : river) {
        if (IsTyuntyanPai(pai % squealOffset)) {
          return boost::none;
        }
        doraCount += boost::count(doraList, static_cast<Pai>(pai));
      }
      doraCount += squeal.GetNorthCount() * boost::count(doraList, Pai::North);
      return Point{RoleResult{doraCount}};
    }
    return boost::none;
  }

  void ClearFuriten() {
    furiten.clear();
  }

  int GetSize() const {
    return river.size();
  }

  int operator [](int i) const {
    return river[i];
  }

 private:
  std::vector<int> river;
  std::vector<Pai> squealed;
  std::vector<Pai> furiten;
  bool isReachCall;
};
}}

#endif
