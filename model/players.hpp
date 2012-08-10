#ifndef FEVERMJ_MODEL_PLAYERS_HPP_
#define FEVERMJ_MPDEL_PLAYERS_HPP_
#include <array>
#include <boost/optional.hpp>
#include "field.hpp"
#include "house.hpp"
#include "pai.hpp"
#include "player.hpp"
#include "river.hpp"
#include "wind.hpp"

namespace FeverMJ { namespace Model {
class Players {
 public:
  void GameStartInit(Field &field) {
    House house = field.GetParentHouse();
    for (int i = 0; i < 3; ++i) {
      players[house].GameStartInit(field.GetFirstPais(), static_cast<Wind>(Wind::East + i), house);
      house = GetDownHouse(house);
    }
  }

  void ContinueSetInit(Field &field) {
    House house = field.GetParentHouse();
    for (int i = 0; i < 3; ++i) {
      players[house].ContinueSetInit(field.GetFirstPais());
      house = GetDownHouse(house);
    }
  }

  void NextSetInit(Field &field) {
    House house = field.GetParentHouse();
    for (int i = 0; i < 3; ++i) {
      players[house].NextSetInit(field.GetFirstPais());
      house = GetDownHouse(house);
    }
  }

  bool HasFlyHouse() const {
    return boost::any_of(players, [](const Player &p) {
      return p.GetPoint() < 0;
    });
  }

  void SetTumoPoint(House goal, const Point &point, Field &field) {
    int getPoint = 0;
    for (House pay = GetDownHouse(goal); pay != goal; pay = GetDownHouse(pay)) {
      const auto payPoint = point.GetTumoPoint(goal, pay, field);
      players[pay].AddPoint(-payPoint);
      getPoint += payPoint;
    }
    players[goal].AddPoint(getPoint + field.ReleaseReachBar());
  }

  void SetRonPoint(House goal, House pay, const Point &point, Field &field) {
    const int getPoint = point.GetRonPoint(goal, pay, field);
    for (auto &p : players) {
      p.AddPoint(0);
    }
    players[goal].AddPoint(getPoint + field.ReleaseReachBar());
    players[pay].AddPoint(-getPoint);
  }

  bool IsParentTenpai(const Field &field) {
    return players[field.GetParentHouse()].IsTypeTenpai(field);
  }

  boost::optional<House> SetFlowPoint(Field &field, boost::optional<Point> &p) {
    if (CheckFlowFeverPoint(field)) {
      return boost::none;
    }
    if (const auto house = CheckLimitHandSink(field, p)) {
      return house;
    }
    CheckTenpai(field);
    return boost::none;
  }

  void SetFuritenPai(Pai pai) {
    for (auto &player : players) {
      player.SetFuritenPai(pai);
    }
  }

  Pai TumoCut(House house, bool isReach = false) {
    const auto pai = players[house].TumoCut(isReach);
    players[house].PushBackRiver(pai, isReach);
    return pai;
  }

  Pai HandCut(House house, int i, bool isReach = false) {
    const auto pai = players[house].HandCut(i, isReach);
    players[house].PushBackRiver(pai, isReach);
    return pai;
  }

  void AllDeleteFirst() {
    for (auto &player : players) {
      player.DeleteFirst();
    }
  }

  void Ti(House house, Pai pai, const TiPair &tiPair) {
    AllDeleteFirst();
    players[GetUpHouse(house)].PopBackRiver();
    players[house].Ti(pai, tiPair);
  }

  void Pon(House squeareHouse, House squearedHouse, Pai pai) {
    AllDeleteFirst();
    players[squearedHouse].PopBackRiver();
    players[squeareHouse].Pon(squearedHouse, pai);
  }

  void EraseNorthPon(House squeareHouse, House squearedHouse) {
    AllDeleteFirst();
    players[squearedHouse].RemoveNorth();
    players[squeareHouse].Pon(squearedHouse, Pai::North);
  }

  boost::optional<Point> LightKan(House squeareHouse, House squearedHouse, Pai pai, Field &field) {
    AllDeleteFirst();
    players[squearedHouse].PopBackRiver();
    return players[squeareHouse].LightKan(squearedHouse, pai, field);
  }

  boost::optional<Point> EraseNorthLightKan(House squeareHouse, House squearedHouse, Field &field) {
    AllDeleteFirst();
    players[squearedHouse].RemoveNorth();
    return players[squeareHouse].LightKan(squearedHouse, Pai::North, field);
  }

  bool IsSelfPonEnable(const Field &field, Pai pai) const {
    return !field.IsPaiEmpty() && !players[House::Self].IsReachTenpai() && players[House::Self].IsPonEnable(pai);
  }

  bool IsSelfLightKanEnable(const Field &field, Pai pai) const {
    return !field.IsPaiEmpty() && !players[House::Self].IsReachTenpai() && players[House::Self].IsLightKanEnable(pai);
  }

  std::vector<TiPair> GetSelfTiCandidate(const Field &field, House house, Pai pai) const {
    return !field.IsPaiEmpty() && !players[House::Self].IsReachTenpai() && house == House::Up ?
           players[House::Self].GetTiCandidate(pai) : std::vector<TiPair>{};
  }

  boost::optional<Point> CheckRon(Field &field, House goal, House pay, Pai pai, bool isTyankan = false) {
    std::uint32_t role = 0x0;
    if (isTyankan) {
      role |= Model::Role::Tyankan;
    } else if (players[goal].IsOpen() && !players[pay].IsReachTenpai()) {
      role |= Model::Role::OpenRon;
    }
    return players[goal].Ron(role, pai, field);
  }

  boost::optional<House> GetFeverHouse() const {
    const auto it = boost::find_if(players, [](const Player &p) {return p.IsFever();});
    return it != players.end() ? boost::make_optional(static_cast<House>(std::distance(players.begin(), it))) : boost::none;
  }

  const Player &operator [](House house) const {
    return players[house];
  }

  Player &operator [](House house) {
    return players[house];
  }

 private:
  bool CheckFlowFeverPoint(Field &field) {
    for (int i = 0; i < 3; ++i) {
      if (players[i].IsFever()) {
        players[i].AddPoint(6000 + field.ReleaseReachBar());
        players[GetUpHouse(static_cast<House>(i))].AddPoint(-3000);
        players[GetDownHouse(static_cast<House>(i))].AddPoint(-3000);
        players[i].SetFlow(false, field);
        players[GetUpHouse(static_cast<House>(i))].SetFlow(true, field);
        players[GetDownHouse(static_cast<House>(i))].SetFlow(true, field);
        return true;
      }
    }
    return false;
  }

  boost::optional<House> CheckLimitHandSink(Field &field, boost::optional<Point> &p) {
    House parent = field.GetParentHouse();
    for (int i = 0; i < 3; ++i) {
      if (const auto point = players[parent].CheckLimitHandSink(field)) {
        p = *point;
        SetTumoPoint(parent, *point, field);
        players[parent].SetFlow(true, field);
        players[GetUpHouse(parent)].SetFlow(true, field);
        players[GetDownHouse(parent)].SetFlow(true, field);
        return parent;
      }
      parent = GetDownHouse(parent);
    }
    return boost::none;
  }

  void CheckTenpai(const Field &field) {
    const int tenpaiCount = boost::count_if(players, [&field](Player &p) {
      return p.IsTypeTenpai(field);
    });
    if (tenpaiCount == 1 || tenpaiCount == 2) {
      for (auto &player : players) {
        if (player.IsTypeTenpai(field)) {
          player.AddPoint(tenpaiCount == 1 ? 6000 : 3000);
        } else {
          player.AddPoint(tenpaiCount == 1 ? -3000 : -6000);
        }
        player.SetFlow(false, field);
      }
    } else {
      for (auto &player : players) {
        player.AddPoint(0);
        player.SetFlow(false, field);
      }
    }
  }

  std::array<Player, 3> players;
  std::array<River, 3> rivers;
};
}}

#endif
