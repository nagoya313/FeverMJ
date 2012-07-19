#ifndef FEVERMJ_VIEW_STATUS_HPP_
#define FEVERMJ_VIEW_STATUS_HPP_
#include <functional>
#include <DxLib.h>
#include <boost/noncopyable.hpp>
#include "button.hpp"
#include "../model/field.hpp"
#include "../model/house.hpp"
#include "../model/player.hpp"
#include "../model/point.hpp"
#include "../model/role.hpp"

namespace FeverMJ { namespace View {
class Status : boost::noncopyable {
 public:
  void Draw(const Model::Field &field, const Model::Players &players, const Controller::Input &input) {
    if (isFlowSet) {
      DrawFlowSet(players, input);
    } else if (isGoal) {
      DrawGoal(players, input);
    } else {
      DrawStatus(field, players);
    }
  }

  template <typename Action>
  void SetFlowSet(Action action, const std::array<int, 3> &points) {
    isFlowSet = true;
    nextGameButton.SetAction(action);
    nextGameButton.SetHide(false);
    variationPoints = points;
  }

  template <typename Action>
  void SetSelectTumo(Action action, const Model::Point &p, const std::array<int, 3> &points) {
    isGoal = true;
    isTumo = true;
    nextGameButton.SetAction(action);
    nextGameButton.SetHide(false);
    variationPoints = points;
    point = p;
  }

  template <typename Action>
  void SetSelectRon(Action action, const Model::Point &p, const std::array<int, 3> &points) {
    isGoal = true;
    isTumo = false;
    nextGameButton.SetAction(action);
    nextGameButton.SetHide(false);
    variationPoints = points;
    point = p;
  }

  void SetStart() {
    isFlowSet = false;
    isGoal = false;
    nextGameButton.SetHide(true);
  }

 private:
  void DrawStatus(const Model::Field &field, const Model::Players &players) {
    if (field.IsOLas()) {
      DrawString(328, 56, "オーラス", GetColor(255, 255, 255));
    }
    DrawFormatString(328, 88, GetColor(255, 255, 255), "東%d局", field.GetSetCount());
    DrawFormatString(328, 120, GetColor(255, 255, 255), "残り山:%d", field.GetTumoMountainSize());
    DrawFormatString(128, 120, GetColor(255, 255, 255), "%d", players[Model::House::Up].GetPoint());
    DrawFormatString(376, 152, GetColor(255, 255, 255), "%d", players[Model::House::Self].GetPoint());
    DrawFormatString(580, 120, GetColor(255, 255, 255), "%d", players[Model::House::Down].GetPoint());
  }

  static DWORD GetPointColor(int point) {
    return point >= 0 ? GetColor(255, 255, 255) : GetColor(255, 0, 0);
  }

  void DrawGoal(const Model::Players &players, const Controller::Input &input) {
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
    DrawBox(0, 0, 800, 600, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    DrawString(380, 8, isTumo ? "ツモ" : "ロン", GetColor(255, 255, 255)); 
    DrawFormatString(32, 112, GetColor(255, 255, 255), "%d符 %d翻", point.GetHu(), point.GetHan());
    DrawFormatString(280, 120, GetColor(255, 255, 255),
                     "上家：%d→%d", players[Model::House::Up].GetPoint() - variationPoints[0], players[Model::House::Up].GetPoint());
    DrawFormatString(280, 156, GetPointColor(variationPoints[0]), "%d", variationPoints[0]);
    DrawFormatString(280, 192, GetColor(255, 255, 255),
                     "自分：%d→%d", players[Model::House::Self].GetPoint() - variationPoints[1], players[Model::House::Self].GetPoint());
    DrawFormatString(280, 228, GetPointColor(variationPoints[1]), "%d", variationPoints[1]);
    DrawFormatString(280, 264, GetColor(255, 255, 255),
                     "下家：%d→%d", players[Model::House::Down].GetPoint() - variationPoints[2], players[Model::House::Down].GetPoint());
    DrawFormatString(280, 300, GetPointColor(variationPoints[2]), "%d", variationPoints[2]);
    int y = 0;
    const auto role = point.GetRole();
    for (std::uint64_t i = 0LL; i < 52LL; ++i) {
      if (role & (1LL << i)) {
        if (i == 51L) {
          DrawFormatString(32, 144 + y * 32, GetColor(255, 255, 255), "ドラ　%d\n", point.GetDoraCount());
        } else {
          DrawFormatString(32, 144 + y * 32, GetColor(255, 255, 255), "%s\n", Model::roleString[i]);
        }
        ++y;
      }
    }
    nextGameButton.Update(input);
  }

  void DrawFlowSet(const Model::Players &players, const Controller::Input &input) {
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
    DrawBox(80, 64, 720, 500, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    DrawString(88, 72, "流局", GetColor(255, 255, 255)); 
    DrawFormatString(280, 120, GetColor(255, 255, 255),
                     "上家：%d→%d", players[Model::House::Up].GetPoint() - variationPoints[0], players[Model::House::Up].GetPoint());
    DrawFormatString(280, 156, GetPointColor(variationPoints[0]), "%d", variationPoints[0]);
    DrawFormatString(280, 192, GetColor(255, 255, 255),
                     "自分：%d→%d", players[Model::House::Self].GetPoint() - variationPoints[1], players[Model::House::Self].GetPoint());
    DrawFormatString(280, 228, GetPointColor(variationPoints[1]), "%d", variationPoints[1]);
    DrawFormatString(280, 264, GetColor(255, 255, 255),
                     "下家：%d→%d", players[Model::House::Down].GetPoint() - variationPoints[2], players[Model::House::Down].GetPoint());
    DrawFormatString(280, 300, GetPointColor(variationPoints[2]), "%d", variationPoints[2]);
    nextGameButton.Update(input);
  }

  std::array<int, 3> variationPoints;
  bool isFlowSet = false;
  bool isGoal = false;
  bool isTumo = false;
  Model::Point point;
  Button nextGameButton{550, 400, "次の局"};
};
}}

#endif
