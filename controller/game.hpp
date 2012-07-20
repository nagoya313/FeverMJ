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
    sequence = [this, pai] {CheckSelfSquealFromUpHouse(pai);};
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
    sequence = [this, pai] {CheckSelfSquealFromDownHouse(pai);};
  }

  void SelectKan(bool isAddKan) {
    players.AllDeleteFirst();
    gameView.SetWaitMode();
    gameView.SetSelectKanMode([this] (Model::Pai darkKanPai, Model::Pai addKanPai) {
      if (darkKanPai != Model::Pai::Invalid) {
        field.AddDora();
        sequence = [this, darkKanPai] {CheckSelfHand(false, players[Model::House::Self].DarkKan(darkKanPai, field));};
      } else {
        sequence = [this, addKanPai] {CheckSelfHand(true, players[Model::House::Self].AddKan(addKanPai, field));};
      }        
      gameView.ResetSelectKanMode();
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
      players[Model::House::Self].Ti(pai, tiPair);
      sequence = [this] {CheckSelfSquealHand();};
      gameView.ResetSelectTiMode();
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
    gameView.SetSelectRon([this] {
      GotoNextGame();
    }, point, players.GetRonPoint(Model::House::Self, house, point, field));
    sequence = [] {};
  }

  void SelectTumo(const Model::Point &point) {
    gameView.SetWaitMode();
    gameView.SetSelectTumo([this] {
      GotoNextGame();
    }, point, players.GetTumoPoint(Model::House::Self, point, field));
    sequence = [] {};
  }

  void CheckSelfHand(bool isAddKan, const Model::Point &point) {
    if (point.GetHan()) {
      gameView.SetMenuMode([this, point] {
        SelectTumo(point);
      }, View::MenuMode::Tumo);
    }
    if (!field.IsPaiEmpty()) {
      if (field.GetDoraCount() <= 5 && players[Model::House::Self].IsDarkOrAddKanenable()) {
        gameView.SetMenuMode([this, isAddKan] {
          SelectKan(isAddKan);
        }, View::MenuMode::Kan);
      }
      if (players[Model::House::Self].GetPaiCount(Model::Pai::North)) {
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
    players.AllDeleteFirst();
    players[house].PopBackRiver();
    players[Model::House::Self].Pon(house, pai);
    sequence = [this] {CheckSelfSquealHand();};
    gameView.SetWaitMode();
  }

  void LightKan(Model::House house, Model::Pai pai) {
    players.AllDeleteFirst();
    players[house].PopBackRiver();
    sequence = [this, house, pai] {CheckSelfHand(true, players[Model::House::Self].LightKan(house, pai, field));};
    gameView.SetWaitMode();
  }

  void ThroughSqueal(Model::House house) {
    // パスしたことを牌を切った家に送信
    if (house == Model::House::Up) {
      sequence = [this] {SelfTumo();};
    } else {
      sequence = [this] {UpHouseTumoWait();};
    }
  }
  
  void CheckSelfSquealFromDownHouse(Model::Pai pai) {
    const bool isPonEnable = players[Model::House::Self].IsPonEnable(pai);
    const bool isKanEnable = players[Model::House::Self].IsLightKanEnable(pai);
    const auto point = players[Model::House::Self].Ron(pai, field);
    if (point.GetHan()) {
      gameView.SetMenuMode([this, point] {
        SelectRon(point, Model::House::Down);
      }, View::MenuMode::Ron);
    }
    if (isPonEnable) {
      gameView.SetMenuMode([this, pai] {
        Pon(Model::House::Down, pai);
      }, View::MenuMode::Pon);
    }
    if (isKanEnable) {
      gameView.SetMenuMode([this, pai] {
        LightKan(Model::House::Down, pai);
      }, View::MenuMode::Kan);
    }
    if (isPonEnable || isKanEnable || point.GetHan()) {
      gameView.SetMenuMode([this] {
        ThroughSqueal(Model::House::Down);
        gameView.SetWaitMode();
      }, View::MenuMode::Cancel);
      sequence = [] {};
    } else {
      ThroughSqueal(Model::House::Down);
    }
  }

  void CheckSelfSquealFromUpHouse(Model::Pai pai) {
    const bool isPonEnable = players[Model::House::Self].IsPonEnable(pai);
    const bool isKanEnable = players[Model::House::Self].IsLightKanEnable(pai);
    const auto tiList = players[Model::House::Self].GetTiCandidate(pai);
    const auto point = players[Model::House::Self].Ron(pai, field);
    if (point.GetHan()) {
      gameView.SetMenuMode([this, point] {
        SelectRon(point, Model::House::Up);
      }, View::MenuMode::Ron);
    }
    if (isPonEnable) {
      gameView.SetMenuMode([this, pai] {
        Pon(Model::House::Up, pai);
      }, View::MenuMode::Pon);
    }
    if (isKanEnable) {
      gameView.SetMenuMode([this, pai] {
        LightKan(Model::House::Up, pai);
      }, View::MenuMode::Kan);
    }
    if (!tiList.empty()) {
      gameView.SetMenuMode([this, pai, tiList] {
        SelectTi(pai, tiList);
      }, View::MenuMode::Ti);
    }
    if (isPonEnable || isKanEnable || !tiList.empty() || point.GetHan()) {
      gameView.SetMenuMode([this] {
        ThroughSqueal(Model::House::Up);
        gameView.SetWaitMode();
      }, View::MenuMode::Cancel);
      sequence = [] {};
    } else {
      ThroughSqueal(Model::House::Up);
    }
  }

  void SelectNorth(bool isAddKan) {
    players.AllDeleteFirst();
    gameView.SetWaitMode();
    sequence = [this] {CheckSelfHand(false, players[Model::House::Self].EraseNorth(field));};
    if (isAddKan) {
      field.AddDora();
    }
  }

  void SelfTumo() {
    sequence = [this] {CheckSelfHand(false, players[Model::House::Self].Tumo(field));};
  }

  void GotoNextGame() {
    gameView.SetStart();
    if (players[Model::House::Up].GetPoint() < 0 || 
        players[Model::House::Self].GetPoint() < 0 ||
        players[Model::House::Down].GetPoint() < 0) {
      field.NextGameInit();
      GameStart();
    // TODO:テンパイ続行
    /*
    } else if (players[1].IsOya() && players[1].IsTenpai()) {
      // 連チャン初期化
      field.RenTyanInit();
      players[0].InitOnaziKaze(field.GettHaiPai());
      players[1].InitOnaziKaze(field.GettHaiPai());
      players[2].InitOnaziKaze(field.GettHaiPai());
      */
    } else {
      if (field.IsOLas()) {
        field.NextGameInit();
        GameStart();
      } else {
        field.NextSetInit();
        players[Model::House::Up].NextSetInit(field.GetFirstPais());
        players[Model::House::Self].NextSetInit(field.GetFirstPais());
        players[Model::House::Down].NextSetInit(field.GetFirstPais());
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
