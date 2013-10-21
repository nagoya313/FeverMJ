#ifndef FEVERMJ_VIEW_STATUS_HPP_
#define FEVERMJ_VIEW_STATUS_HPP_
#include <functional>
#include <DxLib.h>
#include <boost/noncopyable.hpp>
#include "../model/field.hpp"
#include "../model/player.hpp"
#include "../model/role.hpp"

namespace FeverMJ { namespace View {
class Status : boost::noncopyable {
 public:
  void Draw(const Model::Field &field, const Model::Players &players) {
    if (!isResult) {
      DrawStatus(field, players);
    }
  }

  void SetResult(bool r) {
    isResult = r;
  }

 private:
  void DrawStatus(const Model::Field &field, const Model::Players &players) {
    if (field.IsAllLast()) {
      DrawString(328, 56, "オーラス", GetColor(255, 255, 255));
    }
    // 供託棒も
    DrawFormatString(328, 88, GetColor(255, 255, 255), "東%d局", field.GetSetCount());
    DrawFormatString(328, 120, GetColor(255, 255, 255), "残り山:%d", field.GetTumoMountainSize());
    DrawFormatString(168, 120, GetColor(255, 255, 255), "%d", players[Model::House::Up].GetPoint());
    DrawFormatString(376, 152, GetColor(255, 255, 255), "%d", players[Model::House::Self].GetPoint());
    DrawFormatString(540, 120, GetColor(255, 255, 255), "%d", players[Model::House::Down].GetPoint());
  }

  bool isResult = false;
};
}}

#endif
