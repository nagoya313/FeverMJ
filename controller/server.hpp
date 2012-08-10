#ifndef FEVERMJ_CONTROLLER_SERVER_HPP_
#define FEVERMJ_CONTROLLER_SERVER_HPP_
#include <array>
#include <functional>
#include <limits>
#include <string>
#include <DxLib.h>
#include <boost/lexical_cast.hpp>
#include <boost/noncopyable.hpp>
#include "input.hpp"
#include "sequence.hpp"
#include "../utility/algorithm.hpp"
#include "../utility/network.hpp"
#include "../view/server.hpp"

namespace FeverMJ { namespace Controller {
class Server : public Sequence {
 public:
  Server() {
    PreparationListenNetWork(10000);
    sequence = [this] {WaitConnect();};
    serverView.Init([this] {
      next = GotoTitle();
    });
    FEVERMJ_LOG("seed %d\n", seed);
  }

  ~Server() {
    StopListenNetWork();
  }

  SequencePtr Update() {
    input.Update();
    serverView.Draw(input, waitCount);
    try {
      sequence();
    } catch (const Utility::NetWorkError &error) {
      MessageBox(GetMainWindowHandle(), error.what(), "エラー", 0);
      return GotoTitle();
    } catch (...) {
      throw;
    }
    return std::move(next);
  }

 private:
  void SendClient1() {
    std::string send = "fp" + boost::lexical_cast<std::string>((firstParent + 1) % 3) + 
                       "sd" + Utility::GetSeedString(seed) +
                       "wait" + "000.000.000.000";
    FEVERMJ_LOG("送信データ %s 送信サイズ %d\n", send.c_str(), send.length());
    netHandles[0].Send(send);
    sequence = [this] {ReceiveClient1();};
  }

  void ReceiveClient1() {
    if (const auto recv = netHandles[0].Receive(4)) {
      if (*recv == "wait") {
        SendClient2();
        sequence = [this] {ReceiveClient2();};
      } else {
        throw Utility::NetWorkError("受信失敗");
      }
    }
  }

  void SendClient2() {
    IPDATA ip;
    GetNetWorkIP(netHandles[0].Get(), &ip);
    std::string send = "fp" + boost::lexical_cast<std::string>((firstParent + 2) % 3) + 
                       "sd" + Utility::GetSeedString(seed) +
                       "ipsd" + Utility::GetIPString(ip);
    FEVERMJ_LOG("送信データ %s 送信サイズ %d\n", send.c_str(), send.length());
    netHandles[1].Send(send);
  }

  void ReceiveClient2() {
    if (const auto recv = netHandles[1].Receive(7)) {
      if (*recv == "connect") {
        sequence = [this] {ReceiveClient3();};
      } else {
        throw Utility::NetWorkError("受信失敗");
      }
    }
  }

  void ReceiveClient3() {
    if (const auto recv = netHandles[0].Receive(7)) {
      if (*recv == "connect") {
        next = GotoGame(firstParent, seed, std::move(netHandles));
      } else {
        throw Utility::NetWorkError("受信失敗");
      }
    }
  }

  void WaitConnect() {
    netHandles[waitCount] = Utility::NetWorkHandle{GetNewAcceptNetWork()}; 
    if (netHandles[waitCount]) {
      ++waitCount;
      if (waitCount == 2) {
        StopListenNetWork();
        SendClient1();
      }
    }
  }
  
  Input input;
  Utility::NetHandleArray netHandles = {{}};
  int waitCount = 0;
  std::function<void ()> sequence = [] {};
  View::Server serverView;
  SequencePtr next = nullptr;
  const int firstParent = GetRand(2);
  const std::uint32_t seed = GetRand(std::numeric_limits<int>::max()); 
};
}}

#endif

