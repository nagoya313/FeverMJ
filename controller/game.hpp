#ifndef FEVERMJ_CONTROLLER_GAME_HPP_
#define FEVERMJ_CONTROLLER_GAME_HPP_
#include <array>
#include <functional>
#include <DxLib.h>
#include <boost/noncopyable.hpp>
#include "input.hpp"
#include "../model/field.hpp"
#include "../model/players.hpp"
#include "../utility/pai_image.hpp"
#include "../view/game.hpp"

namespace FeverMJ { namespace Controller {
class Game : boost::noncopyable {
 public:
  Game() {
    // TODO:どこが立家かのレスポンスを待つ
    field.FirstGameInit(static_cast<Model::House>(GetRand(2)));
    GameStart();
  }

  void Update() {
    input.Update();
    gameView.Draw(input, field, players, paiImage);
    sequence();
  }

 private:
  void GameStart() {
    // TODO:配牌のレスポンスを待つ
    players.GameStartInit(field);
    const std::function<void ()> firstSequence[] = {
      [this] {UpHouseTumoWait();}, [this] {SelfTumo();}, [this] {DownHouseTumoWait();}
    };
    sequence = firstSequence[field.GetFirstParentHouse()];
  }

  void UpHouseTumoWait() {
    players[Model::House::Up].Tumo(field);
    sequence = [this] {UpHouseTernWait();};
  }

  void UpHouseTernWait() {
    // TODO:上家の処理待ちレスポンスを待つ
    const auto pai = players.TumoCut(Model::House::Up);
    if (field.IsPaiEmpty()) {
      sequence = [this] {FlowSet();};
    } else {
      sequence = [this, pai] {UpHouseSquealWait(pai);};
    }
  }

  void UpHouseSquealWait(Model::Pai pai) {
    sequence = [this, pai] {CheckSelfSqueal(Model::House::Up, pai);};
  }
  
  void DownHouseTumoWait() {
    players[Model::House::Down].Tumo(field);
    sequence = [this] {DownHouseTernWait();};
  }
  
  void DownHouseTernWait() {
    const auto pai = players.TumoCut(Model::House::Down);
    if (field.IsPaiEmpty()) {
      sequence = [this] {FlowSet();};
    } else {
      sequence = [this, pai] {DownHouseSquealWait(pai);};
    }
  }

  void DownHouseSquealWait(Model::Pai pai) {
    sequence = [this, pai] {CheckSelfSqueal(Model::House::Down, pai);};
  }

  void SelectKan(bool isAddKan) {
    players.AllDeleteFirst();
    gameView.SetWaitMode();
    gameView.SetSelectKanMode([this] (Model::Pai darkKanPai, Model::Pai addKanPai) {
      if (darkKanPai != Model::Pai::Invalid) {
        field.AddDora();
        sequence = [this, darkKanPai] {
          if (players[Model::House::Self].GetPlayerState().IsReachTenpai()) {
            CheckSelfReachHand(players[Model::House::Self].DarkKan(darkKanPai, field));
          } else {
           CheckSelfHand(false, players[Model::House::Self].DarkKan(darkKanPai, field));
          }
        };
      } else {
        sequence = [this, addKanPai] {CheckSelfHand(true, players[Model::House::Self].AddKan(addKanPai, field));};
      }        
      gameView.SetWaitMode();
    });
    if (isAddKan) {
      field.AddDora();
    }
    sequence = [] {};
  }

  void SelectTi(Model::Pai pai, const std::vector<std::pair<Model::Pai, Model::Pai>> &tiList) {
    players.AllDeleteFirst();
    gameView.SetWaitMode();
    gameView.SetSelectTiMode([this, pai] (const std::pair<Model::Pai, Model::Pai> &tiPair) {
      players[Model::House::Up].PopBackRiver();
      players[Model::House::Self].Ti(pai, tiPair);
      sequence = [this] {CheckSelfSquealHand();};
      gameView.SetWaitMode();
    }, tiList);
    sequence = [] {};
  }

  void SelfDiscardPai(int i, bool isAddKan) {
    if (i < 0) {
      players.TumoCut(Model::House::Self);
    } else {
      players.HandCut(Model::House::Self, i);
    }
    if (isAddKan) {
      field.AddDora();
    }
    if (field.IsPaiEmpty()) {
      sequence = [this] {FlowSet();};
    } else {
      // TODO:切った牌を他家に送信
      sequence = [this] {DownHouseTumoWait();};
    }
    gameView.SetWaitMode();
  }

  void SelectRon(const Model::Point &point, Model::House house) {
    gameView.SetWaitMode();
    players.SetRonPoint(Model::House::Self, house, point, field);
    gameView.SetSelectRon([this] {
      GotoNextGame();
    }, point, Model::House::Self);
    sequence = [] {};
  }

  void SelectTumo(const Model::Point &point) {
    gameView.SetWaitMode();
    players.SetTumoPoint(Model::House::Self, point, field);
    gameView.SetSelectTumo([this] {
      GotoNextGame();
    }, point, Model::House::Self);
    sequence = [] {};
  }

  void SelectReach(std::uint32_t reachIndex) {
    gameView.SetWaitMode();
    // TODO:リーチ不成立時のリーチ棒、供託棒の処理
    gameView.SetSelectReachMode(reachIndex, [this] (int index) {
      players[Model::House::Self].SetReach();
      SelfDiscardPai(index, false);
      players[Model::House::Self].SetReachRiver();
    });
    sequence = [] {};
  }

  void SelectOpenReach(std::uint32_t reachIndex) {
    gameView.SetWaitMode();
    // TODO:リーチ不成立時のリーチ棒、供託棒の処理
    gameView.SetSelectReachMode(reachIndex, [this] (int index) {
      players[Model::House::Self].SetOpenReach();
      SelfDiscardPai(index, false);
      players[Model::House::Self].SetReachRiver();
    });
    sequence = [] {};
  }

  void CheckSelfReachHand(const Model::Point &point) {
    const auto han = point.GetHan();
    if (han) {
      gameView.SetMenuMode([this, point] {
        SelectTumo(point);
      }, View::MenuMode::Tumo);
    }
    bool isEraseNorthEnable = false;
    bool isReachKanEnable = false;
    if (!field.IsPaiEmpty()) {
      if ((isReachKanEnable = field.GetDoraCount() <= 5 && players[Model::House::Self].IsReachKanEnable())) {
        gameView.SetMenuMode([this] {
          SelectReachKan();
        }, View::MenuMode::Kan);
      }
      if ((isEraseNorthEnable = players[Model::House::Self].GetTumo() == Model::Pai::North)) {
        gameView.SetMenuMode([this] {
          SelectNorth(false);
        }, View::MenuMode::EraseNorth);
      }
    }
    if (han || isReachKanEnable || isEraseNorthEnable) {
      gameView.SetMenuMode([this] {
        SelfDiscardPai(-1, false);
        gameView.SetWaitMode();
      }, View::MenuMode::Cancel);
      sequence = [] {};
    } else {
      SelfDiscardPai(-1, false);
    }
  }

  void CheckSelfHand(bool isAddKan, const Model::Point &point) {
    if (point.GetHan()) {
      gameView.SetMenuMode([this, point] {
        SelectTumo(point);
      }, View::MenuMode::Tumo);
    }
    const std::uint32_t reachIndex = players[Model::House::Self].GetReachEnableIndex();
    if (reachIndex && field.GetTumoMountainSize() > 2) {
      gameView.SetMenuMode([this, reachIndex] {
        SelectReach(reachIndex);
      }, View::MenuMode::Reach);
      gameView.SetMenuMode([this, reachIndex] {
        SelectOpenReach(reachIndex);
      }, View::MenuMode::OpenReach);
    }
    if (!field.IsPaiEmpty()) {
      if (field.GetDoraCount() <= 5 && players[Model::House::Self].IsDarkOrAddKanEnable() && !gameView.NotSquealButtonIsToggle()) {
        gameView.SetMenuMode([this, isAddKan] {
          SelectKan(isAddKan);
        }, View::MenuMode::Kan);
      }
      if (players[Model::House::Self].GetPaiCount(Model::Pai::North) && !gameView.NotSquealButtonIsToggle()) {
        gameView.SetMenuMode([this, isAddKan] {
          SelectNorth(isAddKan);
        }, View::MenuMode::EraseNorth);
      }
    }
    sequence = [] {};
    gameView.SetSelectMode([this, isAddKan](int i) {
      SelfDiscardPai(i, isAddKan);
    });
  }

  void CheckSelfSquealHand() {
    sequence = [] {};
    gameView.SetSelectMode([this](int i) {
      SelfDiscardPai(i, false);
    });
  }

  void Pon(Model::House house, Model::Pai pai) {
    players.Pon(Model::House::Self, house, pai);
    sequence = [this] {CheckSelfSquealHand();};
    gameView.SetWaitMode();
  }

  void LightKan(Model::House house, Model::Pai pai) {
    const auto point = players.LightKan(Model::House::Self, house, pai, field);
    sequence = [this, point] {CheckSelfHand(true, point);};
    gameView.SetWaitMode();
  }

  void SelectReachKan() {
    players.AllDeleteFirst();
    field.AddDora();
    sequence = [this] {CheckSelfReachHand(players[Model::House::Self].DarkKan(players[Model::House::Self].GetTumo(), field));};
    gameView.SetWaitMode();
  }

  void ThroughSqueal(Model::House house, Model::Pai pai) {
    // パスしたことを牌を切った家に送信
    if (pai != Model::Pai::Invalid) {
      players[Model::House::Self].SetFuritenPai(pai);
    }
    if (house == Model::House::Up) {
      sequence = [this] {SelfTumo();};
    } else {
      sequence = [this] {UpHouseTumoWait();};
    }
  }

  void CheckSelfSqueal(Model::House house, Model::Pai pai) {
    const bool isPonEnable = !players[Model::House::Self].GetPlayerState().IsReachTenpai() && players[Model::House::Self].IsPonEnable(pai)  && !gameView.NotSquealButtonIsToggle();
    const bool isKanEnable = !players[Model::House::Self].GetPlayerState().IsReachTenpai() && players[Model::House::Self].IsLightKanEnable(pai)  && !gameView.NotSquealButtonIsToggle();
    const auto tiList = !players[Model::House::Self].GetPlayerState().IsReachTenpai() && house == Model::House::Up && !gameView.NotSquealButtonIsToggle() ?
                        players[Model::House::Self].GetTiCandidate(pai) : std::vector<Model::TiPair>{};
    if (players[Model::House::Self].GetPlayerState().IsOpen() && !players[house].GetPlayerState().IsReachTenpai()) {
      players[Model::House::Self].SetOpenRon();
    }
    const auto point = players[Model::House::Self].Ron(pai, field);
    players[Model::House::Self].ResetOpenRon();
    if (point.GetHan()) {
      gameView.SetMenuMode([this, point, house] {
        SelectRon(point, house);
      }, View::MenuMode::Ron);
    }
    if (isPonEnable) {
      gameView.SetMenuMode([this, pai, house] {
        Pon(house, pai);
      }, View::MenuMode::Pon);
    }
    if (isKanEnable) {
      gameView.SetMenuMode([this, pai, house] {
        LightKan(house, pai);
      }, View::MenuMode::Kan);
    }
    if (!tiList.empty()) {
      gameView.SetMenuMode([this, pai, tiList] {
        SelectTi(pai, tiList);
      }, View::MenuMode::Ti);
    }
    if (isPonEnable || isKanEnable || !tiList.empty() || point.GetHan()) {
      gameView.SetMenuMode([this, house, pai] {
        ThroughSqueal(house, pai);
        gameView.SetWaitMode();
      }, View::MenuMode::Cancel);
      sequence = [] {};
    } else {
      ThroughSqueal(house, pai);
    }
  }

  void SelectNorth(bool isAddKan) {
    players.AllDeleteFirst();
    gameView.SetWaitMode();
    sequence = [this] {
      if (players[Model::House::Self].GetPlayerState().IsReachTenpai()) {
        CheckSelfReachHand(players[Model::House::Self].EraseNorth(field));
      } else {
        CheckSelfHand(false, players[Model::House::Self].EraseNorth(field));
      }
    };
    if (isAddKan) {
      field.AddDora();
    }
  }

  void SelfTumo() {
    sequence = [this] {
      if (players[Model::House::Self].GetPlayerState().IsReachTenpai()) {
        CheckSelfReachHand(players[Model::House::Self].Tumo(field));
      } else {
        CheckSelfHand(false, players[Model::House::Self].Tumo(field));
      }
    };
  }

  void GotoNextGame() {
    gameView.SetStart();
    if (players.HasFlyHouse()) {
      field.NextGameInit();
      GameStart();
    // TODO:テンパイ続行
    } else {
      if (field.IsOLas()) {
        field.NextGameInit();
        GameStart();
      } else {
        field.NextSetInit();
        players.NextSetInit(field);
      }
    }
    if (players[Model::House::Up].IsParent()) {
      sequence = [this] {UpHouseTumoWait();};
    } else if (players[Model::House::Self].IsParent()) {
      sequence = [this] {SelfTumo();};
    } else if (players[Model::House::Down].IsParent()) {
      sequence = [this] {DownHouseTumoWait();};
    } else {
      assert(false);
    }
  }

  void FlowSet() {
    // TODO:形式聴牌に対応
    players.SetFlowPoint();
    gameView.SetFlowSet([this] {
      GotoNextGame();
    }, {{0, 0, 0}});
    sequence = [] {};
  }

  Input input;
  Model::Field field;
  Model::Players players;
  View::Game gameView;
  Utility::PaiImage paiImage;
  std::function<void ()> sequence;
};
}}

#endif
