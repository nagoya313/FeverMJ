#ifndef FEVERMJ_MODEL_PLAYER_STATE_HPP_
#define FEVERMJ_MODEL_PLAYER_STATE_HPP_
#include <cstdint>
#include "wind.hpp"

namespace FeverMJ { namespace Model {
class PlayerState {
 public:
  void Init(Wind wind) {
    state = Menzen | FirstTumo;
    selfWind = wind;
  }

  Wind GetSelfWind() const {
    return selfWind;
  }
 
  void SetReach() {
    state |= Reach | ReachFirstTumo;
  }

  void SetDoubleReach() {
    state |= DoubleReach | ReachFirstTumo;
  }

  void SetOpen() {
    state |= Open;
  }

  void SetFever() {
    state |= Fever;
  }

  void SetDoubleFever() {
    state |= DoubleFever;
  }

  void SetOpenRon() {
    state |= OpenRon;
  }

  void ResetOpenRon() {
    state &= ~OpenRon;
  }

  void SetFirst() {
    state |= ReachFirstTumo;
  }

  void Squeal() {
    state &= ~Menzen;
  }
  
  void DeleteFirst() {
    state &= ~(FirstTumo | RinsyanKaiho | ReachFirstTumo);
  }
  
  void SetFuriten() {
    state |= Furiten;
  }
  
  void ResetFuriten() {
    state &= ~Furiten;
  }

  void SetRinsyanKaiho() {
    state |= RinsyanKaiho;
  }
  
  bool IsMenzen() const {
    return state & Menzen;
  }

  bool IsReachTenpai() const {
    return (state & Reach) || (state & DoubleReach);
  }
  
  bool IsReach() const {
    return state & Reach;
  }
  
  bool IsDoubleReach() const {
    return state & DoubleReach;
  }
  
  bool IsOpen() const {
    return state & Open;
  }
  
  bool IsFever() const {
    return state & Fever;
  }
  
  bool IsDoubleFever() const {
    return state & DoubleFever;
  }
  
  bool IsTyankan() const {
    return state & Tyankan;
  }
  
  bool IsRinsyanKaiho() const {
    return state & RinsyanKaiho;
  }

  bool IsFirstTumo() const {
    return state & FirstTumo;
  }

  bool IsReachFirstTumo() const {
    return state & ReachFirstTumo;
  }

  bool IsFuriten() const {
    return state & Furiten;
  }

  bool IsOpenRon() const {
    return state & OpenRon;
  }
 
 private:
  enum : std::uint32_t {
    Reach = 1 << 0,
    DoubleReach = 1 << 1,
    Fever = 1 << 2,
    DoubleFever = 1 << 3,
    RinsyanKaiho = 1 << 4,
    Tyankan = 1 << 5,
    Furiten = 1 << 6,
    FirstTumo = 1 << 7,
    Menzen = 1 << 8,
    Open = 1 << 9,
    ReachFirstTumo = 1 << 10,
    OpenRon = 1 << 11
  };
  
  std::uint32_t state;
  Wind selfWind;
};
}}

#endif
