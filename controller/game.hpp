#ifndef FEVERMJ_CONTROLLER_GAME_HPP_
#define FEVERMJ_CONTROLLER_GAME_HPP_
#include <array>
#include <functional>
#include <DxLib.h>
#include <boost/noncopyable.hpp>
#include "input.hpp"
#include "network.hpp"
#include "sequence.hpp"
#include "../model/field.hpp"
#include "../model/players.hpp"
#include "../model/response.hpp"
#include "../utility/network.hpp"
#include "../utility/pai_image.hpp"
#include "../view/game.hpp"

namespace FeverMJ { namespace Controller {
class Game : public Sequence {
 public:
  explicit Game(int firstParent, int seed, Utility::NetHandleArray &&handles) 
      : network(std::move(handles)), isEndress(firstParent < 0) {
    SRand(seed);
    field.FirstGameInit(firstParent >= 0 ? static_cast<Model::House>(firstParent) : static_cast<Model::House>(GetRand(2)));
    gameView.SetStart();
    GameStart();
  }

  std::unique_ptr<Sequence> Update() {
    input.Update();
    gameView.Draw(input, field, players, paiImage);
    sequence();
    return std::move(next);
  }

 private:
  void GameStart() {
    players.GameStartInit(field);
    sequence = [this] {Tumo(field.GetFirstParentHouse());};
  }

  void Tumo(Model::House house) {
    if (house == Model::House::Self) {
      sequence = [this] {SelfTumo();};
    } else {
      const auto point = players[house].Tumo(field);
      sequence = [this, house, point] {OtherHouseTern(house, point, false);};
    }
  }

  void SelfTumo() {
    sequence = [this] {
      if (players[Model::House::Self].IsReachTenpai()) {
        CheckSelfReachHand(players[Model::House::Self].Tumo(field));
      } else {
        CheckSelfHand(false, players[Model::House::Self].Tumo(field));
      }
    };
  }

  void CheckSelfReachHand(const boost::optional<Model::Point> &point) {
    const auto fever = players.GetFeverHouse();
    if (fever && fever != Model::House::Self) {
      SelfDiscardPai(-1, false, Model::ReachState::Normal);
    } else {
      CheckTumo(point);
      const bool isEraseNorthEnable = CheckEraseNorth(false);
      const bool isReachKanEnable = CheckKan(false);
      if (players[Model::House::Self].IsFever() && point) {
        sequence = [] {};
      } else if (point || isReachKanEnable || isEraseNorthEnable) {
        gameView.SetMenuMode([this] {
          SelfDiscardPai(-1, false);
          gameView.SetWaitMode();
        }, View::MenuMode::Cancel);
        sequence = [] {};
      } else {
        SelfDiscardPai(-1, false);
      }
    }
  }

  void CheckSelfHand(bool isAddDora, const boost::optional<Model::Point> &point) {
    const auto fever = players.GetFeverHouse();
    if (fever) {
      SelfDiscardPai(-1, false);
    } else {
      CheckTumo(point);
      CheckReach();
      CheckEraseNorth(isAddDora);
      CheckKan(isAddDora);
      sequence = [] {};
      gameView.SetSelectMode([this, isAddDora](int i) {
        SelfDiscardPai(i, isAddDora);
        gameView.SetWaitMode();
      });
    }
  }

  void SelfDiscardPai(int i, bool isAddDora, Model::ReachState state = Model::ReachState::Normal) {
    Model::Pai pai;
    if (i < 0) {
      pai = players.TumoCut(Model::House::Self, state != Model::ReachState::Normal);
      network.SendTumoCut(state);
    } else {
      pai = players.HandCut(Model::House::Self, i, state != Model::ReachState::Normal);
      network.SendHandCut(state, i);
    }
    responsePair = Model::responseWaitPair;
    gameView.SetWaitMode();
    sequence = [this, pai, isAddDora, state] {OtherHouseInterruptWait(pai, isAddDora, state != Model::ReachState::Normal);};
  }

  void OtherHouseInterruptWait(Model::Pai pai, bool isAddDora, bool isReach) {
    CheckResponse();
    if (responsePair.up.data != Model::Response::Wait && responsePair.down.data != Model::Response::Wait) {
      const auto resp = Model::GetHighPriorityResponse(field.GetParentHouse(), responsePair.up, responsePair.down);
      if (resp.first.data == Model::Response::Ron) {
        if (isReach) {
          field.RemoveReachBar();
          players[Model::House::Self].ReachCancel();
        }
        SelfDiscardPaiInterruptRon(pai, resp);
      } else {
        if (isAddDora) {
          field.AddDora();
        }
        if (resp.first.data == Model::Response::Pass) {
          SelfDiscardPaiSuccess(resp);
        } else if (resp.first.data == Model::Response::Ti) {
          SelfDiscardPaiInterruptTi(pai, resp); 
        } else if (resp.first.data == Model::Response::Pon) {
          SelfDiscardPaiInterruptPon(pai, resp);
        } else if (resp.first.data == Model::Response::LightKan) {
          SelfDiscardPaiInterruptLightKan(pai, resp);
        } else {
          throw Utility::NetWorkError("受信失敗");
        }
        players.SetFuritenPai(pai);
      }
    }
  }

  void OtherHouseTern(Model::House house, const boost::optional<Model::Point> &point, bool isAddDora) {
    const auto response = network.GetTernResponse(house);
    switch (response.data) {
      case Model::Response::PaiCut: {
        const auto pai = response.value0 == 14 ? players.TumoCut(house) : players.HandCut(house, response.value0);
        sequence = [this, house, pai, isAddDora] {CheckSelfSqueal(house, pai, isAddDora, false);};
        break;
      }
      case Model::Response::EraseNorth: {
        players[house].EraseNorth();
        sequence = [this, house, isAddDora] {CheckSelfEraseNorth(house, isAddDora);};
        break;
      }
      case Model::Response::DarkKan: {
        players[house].DarkKan(static_cast<Model::Pai>(response.value0), field);
        field.AddDora();
        break;
      }
      case Model::Response::AddKan: {
        sequence = [this, house, response, isAddDora] {
          CheckSelfChanKan(house, static_cast<Model::Pai>(response.value0), isAddDora);
        };
        break;
      }
      case Model::Response::Reach: {
        const auto pai = response.value0 == 14 ? players.TumoCut(house, true) : players.HandCut(house, response.value0, true);
        players[house].SetReachFirst();
        sequence = [this, house, pai] {CheckSelfSqueal(house, pai, false, true);};
        field.AddReachBar();
        break;
      }
      case Model::Response::OpenReach: {
        const auto pai = response.value0 == 14 ? players.TumoCut(house, true) : players.HandCut(house, response.value0, true);
        players[house].SetOpenReach();
        sequence = [this, house, pai] {CheckSelfSqueal(house, pai, false, true);};
        field.AddReachBar();
        break;
      }
      case Model::Response::FeverReach: {
        const auto pai = response.value0 == 14 ? players.TumoCut(house, true) : players.HandCut(house, response.value0, true);
        players[house].SetFeverReach();
        sequence = [this, house, pai] {CheckSelfRon(house, pai);};
        field.AddReachBar();
        break;
      }
      case Model::Response::DoubleFeverReach: {
        const auto pai = response.value0 == 14 ? players.TumoCut(house, true) : players.HandCut(house, response.value0, true);
        players[house].SetDoubleFeverReach();
        sequence = [this, house, pai] {CheckSelfRon(house, pai);};
        field.AddReachBar();
        break;
      }
      case Model::Response::Tumo: {
        TumoGoal(house, *point);
        break;
      }
      case Model::Response::Wait: {
        break;
      }
      default: {
        throw Utility::NetWorkError("受信失敗");
      }
    }
  }

  void CheckSelfEraseNorth(Model::House house, bool isAddDora) {
    const bool isPonEnable = CheckPon(house, Model::Pai::North, true, isAddDora, false);
    const bool isKanEnable = CheckLightKan(house, Model::Pai::North, true, isAddDora, false);
    const auto point = CheckRon(house, Model::Pai::North, false);
    if (isPonEnable || isKanEnable || point) {
      gameView.SetMenuMode([this, house, isAddDora] {
        network.SendPass(house);
        sequence = [this, house, isAddDora] {ThroughEraseNorth(house, isAddDora);};
        gameView.SetWaitMode();
      }, View::MenuMode::Cancel);
      sequence = [] {};
    } else {
      network.SendPass(house);
      sequence = [this, house, isAddDora] {ThroughEraseNorth(house, isAddDora);};
    }
  }

  void CheckSelfChanKan(Model::House house, Model::Pai pai, bool isAddDora) {
    if (CheckRon(house, pai, false)) {
      gameView.SetMenuMode([this, house, pai, isAddDora] {
        network.SendPass(house);
        sequence = [this, house, pai, isAddDora] {ThroughChankan(house, pai, isAddDora);};
        gameView.SetWaitMode();
      }, View::MenuMode::Cancel);
      sequence = [] {};
    } else {
      network.SendPass(house);
      sequence = [this, house, pai, isAddDora] {ThroughChankan(house, pai, isAddDora);};
    }
  }

  void CheckSelfSqueal(Model::House house, Model::Pai pai, bool isAddDora, bool isReach) {
    const bool isPonEnable = CheckPon(house, pai, false, isAddDora, isReach);
    const bool isKanEnable = CheckLightKan(house, pai, false, isAddDora, isReach);
    const auto tiList = CheckTi(house, pai, isAddDora, isReach);
    const auto point = CheckRon(house, pai, isReach);
    if (isPonEnable || isKanEnable || !tiList.empty() || point) {
      gameView.SetMenuMode([this, house, pai, isAddDora, isReach] {
        network.SendPass(house);
        sequence = [this, house, pai, isAddDora, isReach] {ThroughSqueal(house, pai, isAddDora, isReach);};
        gameView.SetWaitMode();
      }, View::MenuMode::Cancel);
      sequence = [] {};
    } else {
      network.SendPass(house);
      sequence = [this, house, pai, isAddDora, isReach] {ThroughSqueal(house, pai, isAddDora, isReach);};
    }
  }

  void CheckSelfRon(Model::House house, Model::Pai pai) {
    if (CheckRon(house, pai, true)) {
      gameView.SetMenuMode([this, house, pai] {
        sequence = [this, house, pai] {ThroughSqueal(house, pai, false, true);};
        gameView.SetWaitMode();
      }, View::MenuMode::Cancel);
      sequence = [] {};
    } else {
      sequence = [this, house, pai] {ThroughSqueal(house, pai, false, false);};
    }
  }

  void ThroughEraseNorth(Model::House house, bool isAddDora) {
    const auto resp = network.GetSquealResponse(house);
    if (resp.data != Model::Response::Wait || isEndress) {
      if (resp.data == Model::Response::Ron) {
        InterruptRon(Model::GetOtherHouse(house), house, Model::Pai::North, false);
      } else {
        if (isAddDora) {
          field.AddDora();
        }
        if (resp.data == Model::Response::Pass || isEndress) {
          const auto point = players[house].EraseNorthTumo(field);
          sequence = [this, house, point] {OtherHouseTern(house, point, false);};
        } else if (resp.data == Model::Response::Pon) {
          InterruptPon(Model::GetOtherHouse(house), house, Model::Pai::North);
        }
      }
    }
  }

  void ThroughChankan(Model::House house, Model::Pai pai, bool isAddDora) {
    const auto resp = network.GetSquealResponse(house);
    if (resp.data != Model::Response::Wait || isEndress) {
      if (resp.data == Model::Response::Ron) {
        InterruptRon(Model::GetOtherHouse(house), house, pai, false);
      } else {
        if (isAddDora) {
          field.AddDora();
        }
        field.AddDora();
        const auto point = players[house].RinsyanTumo(field);
        sequence = [this, house, point] {OtherHouseTern(house, point, false);};
      }
    }
  }

  void Ti(Model::Pai pai, const Model::TiPair &tiPair, bool isAddDora, bool isReach) {
    const auto resp = network.GetSquealResponse(Model::House::Up);
    if (resp.data != Model::Response::Wait || isEndress) {
      if (resp.data == Model::Response::Ron) {
        InterruptRon(Model::House::Down, Model::House::Up, pai, isReach);
      } else {
        if (isAddDora) {
          field.AddDora();
        }
        if (resp.data == Model::Response::Ti || isEndress) {
          players.Ti(Model::House::Self, pai, tiPair);
          sequence = [this] {CheckSelfSquealHand();};
        } else if (resp.data == Model::Response::Pon) {
          InterruptPon(Model::House::Down, Model::House::Up, pai);
        } else if (resp.data == Model::Response::LightKan) {
          InterruptLightKan(Model::House::Down, Model::House::Up, pai);
        }
      }
    }
  }

  void Pon(Model::House house, Model::Pai pai, bool isAddDora, bool isEraseNorth, bool isReach) {
    const auto resp = network.GetSquealResponse(house);
    if (resp.data != Model::Response::Wait || isEndress) {
      if (resp.data == Model::Response::Pon || isEndress) {
        if (isAddDora) {
          field.AddDora();
        }
        if (isEraseNorth) {
          players.EraseNorthPon(Model::House::Self, house);
        } else {
          players.Pon(Model::House::Self, house, pai);
        }
        sequence = [this] {CheckSelfSquealHand();};
      } else if (resp.data == Model::Response::Ron) {
        InterruptRon(Model::GetOtherHouse(house), house, pai, isReach);
      }
    }
  }

  void LightKan(Model::House house, Model::Pai pai, bool isAddDora, bool isEraseNorth, bool isReach) {
    const auto resp = network.GetSquealResponse(house);
    if (resp.data != Model::Response::Wait || isEndress) {
      if (resp.data == Model::Response::LightKan || isEndress) {
        if (isAddDora) {
          field.AddDora();
        }
        const auto point = isEraseNorth ? 
                           players.EraseNorthLightKan(Model::House::Self, house, field) : 
                           players.LightKan(Model::House::Self, house, pai, field);
        sequence = [this, point] {CheckSelfHand(true, point);};
      } else if (resp.data == Model::Response::Ron) {
        InterruptRon(Model::GetOtherHouse(house), house, pai, isReach);
      }
    }
  }

  void Ron(const Model::Point &point, Model::House house, bool isReach = false) {
    const auto resp = network.GetSquealResponse(house);
    if (resp.data != Model::Response::Wait || isEndress) {
      if (isReach) {
        players[house].ReachCancel();
        field.RemoveReachBar();
      }
      if (resp.data == Model::Response::Ron || isEndress) {
        players.SetRonPoint(Model::House::Self, house, point, field);
        gameView.SetSelectRon([this, house] {
          if (players[Model::House::Self].IsFever()) {
            sequence = [this, house] {ThroughSqueal(house, players[Model::House::Self].GetGoalPai(), false, false);};
            gameView.SetStart();
          } else {
            GotoNextGame();
          }
        }, point, Model::House::Self);
        sequence = [] {};
      } else if (resp.data == Model::Response::Pass) {
        InterruptRon(Model::GetOtherHouse(house), house, players[Model::House::Self].GetGoalPai(), false);
      }
    }
  }

  void ThroughSqueal(Model::House house, Model::Pai pai, bool isAddDora, bool isReach) {
    const auto resp = network.GetSquealResponse(house);
    if (resp.data != Model::Response::Wait || isEndress) {
      if (resp.data == Model::Response::Ron) {
        InterruptRon(Model::GetOtherHouse(house), house, pai, isReach);
      } else {
        if (isAddDora) {
          field.AddDora();
        }
        if (resp.data == Model::Response::Pass || isEndress) {
          players[house].SetRiver();
          if (field.IsPaiEmpty()) {
            sequence = [this] {FlowSet();};
          } else {
            if (house == Model::House::Up) {
              sequence = [this] {SelfTumo();};
            } else {
              sequence = [this] {Tumo(Model::House::Up);};
            }
          }
        } else if (resp.data == Model::Response::Ti) {
          InterruptTi(Model::GetOtherHouse(house),
                      house,
                      pai,
                      {static_cast<Model::Pai>(resp.value0), static_cast<Model::Pai>(resp.value1)});
        } else if (resp.data == Model::Response::Pon) {
          InterruptPon(Model::GetOtherHouse(house), house, pai);
        } else if (resp.data == Model::Response::LightKan) {
          InterruptLightKan(Model::GetOtherHouse(house), house, pai);
        }
      }
    }
  }

  void CheckSelfSquealHand() {
    sequence = [] {};
    gameView.SetSelectMode([this](int i) {
      SelfDiscardPai(i, false);
    });
  }

  void FlowSet() {
    const auto fever = players.GetFeverHouse();
    if (fever && players[*fever].GetDiffPoint() != -1000) {
      gameView.SetFeverResult([this] {
        GotoNextGame();
      });
    } else {
      boost::optional<Model::Point> point;
      if (const auto house = players.SetFlowPoint(field, point)) {
        gameView.SetSelectTumo([this] {
          GotoNextGame();
        }, *point, *house);
      } else {
        gameView.SetFlowSet([this] {
          GotoNextGame();
        });
      }
    }
    sequence = [] {};
  }

  void CheckTumo(const boost::optional<Model::Point> &point) {
    if (point) {
      if (players[Model::House::Self].IsFever()) {
        TumoGoal(Model::House::Self, *point);
      } else {
        gameView.SetMenuMode([this, point] {
          TumoGoal(Model::House::Self, *point);
        }, View::MenuMode::Tumo);
      }
    }
  }

  void CheckReach() {
    const auto reachIndex = players[Model::House::Self].GetReachEnableIndex();
    if (field.GetTumoMountainSize() > 2 && players[Model::House::Self].GetPoint() >= 1000) {
      if (reachIndex.reach) {
        gameView.SetMenuMode([this, reachIndex] {
          SelectReach(reachIndex.reach);
        }, View::MenuMode::Reach);
        gameView.SetMenuMode([this, reachIndex] {
          SelectOpenReach(reachIndex.reach);
        }, View::MenuMode::OpenReach);
      }
      if (reachIndex.fever) {
        gameView.SetMenuMode([this, reachIndex] {
          SelectFeverReach(reachIndex);
        }, View::MenuMode::Fever);
      }
    }
  }

  bool CheckEraseNorth(bool isAddDora) {
    if (!field.IsPaiEmpty()) {
      if ((players[Model::House::Self].IsReachTenpai() && players[Model::House::Self].GetTumo() == Model::Pai::North) ||
          (!players[Model::House::Self].IsReachTenpai() && players[Model::House::Self].GetPaiCount(Model::Pai::North))) {
        gameView.SetMenuMode([this] {
          SelectNorth(false);
        }, View::MenuMode::EraseNorth);
        return true;
      }
    }
    return false;
  }

  bool CheckKan(bool isAddDora) {
    if (!field.IsPaiEmpty() && field.GetDoraCount() <= 5) {
      if (players[Model::House::Self].IsReachTenpai()) {
        if (players[Model::House::Self].IsReachKanEnable()) {
          gameView.SetMenuMode([this] {
            SelectReachKan();
          }, View::MenuMode::Kan);
          return true;
        }
      } else if (players[Model::House::Self].IsDarkOrAddKanEnable()) {
        gameView.SetMenuMode([this, isAddDora] {
          SelectKan(isAddDora);
        }, View::MenuMode::Kan);
        return true;
      }
    }
    return false;
  }

  void TumoGoal(Model::House house, const Model::Point &point) {
    if (house == Model::House::Self) {
      network.SendTumoGoal();
    }
    players.SetTumoPoint(house, point, field);
    gameView.SetWaitMode();
    gameView.SetSelectTumo([this, house] {
      if (players[house].IsFever()) {
        players.TumoCut(house);
        if (field.IsPaiEmpty()) {
          sequence = [this] {FlowSet();};
        } else {
          gameView.SetStart();
          sequence = [this, house] {Tumo(Model::GetDownHouse(house));};
        }
      } else {    
        GotoNextGame();
      }
    }, point, house);
    sequence = [] {};
  }

  bool CheckPon(Model::House house, Model::Pai pai, bool isEraseNorth, bool isAddDora, bool isReach) {
    if (players.IsSelfPonEnable(field, pai) && !gameView.NotSquealButtonIsToggle()) {
      gameView.SetMenuMode([=] {
        gameView.SetWaitMode();
        network.SendPon(house);
        sequence = [=] {Pon(house, pai, isEraseNorth, isAddDora, isReach);};
      }, View::MenuMode::Pon);
      return true;
    }
    return false;
  }

  bool CheckLightKan(Model::House house, Model::Pai pai, bool isEraseNorth, bool isAddDora, bool isReach) { 
    if (players.IsSelfLightKanEnable(field, pai) && !gameView.NotSquealButtonIsToggle()) {
      gameView.SetMenuMode([=] {
        gameView.SetWaitMode();
        network.SendLightKan(house);
        sequence = [=] {LightKan(house, pai, isEraseNorth, isAddDora, isReach);};
      }, View::MenuMode::Kan);
      return true;
    }
    return false;
  }

  std::vector<Model::TiPair> CheckTi(Model::House house, Model::Pai pai, bool isAddDora, bool isReach) {
    const auto tiList = !gameView.NotSquealButtonIsToggle() ?
                        players.GetSelfTiCandidate(field, house, pai) : std::vector<Model::TiPair>{};
    if (!tiList.empty()) {
      gameView.SetMenuMode([=] {
        SelectTi(pai, tiList, isAddDora, isReach);
      }, View::MenuMode::Ti);
    }
    return tiList;
  }

  boost::optional<Model::Point> CheckRon(Model::House house, Model::Pai pai, bool isReach) {
    const auto point = players.CheckRon(field, Model::House::Self, house, pai);
    if (point) {
      gameView.SetMenuMode([=] {
        gameView.SetWaitMode();
        network.SendRon(house); 
        sequence = [=] {Ron(*point, house, isReach);};
      }, View::MenuMode::Ron);
    }
    return point;
  }

  void SelfDiscardPaiSuccess(const std::pair<Model::ResponseData, Model::ResponseData> &pair) {
    players[Model::House::Self].SetRiver();
    network.SendPass(pair.first.house);
    network.SendPass(pair.second.house);
    if (field.IsPaiEmpty()) {
      sequence = [this] {FlowSet();};
    } else {
      sequence = [this] {Tumo(Model::House::Down);};
    }
  }

  void SelfDiscardPaiInterruptTi(Model::Pai pai, const std::pair<Model::ResponseData, Model::ResponseData> &pair) {
    const Model::TiPair tiPair = {static_cast<Model::Pai>(pair.first.value0), static_cast<Model::Pai>(pair.first.value1)};
    network.SendTi(pair.first.house, tiPair);
    network.SendTi(pair.second.house, tiPair);
    players.Ti(pair.first.house, pai, tiPair);
    sequence = [this, pair] {OtherHouseTern(pair.first.house, boost::none, false);};
  }

  void SelfDiscardPaiInterruptPon(Model::Pai pai, const std::pair<Model::ResponseData, Model::ResponseData> &pair) {
    network.SendPon(pair.first.house);
    network.SendPon(pair.second.house);
    players.Pon(pair.first.house, Model::House::Self, pai);
    sequence = [this, pair] {OtherHouseTern(pair.first.house, boost::none, false);};
  }

  void SelfDiscardPaiInterruptLightKan(Model::Pai pai, const std::pair<Model::ResponseData, Model::ResponseData> &pair) {
    network.SendLightKan(pair.first.house);
    network.SendLightKan(pair.second.house);
    players.LightKan(pair.first.house, Model::House::Self, pai, field);
    sequence = [this, pair] {OtherHouseTern(pair.first.house, boost::none, true);};
  }

  void SelfDiscardPaiInterruptRon(Model::Pai pai, const std::pair<Model::ResponseData, Model::ResponseData> &resp) {
    network.SendRon(resp.first.house);
    // プロトコルが汚いからしゃーない
    if (resp.second.data == Model::Response::Ron) {
      network.SendPass(resp.second.house);
    } else {
      network.SendRon(resp.second.house);
    }
    gameView.SetWaitMode();
    const auto point = players.CheckRon(field, resp.first.house, Model::House::Self, pai);
    assert(point);
    players.SetRonPoint(resp.first.house, Model::House::Self, *point, field);
    gameView.SetSelectRon([this, pai, resp] {
      if (players[resp.first.house].IsFever()) {
        sequence = [this, resp, pai] {ThroughSqueal(resp.first.house, pai, false, false);};
        gameView.SetStart();
      } else {
        GotoNextGame();
      }
    }, *point, resp.first.house);
    sequence = [] {};
  }

  void InterruptTi(Model::House goal, Model::House pay, Model::Pai pai, const Model::TiPair &tiPair) {
    players.Ti(goal, pai, tiPair);
    sequence = [this, goal] {OtherHouseTern(goal, boost::none, false);};
  }

  void InterruptPon(Model::House goal, Model::House pay, Model::Pai pai) {
    players.Pon(goal, pay, pai);
    sequence = [this, goal] {OtherHouseTern(goal, boost::none, false);};
  }

  void InterruptLightKan(Model::House goal, Model::House pay, Model::Pai pai) {
    players.LightKan(goal, pay, pai, field);
    sequence = [this, goal] {OtherHouseTern(goal, boost::none, true);};
  }

  void InterruptRon(Model::House goal, Model::House pay, Model::Pai pai, bool isReach) {
    if (isReach) {
      players[pay].ReachCancel();
      field.RemoveReachBar();
    }
    const auto point = players.CheckRon(field, goal, pay, pai);
    assert(point);
    players.SetRonPoint(goal, pay, *point, field);
    gameView.SetSelectRon([this] {
      GotoNextGame();
    }, *point, goal);
    sequence = [] {};
  }

  void SelectReach(std::uint32_t reachIndex) {
    gameView.SetWaitMode();
    gameView.SetSelectReachMode(reachIndex, [this] (int index) {
      SelfDiscardPai(index, false, Model::ReachState::ReachStart);
      players[Model::House::Self].SetReachFirst();
      field.AddReachBar();
    });
    sequence = [] {};
  }

  void SelectOpenReach(std::uint32_t reachIndex) {
    gameView.SetWaitMode();
    gameView.SetSelectReachMode(reachIndex, [this] (int index) {
      SelfDiscardPai(index, false, Model::ReachState::OpenStart);
      players[Model::House::Self].SetOpenReach();
      field.AddReachBar();
    });
    sequence = [] {};
  }

  void SelectFeverReach(const Model::ReachIndex &index) {
    gameView.SetWaitMode();
    gameView.SetSelectReachMode(index.fever, [this, index] (int i) {
      if (index.doubleFever & (1 << (i < 0 ? 14 : i))) {
        SelfDiscardPai(i, false, Model::ReachState::DoubleFeverStart);
        players[Model::House::Self].SetDoubleFeverReach();
      } else {
        SelfDiscardPai(i, false, Model::ReachState::FeverStart);
        players[Model::House::Self].SetFeverReach();
      }
      field.AddReachBar();
    });
    sequence = [] {};
  }

  void SelectKan(bool isAddDora) {
    players.AllDeleteFirst();
    gameView.SetWaitMode();
    gameView.SetSelectKanMode([this, isAddDora] (const boost::optional<Model::Pai> &darkKanPai,
                                                 const boost::optional<Model::Pai> &addKanPai) {
      if (darkKanPai) {
        if (isAddDora) {
          field.AddDora();
        }
        field.AddDora();
        sequence = [this, darkKanPai] {
          network.SendDarkKan(*darkKanPai);
          if (players[Model::House::Self].IsReachTenpai()) {
            CheckSelfReachHand(players[Model::House::Self].DarkKan(*darkKanPai, field));
          } else {
            CheckSelfHand(false, players[Model::House::Self].DarkKan(*darkKanPai, field));
          }
        };
      } else {
        responsePair = Model::responseWaitPair;
        network.SendAddKan(*addKanPai);
        players[Model::House::Self].AddKan(*addKanPai);
        sequence = [this, addKanPai, isAddDora] {ChanKanInterruptWait(*addKanPai, isAddDora);};
      }        
      gameView.SetWaitMode();
    });
    sequence = [] {};
  }

  void SelectReachKan() {
    players.AllDeleteFirst();
    field.AddDora();
    sequence = [this] {CheckSelfReachHand(players[Model::House::Self].DarkKan(*players[Model::House::Self].GetTumo(), field));};
    gameView.SetWaitMode();
  }

  void SelectTi(Model::Pai pai, const std::vector<Model::TiPair> &tiList, bool isAddDora, bool isReach) {
    gameView.SetWaitMode();
    gameView.SetSelectTiMode([=] (const Model::TiPair &tiPair) {
      network.SendTi(Model::House::Up, tiPair);
      sequence = [=] {Ti(pai, tiPair, isAddDora, isReach);};
      gameView.SetWaitMode();
    }, tiList);
    sequence = [] {};
  }

  void SelectNorth(bool isAddDora) {
    players.AllDeleteFirst();
    gameView.SetWaitMode();
    players[Model::House::Self].EraseNorth();
    network.SendEraseNorth();
    responsePair = Model::responseWaitPair;
    sequence = [this, isAddDora] {EraseNorthInterruptWait(isAddDora);};
  }

  void SelfEraseNorthSuccess(const std::pair<Model::ResponseData, Model::ResponseData> &pair) {
    network.SendPass(pair.first.house);
    network.SendPass(pair.second.house);
    sequence = [this] {
      if (players[Model::House::Self].IsReachTenpai()) {
        CheckSelfReachHand(players[Model::House::Self].EraseNorthTumo(field));
      } else {
        CheckSelfHand(false, players[Model::House::Self].EraseNorthTumo(field));
      }
    };
  }

  void SelfEraseNorthInterruptPon(const std::pair<Model::ResponseData, Model::ResponseData> &resp) {
    network.SendPon(resp.first.house);
    network.SendPon(resp.second.house);
    players.EraseNorthPon(resp.first.house, Model::House::Self);
    sequence = [this, resp] {OtherHouseTern(resp.first.house, boost::none, false);};
  }

  void SelfEraseNorthInterruptLightKan(const std::pair<Model::ResponseData, Model::ResponseData> &resp) {
    network.SendLightKan(resp.first.house);
    network.SendLightKan(resp.second.house);
    players.EraseNorthLightKan(resp.first.house, Model::House::Self, field);
    sequence = [this, resp] {OtherHouseTern(resp.first.house, boost::none, true);};
  }

  void SelfInterruptRon(Model::Pai pai, const std::pair<Model::ResponseData, Model::ResponseData> &resp) {
    network.SendRon(resp.first.house);
    // プロトコルが汚いからしゃーない
    if (resp.second.data == Model::Response::Ron) {
      network.SendPass(resp.second.house);
    } else {
      network.SendRon(resp.second.house);
    }
    gameView.SetWaitMode();
    const auto point = players.CheckRon(field, resp.first.house, Model::House::Self, pai);
    players.SetRonPoint(resp.first.house, Model::House::Self, *point, field);
    gameView.SetSelectRon([this, resp] {
      GotoNextGame();
    }, *point, resp.first.house);
    sequence = [] {};
  }

  void ChanKanInterruptWait(Model::Pai pai, bool isAddDora) {
    CheckResponse();
    if (responsePair.up.data != Model::Response::Wait && responsePair.down.data != Model::Response::Wait) {
      const auto resp = Model::GetHighPriorityResponse(field.GetParentHouse(), responsePair.up, responsePair.down);
      if (resp.first.data == Model::Response::Pass) {
        if (isAddDora) {
          field.AddDora();
        }
        field.AddDora();
        sequence = [this] {
          CheckSelfHand(false, players[Model::House::Self].RinsyanTumo(field));
        };
      } else if (resp.first.data == Model::Response::Ron) {
        SelfInterruptRon(pai, resp);
      } else {
        throw Utility::NetWorkError("受信失敗");
      }
      players.SetFuritenPai(pai);
    }
  }

  void EraseNorthInterruptWait(bool isAddDora) {
    CheckResponse();
    if (responsePair.up.data != Model::Response::Wait && responsePair.down.data != Model::Response::Wait) {
      const auto resp = Model::GetHighPriorityResponse(field.GetParentHouse(), responsePair.up, responsePair.down);
      if (resp.first.data == Model::Response::Ron) {
        SelfInterruptRon(Model::Pai::North, resp);
      } else {
        if (isAddDora) {
          field.AddDora();
        }
        if (resp.first.data == Model::Response::Pass) {
          SelfEraseNorthSuccess(resp);
        } else if (resp.first.data == Model::Response::Pon) {
          SelfEraseNorthInterruptPon(resp);
        } else if (resp.first.data == Model::Response::LightKan) {
          SelfEraseNorthInterruptLightKan(resp);
        } else {
          throw Utility::NetWorkError("受信失敗");
        }
      }
      players.SetFuritenPai(Model::Pai::North);
    }
  }

  void GotoNextGame() {
    gameView.SetStart();
    if (players.HasFlyHouse()) {
      field.NextGameInit();
      GameStart();
    } else if (players.IsParentTenpai(field)) {
      field.ContinueSetInit();
      players.ContinueSetInit(field);
    } else {
      if (field.IsOLas()) {
        field.NextGameInit();
        GameStart();
      } else {
        field.NextSetInit();
        players.NextSetInit(field);
      }
    }
    sequence = [this] {Tumo(field.GetParentHouse());};
  }

  void CheckResponse() {
    const auto respUp = network.GetSquealResponse(Model::House::Up);
    if (respUp.data != Model::Response::Wait) {
      responsePair.up = respUp;
    }
    const auto respDown = network.GetSquealResponse(Model::House::Down);
    if (respDown.data != Model::Response::Wait) {
      responsePair.down = respDown;
    }
  }

  Input input;
  NetWork network;
  Model::Field field;
  Model::Players players;
  View::Game gameView;
  Utility::PaiImage paiImage;
  std::function<void ()> sequence;
  bool isEndress;
  std::unique_ptr<Sequence> next = nullptr;
  Model::ResponsePair responsePair;
};
}}

#endif
