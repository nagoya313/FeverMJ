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
  explicit Field(House parent) : firstParent(parent){
    Init();
  }
  
  void NextGameInit() {
    set = 0;
    firstParent = GetDownHouse(firstParent);
    Init();
  }

  void ContinueSetInit() {
    Init();
  }
  
  void NextSetInit() {
    ++set;
	parent = GetDownHouse(parent);
    Init();
  }

  void AddReachBar() {
    reachBar += 1000;
  }

  void RemoveReachBar() {
    reachBar -= 1000;
  }
  
  void ReleaseReachBar() {
    reachBar = 0;
  }
  
  int GetReachBarCount() const {
    return reachBar;
  }

  bool IsAllLast() const {
    return set == 2;
  }

  int GetSetCount() const {
    return set + 1;
  }
  
  House GetFirstParent() const {
    return firstParent;
  }

  House GetParent() const {
    return parent;
  }

  House GetWareme() const {
    return wareme;
  }

  HandVector GetFirstPais() {
    return yama.PopFirstPais();
  }

  Pai GetTumoPai() {
    return yama.PopTumoPai();
  }

  Pai GetRinsyan() {
    return wanpai.PopRinsyanPai();
  }

  int GetTumoMountainSize() const {
    return yama.GetMountainSize();
  }

  bool IsPaiEmpty() const {
    return !GetTumoMountainSize();
  }

  void AddDora() {
    yama.PopBackPai();
    wanpai.AddDora();
  }

  int GetDoraCount() const {
    return wanpai.GetDoraCount();
  }

  boost::optional<Pai> GetDisplayDora(int i) const {
    return wanpai.GetDisplayDora(i);
  }

  DoraVector GetDoraList(bool withReverse) const {
    return wanpai.GetDoraList(withReverse);
  }

 private:
  void Init() {
    wareme = SelectWareme();
    yama.Init();
    wanpai.Init(yama.PopOnePais());
  }

  House SelectWareme() const {
    switch ((GetRand(10) + 2) % 3) {
      case 0:
        return GetParent();
      case 1:
        return GetDownHouse(GetParent());
      case 2:
        return GetUpHouse(GetParent());
      default:
        assert(false);
        return GetParent();
    }
  }

  int set = 0;
  int reachBar = 0;
  House firstParent;
  House parent = firstParent;
  House wareme;
  TumoMountain yama;
  OneMountain wanpai;
};
}}

#endif
