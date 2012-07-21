#ifndef FEVERMJ_MODEL_HAND_HPP_
#define FEVERMJ_MODEL_HAND_HPP_
#include <cassert>
#include <array>
#include <vector>
#include "check_role.hpp"
#include "check_tenpai.hpp"
#include "field.hpp"
#include "pai.hpp"
#include "point.hpp"
#include "squeal.hpp"
#include "wind.hpp"
#include "../utility/algtorithm.hpp"
#include "../utility/log.hpp"

namespace FeverMJ { namespace Model {
class Hand {
 public:
  void Init(std::vector<Pai> &&firstPai) {
    tumo = Pai::Invalid;
    hand = std::move(firstPai);
    boost::sort(hand);
    kind = {};
    for (const Pai pai : hand) {
      ++kind[pai];
    }
    tenpais.clear();
    waitPais = 0x0;
  }

  void Tumo(Pai pai) {
    tumo = pai;
    ++kind[pai];
  }

  Point IsTumoGoal(Wind selfWind, const Squeal &squeal, const Field &field, PlayerState playerState) {
    assert(waitPais & (1 << tumo));
    std::vector<RoleHandState> states = GetStates(GetGoals(tumo), selfWind, tumo, true);
    playerState.SetTumoGoal();
    const auto squealRole = squeal.GetRoleState(field.GetDoraList(!playerState.IsSilentTenpai()));
    const std::uint32_t paiKindBits = GetPaiKaind() | squealRole.paiKindBits;
    RoleResult result = GetPreRole(selfWind, paiKindBits, squealRole, field, playerState);
    std::vector<Point> results;
    for (const auto &state : states) {
      RoleResult temp(result);
      if (!CheckKokusiMuso(state, temp)) {
        if (!CheckSevenDouble(state, temp)) {
          CheckRole(selfWind, paiKindBits, playerState, state, squealRole, temp);
        }
      }
      FEVERMJ_LOG("%d–|,–ð:%x\n", temp.hanCount, temp.roleBits);
      if (temp.hanCount >= 2) {
        if (!(temp.roleBits & Role::Peace)) {
          temp.huCount += 2;
        }
        if (temp.roleBits & Role::SevenDouble || temp.roleBits & Role::KokusiMuso) {
          temp.huCount = 25;
        } else {
          temp.huCount = RoundedUpHu(temp.huCount);
        }
        results.emplace_back(temp);
      }
    }
    return !results.empty() ? SelectPoint(results, playerState) : Point{};
  }

  Point IsRonGoal(Pai pai, Wind selfWind, const Squeal &squeal, const Field &field, PlayerState playerState) {
    assert(waitPais & (1 << pai));
    ++kind[pai];
    std::vector<RoleHandState> states = GetStates(GetGoals(pai), selfWind, pai, false);
    const auto squealRole = squeal.GetRoleState(field.GetDoraList(!playerState.IsSilentTenpai()));
    const std::uint32_t paiKindBits = GetPaiKaind() | squealRole.paiKindBits;
    RoleResult result = GetPreRole(selfWind, paiKindBits, squealRole, field, playerState);
    std::vector<Point> results;
    for (const auto &state : states) {
      RoleResult temp(result);
      if (!CheckKokusiMuso(state, temp)) {
        if (!CheckSevenDouble(state, temp)) {
          CheckRole(selfWind, paiKindBits, playerState, state, squealRole, temp);
        }
      }
      FEVERMJ_LOG("%d–|,–ð:%x\n", temp.hanCount, temp.roleBits);
      if (temp.hanCount >= 2) {
        if (playerState.IsMenzen()) {
          temp.huCount += 10;
        } else if (!(temp.roleBits & Role::Peace) && temp.huCount == 20) {
          temp.huCount = 30;
        }
        if (temp.roleBits & Role::SevenDouble || temp.roleBits & Role::KokusiMuso) {
          temp.huCount = 25;
        } else {
          temp.huCount = RoundedUpHu(temp.huCount);
        }
        results.emplace_back(temp);
      }
    }
    --kind[pai];
    return !results.empty() ? SelectPoint(results, playerState) : Point{};
  }

  Pai TumoCut() {
    const Pai pai = tumo;
    --kind[tumo];
    tumo = Pai::Invalid;
    return pai;
  }

  Pai HandCut(int i) {
    const Pai pai = hand[i];
    --kind[pai];
    hand.erase(std::next(hand.begin(), i));
    if (tumo != Pai::Invalid) {
      hand.insert(boost::lower_bound(hand, tumo), tumo);
      tumo = Pai::Invalid;
    }
    return pai;
  }

  void EraseNorth() {
    assert(kind[Pai::North]);
    --kind[Pai::North];
    if (tumo != Pai::North) {
      hand.erase(boost::equal_range(hand, Pai::North).first);
      hand.insert(boost::lower_bound(hand, tumo), tumo);
    }
    tumo = Pai::Invalid;
  }

  void Ti(const std::pair<Pai, Pai> &tiPair) {
    assert(tiPair.first != Pai::Invalid);
    assert(tiPair.second != Pai::Invalid);
    assert(tiPair.first < tiPair.second);
    --kind[tiPair.first];
    --kind[tiPair.second];
    hand.erase(boost::equal_range(hand, tiPair.first).first);
    hand.erase(boost::equal_range(hand, tiPair.second).first);
  }

  void Pon(Pai pai) {
    assert(pai != Pai::Invalid);
    assert(kind[pai] >= 2);
    kind[pai] -= 2;
    const auto pair = boost::equal_range(hand, pai);
    assert(pair.first != pair.second);
    hand.erase(pair.first, std::next(pair.first, 2));
  }

  void DarkKan(Pai pai) {
    assert(pai != Pai::Invalid);
    assert(kind[pai] == 4);
    kind[pai] -= 4;
    const auto pair = boost::equal_range(hand, pai);
    assert(pair.first != pair.second);
    hand.erase(pair.first, pair.second);
    if (pai != tumo) {
      hand.insert(boost::lower_bound(hand, tumo), tumo);
    }
    tumo = Pai::Invalid;
  }

  void AddKan(Pai pai) {
    assert(pai != Pai::Invalid);
    --kind[pai];
    if (pai != tumo) {
      hand.erase(boost::equal_range(hand, pai).first);
      hand.insert(boost::lower_bound(hand, tumo), tumo);
    }
    tumo = Pai::Invalid;
  }

  void LightKan(Pai pai) {
    assert(pai != Pai::Invalid);
    assert(kind[pai] == 3);
    kind[pai] -= 3;
    const auto pair = boost::equal_range(hand, pai);
    assert(pair.first != pair.second);
    hand.erase(pair.first, pair.second);
  }

  Pai GetTumo() const {
    return tumo;
  }

  Pai GetHand(int i) const {
    return hand[i];
  }

  int GetHandSize() const {
    return hand.size();
  }

  int GetPaiCount(Pai pai) const {
    assert(pai != Pai::Invalid);
    return kind[pai];
  }

  bool IsDarkKanEnable() const {
    for (const int size : kind) {
      if (size == 4) {
        return true;
      }
    }
    return false;
  }

  std::vector<TiPair> GetTiCandidate(Pai pai) const {
    assert(pai != Pai::Invalid);
    std::vector<std::pair<Pai, Pai>> tiCandidate;
    if (pai >= Pai::P1 && pai <= Pai::S9) {
      GetTiCandidateUpBothSideWait(tiCandidate, pai);
      GetTiCandidateDownBothSideWait(tiCandidate, pai);
      GetTiCandidateBetweenWait(tiCandidate, pai);
    }
    return tiCandidate;
  }

  void CheckTenpai() {
    waitPais = Model::GetWaitPai(kind, hand.size() == 13, tenpais);
    FEVERMJ_LOG("wait kind:%d\n", tenpais.size());
  }

  std::vector<std::vector<RoleHand>> GetReachPatern() const {
    const int size = hand.size();
    std::vector<std::vector<RoleHand>> patern;
    std::vector<RoleHand> t;
    Pai beforePai = Pai::Invalid;
    std::uint32_t beforeWait = 0;
    for (int i = 0; i < size; ++i) {
      if (hand[i] == beforePai && beforeWait) {
        patern.push_back(t);
      } else {
        t.clear();
        --kind[hand[i]];
        if ((beforeWait = Model::GetWaitPai(kind, size == 13, t))) {
          FEVERMJ_LOG("reach kind:%d\n", t.size());
          patern.push_back(t);
        } else {
          patern.push_back({});
        }
        ++kind[hand[i]];
      } 
    }
    t.clear();
    --kind[tumo];
    if ((beforeWait = Model::GetWaitPai(kind, size == 13, t))) {
      patern.push_back(t);
    } else {
      patern.push_back({});
    }
    ++kind[tumo];
    return patern;
  }
  
  std::uint32_t GetWaitPais() const {
    return waitPais;
  }

 private:
  void GetTiCandidateUpBothSideWait(std::vector<std::pair<Pai, Pai>> &tiCandidate, Pai pai) const {
    assert(pai != Pai::Invalid);
    assert(pai >= Pai::P1 && pai <= Pai::S9);
    if (GetNumber(pai) < 7 && kind[pai + 1] && kind[pai + 2]) {
      for (const Pai p : hand) {
        if (p != pai && p != pai + 1 && p != pai + 2) {
          if (pai < 6) {
            if (p != pai + 3) {
              tiCandidate.push_back({static_cast<Pai>(pai + 1), static_cast<Pai>(pai + 2)});
              break;
            }
          } else {
            tiCandidate.push_back({static_cast<Pai>(pai + 1), static_cast<Pai>(pai + 2)});
            break;
          }
        }
      }
    }
  }

  void GetTiCandidateDownBothSideWait(std::vector<std::pair<Pai, Pai>> &tiCandidate, Pai pai) const {
    assert(pai != Pai::Invalid);
    assert(pai >= Pai::P1 && pai <= Pai::S9);
    if (GetNumber(pai) > 1 && kind[pai -1] && kind[pai -2]) {
      for (const Pai p : hand) {
        if (p != pai && p != pai - 1 && p != pai - 2) {
          if (GetNumber(pai) > 2) {
            if (p != pai - 3) {
              tiCandidate.push_back({static_cast<Pai>(pai - 2), static_cast<Pai>(pai - 1)});
              break;
            }
          } else {
            tiCandidate.push_back({static_cast<Pai>(pai - 2), static_cast<Pai>(pai - 1)});
            break;
          }
        }
      }
    }
  }

  void GetTiCandidateBetweenWait(std::vector<std::pair<Pai, Pai>> &tiCandidate, Pai pai) const {
    assert(pai != Pai::Invalid);
    assert(pai >= Pai::P1 && pai <= Pai::S9);
    const int number = GetNumber(pai);
    if (number != 0 && number != 8 && kind[pai - 1] && kind[pai + 1]) {
      for (const Pai p : hand) {
        if (p != pai && p != pai - 1 && p != pai + 1) {
          tiCandidate.push_back({static_cast<Pai>(pai - 1), static_cast<Pai>(pai + 1)});
          break;
        }
      }
    }
  }

  std::vector<RoleHand> GetGoals(Pai pai) const {
    assert(pai != Pai::Invalid);
    std::vector<RoleHand> goals;
    boost::copy_if(tenpais, std::back_inserter(goals), [this, pai](const RoleHand &hand) {
      FEVERMJ_LOG("%x\n", hand.GetWaitPais() & (1 << pai));
      return hand.GetWaitPais() & (1 << pai);
    });
    return goals;
  }

  static std::vector<RoleHandState> GetStates(std::vector<RoleHand> &&goals, Wind selfWind, Pai pai, bool isTumo) {
    std::vector<RoleHandState> states;
    boost::transform(goals, std::back_inserter(states), [selfWind, isTumo, pai](RoleHand x) {
      return x.GetRoleState(isTumo, pai, selfWind);
    });
    return states;
  }

  std::uint32_t GetPaiKaind() const {
    std::uint32_t paiKindBits = 0x0;
    for (int i = 0; i < paiKindMax; ++i) {
      if (kind[i]) {
        paiKindBits |= 1 << i;
      }
    }
    return paiKindBits;
  }

  int GetDoraCount(const Field &field, const PlayerState &playerState) const {
    int doraCount = 0;
    // TODO:— ƒhƒ‰‚É‘Î‰ž‚·‚é
    for (const Pai dora : field.GetDoraList(!playerState.IsSilentTenpai())) {
      if (kind[dora]) {
        doraCount += kind[dora];
      }
    }
    return doraCount;
  }

  static Point SelectPoint(const std::vector<Point> &results, const PlayerState &playerState) {
    auto point = *boost::max_element(results, 
                                     [](const Point &lhs, const Point &rhs) {
                                       return lhs.GetHan() == rhs.GetHan() ?
                                              lhs.GetHu() < rhs.GetHu() : 
                                              lhs.GetBasicPoint() == rhs.GetBasicPoint() ?
                                              lhs.GetHan() < rhs.GetHan() : 
                                              lhs.GetBasicPoint() < rhs.GetBasicPoint();
                                     });
    point.CheckBounus(playerState);
    FEVERMJ_LOG("%d–|,–ð:%x\n", point.GetHan(), point.GetRole());
    return point;
  }

  RoleResult GetPreRole(Wind selfWind,
                        std::uint32_t paiKindBits,
                        const RoleSquealState &squealRole,
                        const Field &field,
                        const PlayerState &playerState) const {
    RoleResult result;
    result.huCount = 20 + squealRole.huCount;
    result.doraCount = GetDoraCount(field, playerState) + squealRole.doraCount;
    PreCheckRole(selfWind, playerState, paiKindBits, hand.size(), squealRole, field, result);
    CheckBitsRole(paiKindBits, kind, playerState, result);
    return result;
  }

  std::vector<Pai> hand;
  mutable std::array<int, paiKindMax> kind;
  Pai tumo;
  std::vector<RoleHand> tenpais;
  std::uint32_t waitPais;
};
}}

#endif
