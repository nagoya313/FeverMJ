#ifndef FEVERMJ_MODEL_PLAYERS_HPP_
#define FEVERMJ_MPDEL_PLAYERS_HPP_
#include <array>
#include <boost/optional.hpp>
#include "field.hpp"
#include "house.hpp"
#include "pai.hpp"
#include "player.hpp"
#include "wind.hpp"

namespace FeverMJ { namespace Model {
class Players {
 public:
  void GameStartInit(Field &field) {
    players[House::Up].GameStartInit(field.GetFirstPais(), GetStartWind(field, Wind::East));
    players[House::Self].GameStartInit(//{Pai::M1, Pai::M1, Pai::M1, Pai::M9, Pai::M9, Pai::M9, Pai::P1, Pai::P1, Pai::P5, Pai::P5, Pai::West, Pai::West, Pai::West},
                                       //{Pai::P1, Pai::P2, Pai::P3, Pai::P4, Pai::P5, Pai::P6, Pai::S1, Pai::S1, Pai::S4, Pai::S5, Pai::S6, Pai::S7, Pai::S8},                                        
                                       //{Pai::M1, Pai::M1, Pai::M1, Pai::P1, Pai::P2, Pai::P3, Pai::P5, Pai::P5, Pai::P5, Pai::P6, Pai::P7, Pai::South, Pai::South},
                                       //{Pai::P7, Pai::P8, Pai::P9, Pai::P9, Pai::P9, Pai::S5, Pai::S5, Pai::East, Pai::East, Pai::East, Pai::South, Pai::South, Pai::South},
                                       //{Pai::P7, Pai::P8, Pai::P9, Pai::S1, Pai::S2, Pai::S3, Pai::S5, Pai::S6, Pai::S7, Pai::S9, Pai::West Pai::West, Pai::West},
                                       //{Pai::P1, Pai::P1, Pai::P2, Pai::P3, Pai::P4, Pai::P5, Pai::P5, Pai::P6, Pai::P7, Pai::P9, Pai::West Pai::West, Pai::West},
                                       //{Pai::P1, Pai::P1, Pai::P2, Pai::P3, Pai::P4, Pai::White, Pai::White, Pai::White, Pai::From, Pai::From, Pai::Center, Pai::Center, Pai::Center},
                                       //{Pai::P2, Pai::P3, Pai::P4, Pai::P5, Pai::P5, Pai::P6, Pai::P6, Pai::P7, Pai::S3, Pai::S4, Pai::S5, Pai::S8, Pai::S8},
                                       //{Pai::P2, Pai::P3, Pai::P4, Pai::P5, Pai::P6, Pai::P7, Pai::P7, Pai::P9, Pai::S3, Pai::S4, Pai::S5, Pai::S8, Pai::S8},
                                       //{Pai::P2, Pai::P3, Pai::P4, Pai::P7, Pai::P8, Pai::S1, Pai::S2, Pai::S3, Pai::S3, Pai::S3, Pai::S6, Pai::S7, Pai::S8},
                                       //{Pai::P3, Pai::P3, Pai::P4, Pai::P4, Pai::P5, Pai::P5, Pai::P6, Pai::P6, Pai::P7, Pai::P7, Pai::P8, Pai::East, Pai::East},
                                       //{Pai::P3, Pai::P3, Pai::P4, Pai::P4, Pai::P5, Pai::P5, Pai::P6, Pai::P6, Pai::P7, Pai::P8, Pai::P8, Pai::East, Pai::East},
                                       //{Pai::P1, Pai::P1, Pai::P1, Pai::P2, Pai::P2, Pai::P3, Pai::P3, Pai::P3, Pai::P7, Pai::P8, Pai::P9, Pai::East, Pai::East},
                                       //{Pai::M9, Pai::M9, Pai::S1, Pai::S1, Pai::S1, Pai::S2, Pai::S3, Pai::S4, Pai::S4, Pai::S5, Pai::S6, Pai::S7, Pai::S8},
                                       //{Pai::M1, Pai::M1, Pai::M9, Pai::P1, Pai::S1, Pai::S9, Pai::East, Pai::South, Pai::West, Pai::North, Pai::North, Pai::From, Pai::Center},
                                       //{Pai::M1, Pai::M1, Pai::P3, Pai::P4, Pai::P5, Pai::P5, Pai::P6, Pai::P7, Pai::P8, Pai::P9, Pai::S3, Pai::S4, Pai::S5},
                                       //{Pai::M1, Pai::M1, Pai::M1, Pai::P3, Pai::P4, Pai::P5, Pai::P5, Pai::P7, Pai::P8, Pai::P9, Pai::S3, Pai::S4, Pai::S5},
                                       //{Pai::P1, Pai::P2, Pai::P3, Pai::P5, Pai::P6, Pai::P7, Pai::S2, Pai::S4, Pai::S5, Pai::S6, Pai::S7, Pai::S8, Pai::S9},
                                       //{Pai::P1, Pai::P2, Pai::P3, Pai::S1, Pai::S2, Pai::S3, Pai::S4, Pai::S4, Pai::S6, Pai::S6, Pai::S7, Pai::S7, Pai::S8},
                                       //{Pai::M1, Pai::M1, Pai::P2, Pai::P3, Pai::P4, Pai::P5, Pai::P6, Pai::P7, Pai::P8, Pai::P9, Pai::S3, Pai::S4, Pai::S5},
                                       //{Pai::P1, Pai::P1, Pai::P1, Pai::P4, Pai::P5, Pai::P6, Pai::S1, Pai::S2, Pai::S3, Pai::S6, Pai::S7, Pai::Center, Pai::Center},
                                       //{Pai::P1, Pai::P1, Pai::P1, Pai::P1, Pai::P2, Pai::P3, Pai::S1, Pai::S2, Pai::S3, Pai::S6, Pai::S7, Pai::Center, Pai::Center},
                                       //{Pai::P2, Pai::P2, Pai::P2, Pai::P7, Pai::P7, Pai::P7, Pai::P8, Pai::S5, Pai::S6, Pai::S7, Pai::Center, Pai::Center, Pai::Center},
                                       //{Pai::M1, Pai::M1, Pai::M1, Pai::M9, Pai::M9, Pai::M9, Pai::P1, Pai::P1, Pai::P1, Pai::P5, Pai::West, Pai::West, Pai::West},
                                       //{Pai::P1, Pai::P2, Pai::P3, Pai::P4, Pai::P7, Pai::P7, Pai::P7, Pai::S2, Pai::S3, Pai::S4, Pai::S7, Pai::S7, Pai::S7},
                                       field.GetFirstPais(),
                                       GetStartWind(field, Wind::South));
    players[House::Down].GameStartInit(field.GetFirstPais(), GetStartWind(field, Wind::West));
  }

  void ContinueSetInit(Field &field) {
    players[Model::House::Up].ContinueSetInit(field.GetFirstPais());
    players[Model::House::Self].ContinueSetInit(field.GetFirstPais());
    players[Model::House::Down].ContinueSetInit(field.GetFirstPais());
  }

  void NextSetInit(Field &field) {
    players[Model::House::Up].NextSetInit(field.GetFirstPais());
    players[Model::House::Self].NextSetInit(field.GetFirstPais());
    players[Model::House::Down].NextSetInit(field.GetFirstPais());
  }

  bool HasFlyHouse() const {
    return players[Model::House::Up].GetPoint() < 0 || players[Model::House::Self].GetPoint() < 0 || players[Model::House::Down].GetPoint() < 0;
  }

  void SetTumoPoint(House house, const Point &point, Field &field) {
    int getPoint = 0;
    House payHouse = GetDownHouse(house);
    for (int i = 0; i < 2; ++i) {
      const bool isBreak = field.GetBreakHouse() == payHouse || field.GetBreakHouse() == house;
      const int payPoint = players[house].IsParent() || players[payHouse].IsParent() ?
                           -point.GetChildTumoParentPoint(isBreak) :
                           -point.GetChildTumoChildPoint(isBreak);
      players[payHouse].AddPoint(payPoint);
      getPoint += -payPoint;
      payHouse = GetDownHouse(payHouse);
    }
    players[house].AddPoint(getPoint + field.ReleaseReachBar());
  }

  void SetRonPoint(House house, House payHouse, const Point &point, Field &field) {
    const bool isBreak = field.GetBreakHouse() == payHouse || field.GetBreakHouse() == house;
    const int getPoint = players[house].IsParent() ?  point.GetParentRonPoint(isBreak) : point.GetChildRonPoint(isBreak);
    for (int i = 0; i < 3; ++i) {
      players[static_cast<House>(i)].AddPoint(0);
    }
    players[house].AddPoint(getPoint + field.ReleaseReachBar());
    players[payHouse].AddPoint(-getPoint);
  }

  bool IsParentTenpai(const Field &field) {
    for (auto &player : players) {
      if (player.IsParent() && player.IsTypeTenpai(field)) {
        return true;
      }
    }
    return false;
  }

  boost::optional<House> SetFlowPoint(Field &field, boost::optional<Point> &p) {
    for (int i = 0; i < 3; ++i) {
      if (players[i].IsFever()) {
        players[i].AddPoint(6000 + field.ReleaseReachBar());
        players[GetUpHouse(static_cast<House>(i))].AddPoint(-3000);
        players[GetDownHouse(static_cast<House>(i))].AddPoint(-3000);
        players[i].SetFlow(false, field);
        players[GetUpHouse(static_cast<House>(i))].SetFlow(true, field);
        players[GetDownHouse(static_cast<House>(i))].SetFlow(true, field);
        return boost::none;
      }
    }
    for (int i = 0; i < 3; ++i) {
      if (const auto point = players[i].GetLimitHandSink(field)) {
        p = *point;
        SetTumoPoint(static_cast<House>(i), *point, field);
        players[i].SetFlow(true, field);
        players[GetUpHouse(static_cast<House>(i))].SetFlow(true, field);
        players[GetDownHouse(static_cast<House>(i))].SetFlow(true, field);
        return static_cast<House>(i);
      }
    }
    const int tenpaiCount = boost::count_if(players, [&field](Player &p) {
      return p.IsTypeTenpai(field);
    });
    if (tenpaiCount == 1 || tenpaiCount == 2) {
      for (auto &player : players) {
        if (player.IsTypeTenpai(field)) {
          player.AddPoint(tenpaiCount ? 6000 : 3000);
        } else {
          player.AddPoint(tenpaiCount ? -3000 : -6000);
        }
        player.SetFlow(false, field);
      }
    } else {
      for (auto &player : players) {
        player.AddPoint(0);
        player.SetFlow(false, field);
      }
    }
    return boost::none;
  }

  void SetFuritenPai(Pai pai) {
    for (auto &player : players) {
      player.SetFuritenPai(pai);
    }
  }

  Pai TumoCut(House house) {
    return players[house].TumoCut();
  }

  Pai HandCut(House house, int i) {
    return players[house].HandCut(i);
  }

  void AllDeleteFirst() {
    players[House::Up].DeleteFirst();
    players[House::Self].DeleteFirst();
    players[House::Down].DeleteFirst();
  }

  void Pon(House squeareHouse, House squearedHouse, Pai pai) {
    AllDeleteFirst();
    players[squearedHouse].PopBackRiver();
    players[squeareHouse].Pon(squearedHouse, pai);
  }

  boost::optional<Point> LightKan(House squeareHouse, House squearedHouse, Pai pai, Field &field) {
    AllDeleteFirst();
    players[squearedHouse].PopBackRiver();
    return players[squeareHouse].LightKan(squearedHouse, pai, field);
  }

  const Player &operator [](House house) const {
    return players[house];
  }

  Player &operator [](House house) {
    return players[house];
  }

 private:
  static Wind GetStartWind(const Field &field, Wind firstWind) {
    return !field.GetFirstParentHouse() ? 
           firstWind : field.GetFirstParentHouse() == House::Self ?
                       GetPrevWind(firstWind) :
                       GetNextWind(firstWind);
  }

  std::array<Player, 3> players;
};
}}

#endif
