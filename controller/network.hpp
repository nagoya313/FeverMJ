#ifndef FEVERMJ_CONTROLLER_NETWORK_HPP_
#define FEVERMJ_CONTROLLER_NETWORK_HPP_
#include <boost/noncopyable.hpp>
#include "../model/house.hpp"
#include "../model/pai.hpp"
#include "../model/reach.hpp"
#include "../model/response.hpp"
#include "../utility/network.hpp"

namespace FeverMJ { namespace Controller {
class NetWork : boost::noncopyable {
 public:
  explicit NetWork(Utility::NetHandleArray &&handles) : netHandles(std::move(handles)) {}

  Model::ResponseData GetTernResponse(Model::House house) const {
    if (!netHandles[GetHouseIndex(house)]) {
      return {house, Model::Response::PaiCut, 14, -1};
    } else if (const auto recv = netHandles[GetHouseIndex(house)].Receive(7)) {
      if (const auto data = Utility::GetResponceData(*recv)) {
        if (data->func == "picut") {
          return {house, Model::Response::PaiCut, data->value, -1};
        } else if (data->func == "ersnt") {
          return {house, Model::Response::EraseNorth, -1, -1};
        } else if (data->func == "dlkan") {
          return {house, Model::Response::DarkKan, data->value, -1};
        } else if (data->func == "adkan") {
          return {house, Model::Response::AddKan, data->value, -1}; 
        } else if (data->func == "reach") {
          return {house, Model::Response::Reach, data->value, -1};
        } else if (data->func == "opnrc") {
          return {house, Model::Response::OpenReach, data->value, -1};
        } else if (data->func == "fevrc") {
          return {house, Model::Response::FeverReach, data->value, -1};
        } else if (data->func == "dfvrc") {
          return {house, Model::Response::DoubleFeverReach, data->value, -1};
        } else if (data->func == "tumog") {
          return {house, Model::Response::Tumo, -1, -1};
        } else {
          throw Utility::NetWorkError(data->func);
        }
      } else {
        throw Utility::NetWorkError(data->func);
      }
    }
    return {house, Model::Response::Wait, -1};
  }

  Model::ResponseData GetSquealResponse(Model::House house) const {
    if (!netHandles[GetHouseIndex(house)]) {
      return {house, Model::Response::Pass, -1, -1};
    } else if (const auto recv = netHandles[GetHouseIndex(house)].Receive(7)) {
      if (const auto data = Utility::GetSquealResponceData(*recv)) {
        if (data->func == "ti") {
          return {house, Model::Response::Ti, data->value0, data->value1};
        } else if (data->func == "pn") {
          return {house, Model::Response::Pon, -1, -1};
        }  else if (data->func == "kn") {
          return {house, Model::Response::LightKan, -1, -1};
        } else if (data->func == "rn") {
          return {house, Model::Response::Ron, -1, -1};
        } else if (data->func == "ps") {
          return {house, Model::Response::Pass, -1, -1};
        } else {
          throw Utility::NetWorkError(data->func);
        }
      } else {
        throw Utility::NetWorkError(data->func);
      }
    }
    return {house, Model::Response::Wait, -1};
  }

  void SendTumoCut(Model::ReachState state) {
    switch (state) {
      case Model::ReachState::ReachStart:
        SendMessage("reach14");
        break;
      case Model::ReachState::OpenStart:
        SendMessage("opnrc14");
        break;
      case Model::ReachState::FeverStart:
        SendMessage("fevrc14");
        break;
      case Model::ReachState::DoubleFeverStart:
        SendMessage("dfvrc14");
        break;
      default:
        SendMessage("picut14");
        break;
    }
  }

  void SendHandCut(Model::ReachState state, int index) {
    switch (state) {
      case Model::ReachState::ReachStart:
        SendMessage(GetReachString(index));
        break;
      case Model::ReachState::OpenStart:
        SendMessage(GetOpenReachString(index));
        break;
      case Model::ReachState::FeverStart:
        SendMessage(GetFeverReachString(index));
        break;
      case Model::ReachState::DoubleFeverStart:
        SendMessage(GetDoubleFeverReachString(index));
        break;
      default:
        SendMessage(GetCutString(index));
        break;
    }
  }

  void SendDarkKan(Model::Pai pai) {
    SendMessage(GetDarkKanString(pai));
  }

  void SendAddKan(Model::Pai pai) {
    SendMessage(GetAddKanString(pai));
  }

  void SendEraseNorth() {
    SendMessage("ersnt00");
  }

  void SendTi(Model::House house, const Model::TiPair &ti) {
    SendMessage(house, GetTiString(ti));
  }

  void SendPon(Model::House house) {
    SendMessage(house, "pn00,00");
  }
  
  void SendLightKan(Model::House house) {
    SendMessage(house, "kn00,00");
  }

  void SendRon(Model::House house) {
    SendMessage(house, "rn00,00");
  }

  void SendPass(Model::House house) {
    SendMessage(house, "ps00,00");
  }

  void SendTumoGoal() {
    SendMessage("tumog00");
  }

 private:
  static
  Model::House GetHouseIndex(Model::House house) {
    return house == Model::House::Down ? static_cast<Model::House>(house - 1) : house;
  }

  static
  std::string GetCutString(int i) {
    std::stringstream index;
    index << std::setw(2) << std::setfill('0') << i;
    return "picut" + index.str();
  }

  static
  std::string GetReachString(int i) {
    std::stringstream index;
    index << std::setw(2) << std::setfill('0') << i;
    return "reach" + index.str();
  }

  static
  std::string GetOpenReachString(int i) {
    std::stringstream index;
    index << std::setw(2) << std::setfill('0') << i;
    return "opnrc" + index.str();
  }

  static
  std::string GetFeverReachString(int i) {
    std::stringstream index;
    index << std::setw(2) << std::setfill('0') << i;
    return "fevrc" + index.str();
  }

  static
  std::string GetDoubleFeverReachString(int i) {
    std::stringstream index;
    index << std::setw(2) << std::setfill('0') << i;
    return "dfvrc" + index.str();
  }

  static
  std::string GetDarkKanString(Model::Pai p) {
    std::stringstream pai;
    pai << std::setw(2) << std::setfill('0') << p;
    return "dlkan" + pai.str();
  }

  static
  std::string GetAddKanString(Model::Pai p) {
    std::stringstream pai;
    pai << std::setw(2) << std::setfill('0') << p;
    return "adkan" + pai.str();
  }

  static
  std::string GetTiString(const Model::TiPair &pair) {
    std::stringstream ti;
    ti << std::setw(2) << std::setfill('0') << pair.first << "," << std::setw(2) << std::setfill('0') << pair.second;
    return "ti" + ti.str();
  }

  void SendMessage(Model::House house, std::string message) {
    FEVERMJ_LOG("送信データ %s\n", message.c_str());
    if (netHandles[GetHouseIndex(house)]) {
      netHandles[GetHouseIndex(house)].Send(message);
    }
  }

  void SendMessage(std::string message) {
    FEVERMJ_LOG("送信データ %s\n", message.c_str());
    for (const auto &handle : netHandles) {
      if (handle) {
        handle.Send(message);
      }
    }
  }

  Utility::NetHandleArray netHandles;
};
}}

#endif
