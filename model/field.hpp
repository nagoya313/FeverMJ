#ifndef FEVERMJ_MODEL_FIELD_HPP_
#define FEVERMJ_MODEL_FIELD_HPP_
#include <cassert>
#include <cstdint>
#include <algorithm>
#include <deque>
#include <vector>
#include <DxLib.h>
#include "house.hpp"
#include "one_mountain.hpp"
#include "pai.hpp"
#include "tumo_moutain.hpp"

namespace FeverMJ { namespace Model {
class Field {
 public:
  void FirstGameInit(House parent) {
    setCount = 0;
    bar = 0;
    firstParentHouse = parent;
    Init();
  }

  void AddReachBar() {
    bar += 1000;
  }

  void RemoveReachBar() {
    bar -= 1000;
  }

  int ReleaseReachBar() {
    int temp = bar;
    bar = 0;
    return temp;
  }

  void NextGameInit() {
    setCount = 0;
    firstParentHouse = GetDownHouse(firstParentHouse);
    Init();
  }

  void ContinueSetInit() {
    Init();
  }

  void NextSetInit() {
    ++setCount;
    Init();
  }

  bool IsOLas() const {
    return setCount == 2;
  }

  int GetSetCount() const {
    return setCount + 1;
  }
  
  House GetFirstParentHouse() const {
    return firstParentHouse;
  }

  House GetParentHouse() const {
    House house = GetFirstParentHouse();
    for (int i = 0; i < GetSetCount() - 1; ++i) {
      house = GetDownHouse(house);
    }
    return house;
  }

  House GetBreakHouse() const {
    return breakHouse;
  }

  HandVector GetFirstPais() {
    return tumoMountain.PopFirstPais();
  }

  Pai GetTumoPai() {
    return tumoMountain.PopTumoPai();
  }

  Pai GetRinsyan() {
    return oneMountain.PopRinsyanPai();
  }

  int GetTumoMountainSize() const {
    return tumoMountain.GetMountainSize();
  }

  bool IsPaiEmpty() const {
    return !GetTumoMountainSize();
  }

  void AddDora() {
    tumoMountain.PopBackPai();
    oneMountain.AddDora();
  }

  int GetDoraCount() const {
    return oneMountain.GetDoraCount();
  }

  boost::optional<Pai> GetDisplayDora(int i) const {
    return oneMountain.GetDisplayDora(i);
  }

  DoraVector GetDoraList(bool withReverse) const {
    return oneMountain.GetDoraList(withReverse);
  }

 private:
  void Init() {
    breakHouse = SelectBreakHouse();
    tumoMountain.Init();
    oneMountain.Init(tumoMountain.PopOnePais());
  }

  House SelectBreakHouse() const {
    switch ((GetRand(10) + 2) % 3) {
      case 0:
        return GetParentHouse();
      case 1:
        return GetDownHouse(GetParentHouse());
      case 2:
        return GetUpHouse(GetParentHouse());
      default:
        assert(false);
        return GetParentHouse();
    }
  }

  int setCount;
  int doraCount;
  int bar;
  House firstParentHouse;
  House breakHouse;
  TumoMountain tumoMountain;
  OneMountain oneMountain;
};
}}

#endif
