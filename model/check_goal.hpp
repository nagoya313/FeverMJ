#ifndef FEVERMJ_MODEL_CHECK_GOAL_HPP_
#define FEVERMJ_MODEL_CHECK_GOAL_HPP_
#include <cassert>
#include <iterator>
#include <vector>
#include <boost/optional.hpp>
#include "check_role.hpp"
#include "field.hpp"
#include "hand.hpp"
#include "hand_role.hpp"
#include "hand_common_role.hpp"
#include "pai.hpp"
#include "player_state.hpp"
#include "point.hpp"
#include "role.hpp"
#include "role_result.hpp"
#include "role_temp.hpp"
#include "squeal_role.hpp"
#include "tenpai_patern.hpp"
#include "wind.hpp"
#include "../utility/algorithm.hpp"
#include "../utility/log.hpp"

namespace FeverMJ { namespace Model {
inline
std::vector<TenpaiPatern> GetGoalPatern(Pai pai, const std::vector<TenpaiPatern> &tenpais) {
  std::vector<TenpaiPatern> goals;
  boost::copy_if(tenpais, std::back_inserter(goals), [pai](const TenpaiPatern &hand) {
    FEVERMJ_LOG("%x\n", hand.IsGoalEnable(pai));
    return hand.IsGoalEnable(pai);
  });
  return goals;
}

inline
std::vector<HandRole> GetHandRoles(std::vector<TenpaiPatern> &&goals, Wind selfWind, Pai pai, bool isTumo) {
  std::vector<HandRole> states;
  boost::transform(goals, std::back_inserter(states), [selfWind, isTumo, pai](TenpaiPatern x) {
    return x.GetHandRole(isTumo, pai, selfWind);
  });
  return states;
}

inline
Point SelectPoint(const std::vector<Point> &results, const PlayerState &playerState) {
  auto point = *boost::max_element(results, 
                                   [](const Point &lhs, const Point &rhs) {
                                     return lhs.GetHan() == rhs.GetHan() ?
                                            lhs.GetHu() < rhs.GetHu() : 
                                            lhs.GetBasicPoint() == rhs.GetBasicPoint() ?
                                            lhs.GetHan() < rhs.GetHan() : 
                                            lhs.GetBasicPoint() < rhs.GetBasicPoint();
                                   });
  point.AddBounus(playerState);
  FEVERMJ_LOG("%d–|,–ð:%x\n", point.GetHan(), point.GetRole());
  return point;
}

inline
RoleResult GetPreRole(const PaiKindArray &kind, const HandCommonRole &hand, const SquealRole &squeal) {
  RoleResult result{hand, squeal};
  PreCheckRole(hand, squeal, result);
  CheckBitsRole(kind, hand, squeal, result);
  return result;
}

inline
RoleResult GetRole(const HandCommonRole &hand, const HandRole &state, const SquealRole &squeal, const RoleResult &preRole) {
  RoleResult temp{preRole};
  if (!CheckKokusiMuso(state, temp)) {
    if (!CheckSevenDouble(state, temp)) {
      CheckRole(RoleTemp{hand, state, squeal}, temp);
    }
  }
  return temp;
}

inline
boost::optional<Point> IsTumoGoal(const Hand &hand, const Squeal &squeal, const Field &field, const PlayerState &playerState) {
  const auto wait = hand.GetWait();
  const auto kind = hand.GetPaiKindArray();
  std::vector<HandRole> states = GetHandRoles(GetGoalPatern(*hand.GetTumo(), wait.tenpaiPatern),
                                              playerState.GetSelfWind(),
                                              *hand.GetTumo(),
                                              true);
  const auto squealRole = squeal.GetSquealRole(field.GetDoraList(playerState.IsReachTenpai()));
  HandCommonRole common{0x0, playerState.GetSelfWind(), true, hand.GetHandSize(), kind, field, playerState};
  RoleResult result = GetPreRole(kind, common, squealRole);
  std::vector<Point> results;
  for (const auto &state : states) {
    RoleResult temp = GetRole(common, state, squealRole, result);
    FEVERMJ_LOG("%d–|,–ð:%x\n", temp.GetHan(), temp.GetRoleBits());
    if (temp.GetHan() >= 2) {
      if (!(temp.HasRole(Role::Peace))) {
        temp.AddHu(2);
      }
      if (temp.HasRole(Role::SevenDouble) || temp.HasRole(Role::KokusiMuso)) {
        temp.SetHu(25);
      } else {
        temp.RoundedUpHu();
      }
      results.emplace_back(temp);
    }
  }
  return !results.empty() ? boost::make_optional(SelectPoint(results, playerState)) : boost::none;
}

inline
boost::optional<Point> IsRonGoal(std::uint32_t role, 
                                 Pai pai,
                                 const Hand &hand,
                                 const Squeal &squeal,
                                 const Field &field,
                                 const PlayerState &playerState) {
  const auto wait = hand.GetWait();
  auto kind = hand.GetPaiKindArray();
  ++kind[pai];
  std::vector<HandRole> states = GetHandRoles(GetGoalPatern(pai, wait.tenpaiPatern), playerState.GetSelfWind(), pai, false);
  const auto squealRole = squeal.GetSquealRole(field.GetDoraList(playerState.IsReachTenpai()));
  HandCommonRole common{role, playerState.GetSelfWind(), false, hand.GetHandSize(), kind, field, playerState};
  RoleResult result = GetPreRole(kind, common, squealRole);
  std::vector<Point> results;
  for (const auto &state : states) {
    RoleResult temp = GetRole(common, state, squealRole, result);
    FEVERMJ_LOG("%d–|,–ð:%x\n", temp.GetHan(), temp.GetRoleBits());
    if (temp.GetHan() >= 2) {
      if (common.IsMenzen()) {
        temp.AddHu(10);
      } else if (temp.GetHu() == 20) {
        temp.SetHu(30);
      }
      if (temp.HasRole(Role::SevenDouble) || temp.HasRole(Role::KokusiMuso)) {
        temp.SetHu(25);
      } else {
        temp.RoundedUpHu();
      }
      results.emplace_back(temp);
    }
  }
  return !results.empty() ? boost::make_optional(SelectPoint(results, playerState)) : boost::none;
}
}}

#endif
