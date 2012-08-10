#ifndef FEVERMJ_MODEL_RESPONSE_HPP_
#define FEVERMJ_MODEL_RESPONSE_HPP_
#include <functional>
#include "house.hpp"

namespace FeverMJ { namespace Model {
enum class Response {
  Error,
  Wait,
  Pass,
  PaiCut,
  EraseNorth,
  DarkKan,
  AddKan,
  Ti,
  Pon,
  LightKan,
  Reach,
  OpenReach,
  FeverReach,
  DoubleFeverReach,
  Ron,
  Tumo,
};

enum class SquealResponse {
  Error,
  Wait,
  Success,
  Interrupt
};

struct ResponseData {
  House house;
  Response data;
  int value0;
  int value1;
};

struct ResponsePair {
  ResponseData up;
  ResponseData down;
};

constexpr ResponsePair responseWaitPair = {
  {House::Up, Response::Wait, -1}, {House::Down, Response::Wait, -1}
};

inline
std::pair<ResponseData, ResponseData> GetHighPriorityResponse(House parent, const ResponseData &lhs, const ResponseData &rhs) {
  if (lhs.data == rhs.data) {
    return GetHouseDistance(parent, lhs.house) < GetHouseDistance(parent, rhs.house) ?
           std::make_pair(lhs, rhs) : std::make_pair(rhs, lhs);
  } else {
    return lhs.data > rhs.data ? std::make_pair(lhs, rhs) : std::make_pair(rhs, lhs);
  }
}
}}

#endif
