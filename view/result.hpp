#ifndef FEVERMJ_VIEW_RESULT_HPP_
#define FEVERMJ_VIEW_RESULT_HPP_
#include <functional>
#include <DxLib.h>
#include <boost/noncopyable.hpp>
#include "button.hpp"
#include "../model/field.hpp"
#include "../model/house.hpp"
#include "../model/player.hpp"
#include "../model/point.hpp"
#include "../model/role.hpp"
#include "../utility/pai_image.hpp"

namespace FeverMJ { namespace View {
class Result : boost::noncopyable {
 public:
  void Draw(const Model::Field &field,
            const Model::Players &players,
            const Controller::Input &input,
            const Utility::PaiImage &paiImage) {
    if (isFlowSet) {
      DrawFlowSet(players, input);
    } else if (isGoal) {
      DrawGoal(field, players, input, paiImage);
    }
  }

  template <typename Action>
  void SetFlowSet(Action action) {
    isFlowSet = true;
    nextGameButton.SetAction(action);
    nextGameButton.SetHide(false);
  }

  template <typename Action>
  void SetSelectTumo(Action action, const Model::Point &p, Model::House g) {
    isGoal = true;
    isTumo = true;
    goalHouse = g;
    nextGameButton.SetAction(action);
    nextGameButton.SetHide(false);
    point = p;
  }

  template <typename Action>
  void SetSelectRon(Action action, const Model::Point &p, Model::House g) {
    isGoal = true;
    isTumo = false;
    goalHouse = g;
    nextGameButton.SetAction(action);
    nextGameButton.SetHide(false);
    point = p;
  }

  void SetStart() {
    isFlowSet = false;
    isGoal = false;
    nextGameButton.SetHide(true);
  }

 private:
  static DWORD GetPointColor(int point) {
    return point >= 0 ? GetColor(255, 255, 255) : GetColor(255, 0, 0);
  }

  void DrawGoalHand(const Model::Player &player, const Utility::PaiImage &paiImage) {
    const int size = player.GetHandSize();
    for (int i = 0; i < size; ++i) {
      DrawGraph(80 + 33 * i, 48, paiImage.GetUpHandle(player.GetHandPai(i) % Model::squealOffset), TRUE);
    }
    DrawGraph(105 + 33 * size, 48, paiImage.GetUpHandle(player.GetGoalPai()), TRUE);
    int pos = 738;
    const int ssize = player.GetSquealSize();
    for (int i = 0; i < ssize; ++i) {
      const int pai = player.GetSquealImageHandle(i);
      pos -= pai < Model::squealOffset ? 33 : 44;
      const int image = pai < 0 ?
                        paiImage.GetBackHandle(0) : pai < Model::squealOffset ?
                        paiImage.GetUpHandle(pai) : paiImage.GetRightHandle(pai % Model::squealOffset);
      if (pai >= Model::squealOffset * 2) {
        DrawGraph(pos, 26, image, TRUE);
      }
      DrawGraph(pos, pai < Model::squealOffset ? 48 : 58, image, TRUE);
    }
  }

  void DrawRole(const Model::Players &players) {
    DrawFormatString(32, 120, GetColor(255, 255, 255), "%d符 %d翻", point.GetHu(), point.GetHan());
    DrawPoint(players);
    int y = 0;
    const auto role = point.GetRole();
    for (std::uint64_t i = 0LL; i < 53LL; ++i) {
      if (role & (1LL << i)) {
        if (i == 52L) {
          DrawFormatString(32, 152 + y * 32, GetColor(255, 255, 255), "ドラ　%d\n", point.GetDoraCount());
        } else {
          DrawFormatString(32, 152 + y * 32, GetColor(255, 255, 255), "%s\n", Model::roleString[i]);
        }
        ++y;
      }
    }
  }

  void DrawDora(const Model::Field &field, const Model::Player &player, const Utility::PaiImage &paiImage) {
    DrawString(280, 120, "ドラ", GetColor(255, 255, 255));
    const auto doraList = field.GetDoraList(player.GetPlayerState().IsReachTenpai());
    const int size = doraList.size() - 3;
    for (int i = 0; i < size; ++i) {
      DrawGraph(372 + 33 * i, 120, paiImage.GetUpHandle(doraList[i + 3]), TRUE);
    }
  }

  void DrawGoal(const Model::Field &field,
                const Model::Players &players,
                const Controller::Input &input,
                const Utility::PaiImage &paiImage) {
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
    DrawBox(0, 0, 800, 600, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    DrawString(380, 8, isTumo ? "ツモ" : "ロン", GetColor(255, 255, 255));
    DrawGoalHand(players[goalHouse], paiImage);
    DrawDora(field, players[goalHouse], paiImage);
    DrawRole(players);
    nextGameButton.Update(input);
  }

  void DrawFlowSet(const Model::Players &players, const Controller::Input &input) {
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
    DrawBox(80, 64, 720, 500, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    DrawString(88, 72, "流局", GetColor(255, 255, 255)); 
    DrawPoint(players);
    nextGameButton.Update(input);
  }

  void DrawPoint(const Model::Players &players) {
    DrawFormatString(280, 188, GetColor(255, 255, 255),
                     "上家：%d→%d", players[Model::House::Up].GetBeforePoint(), players[Model::House::Up].GetPoint());
    DrawFormatString(280, 224, GetPointColor(players[Model::House::Up].GetDiffPoint()), "%d", players[Model::House::Up].GetDiffPoint());
    DrawFormatString(280, 260, GetColor(255, 255, 255),
                     "自分：%d→%d", players[Model::House::Self].GetBeforePoint(), players[Model::House::Self].GetPoint());
    DrawFormatString(280, 296, GetPointColor(players[Model::House::Self].GetDiffPoint()), "%d", players[Model::House::Self].GetDiffPoint());
    DrawFormatString(280, 330, GetColor(255, 255, 255),
                     "下家：%d→%d", players[Model::House::Down].GetBeforePoint(), players[Model::House::Down].GetPoint());
    DrawFormatString(280, 368, GetPointColor(players[Model::House::Down].GetDiffPoint()), "%d", players[Model::House::Down].GetDiffPoint());
  }

  std::array<int, 3> variationPoints;
  bool isFlowSet = false;
  bool isGoal = false;
  bool isTumo = false;
  Model::House goalHouse;
  Model::Point point;
  Button nextGameButton{550, 420, "次の局"};
};
}}

#endif
