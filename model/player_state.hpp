#ifndef FEVERMJ_MODEL_PLAYER_STATE_HPP_
#define FEVERMJ_MODEL_PLAYER_STATE_HPP_
#include <cstdint>

namespace FeverMJ { namespace Model {
class PlayerState {
 public:
  void InitParent() {
    state = Menzen | HeavenGoal;
  }
  
  void InitChild() {
    state = Menzen | GrandGoal | PersonGoal;
  }

  void Squeal() {
    state &= ~Menzen;
  }
  
  void DeleteFirst() {
    state &= ~(HeavenGoal | GrandGoal | PersonGoal | First | RinsyanKaiho);
  }
  
  void SetFuriten() {
    state |= Furiten;
  }
  
  void ResetFuriten() {
    state &= ~Furiten;
  }
  
  void SetTumoGoal() {
    state |= TumoGoal;
  }

  void SetRinsyanKaiho() {
    state |= RinsyanKaiho;
  }
  
  bool IsMenzen() const {
    return state & Menzen;
  }
  
  bool IsSilentTenpai() const {
    return !((state & Reach) || (state & DoubleReach));
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
  
  bool IsHeavenGoal() const {
    return state & HeavenGoal;
  }
  
  bool IsGrandGoal() const {
    return state & GrandGoal;
  }
  
  bool IsPersonGoal() const {
    return state & PersonGoal;
  }
  
  bool IsTumoGoal() const {
    return state & TumoGoal;
  }
  
  bool IsTyankan() const {
    return state & Tyankan;
  }
  
  bool IsRinsyanKaiho() const {
    return state & RinsyanKaiho;
  }

  bool IsFirst() const {
    return state & First;
  }

  bool IsFuriten() const {
    return state & Furiten;
  }
 
 private:
  enum {
    Reach = 1 << 0,
    DoubleReach = 1 << 1,
    Fever = 1 << 2,
    DoubleFever = 1 << 3,
    HeavenGoal = 1 << 4,
    GrandGoal = 1 << 5,
    PersonGoal = 1 << 6,
    RinsyanKaiho = 1 << 7,
    Tyankan = 1 << 8,
    Furiten = 1 << 9,
    First = 1 << 10,
    Menzen = 1 << 11,
    Open = 1 << 12,
    TumoGoal = 1 << 13
  };
  
  std::uint32_t state;
};
}}

#endif
