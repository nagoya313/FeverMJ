#ifndef FEVERMJ_CONTROLLER_SERVER_HPP_
#define FEVERMJ_CONTROLLER_SERVER_HPP_
#include <array>
#include <functional>
#include <iostream>
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
  }

  ~Server() {
    StopListenNetWork();
    for (const int handle : netHandles) {
      if (handle != -1) {
        CloseNetWork(handle);
      }
    }
  }

  std::unique_ptr<Sequence> Update() {
    input.Update();
    serverView.Draw(input, waitCount);
    sequence();
    return std::move(next);
  }

 private:
  void WaitConnect() {
    netHandles[waitCount] = GetNewAcceptNetWork(); 
    if (netHandles[waitCount] != -1) {
      if (++waitCount == 2) {
        StopListenNetWork();
        IPDATA ip;
        GetNetWorkIP(netHandles[1], &ip);
       char send0[] = "ipsd";
        if (NetWorkSend(netHandles[0], send0, 4) == -1) {
          if (NetWorkSend(netHandles[0], send0, 4) == -1) {
            MessageBox(GetMainWindowHandle(), "送信失敗", "エラー", 0);
            next = GotoTitle();
          }
        }
        char send1[] = "wait";
        if (NetWorkSend(netHandles[0], send1, 4) == -1) {
          if (NetWorkSend(netHandles[0], send1, 4) == -1) {
            MessageBox(GetMainWindowHandle(), "送信失敗", "エラー", 0);
            next = GotoTitle();
          }
        }
        sequence = [this] {};
      }
    }
  }
  
   /*
  void SendInit() {
    currentHouse = GetRand(2);
    const std::string sendMessage[] = {
      "f" + boost::lexical_cast<std::string>((currentHouse + 1) % 3),
      "f" + boost::lexical_cast<std::string>(currentHouse),
      "f" + boost::lexical_cast<std::string>((currentHouse + 2) % 3)
    };
    for (int i = 0; i < 3) {
      NetWorkSend(netHandles[i], sendMessage[i].c_str(), sendMessage[i].length());
    }
    bool respResult[] = {}
    while(!ProcessMessage()) {
      for (int i = 0; i < 3; ++i) {
        if (const int length = GetNetWorkDataLength(netHandles[i])) {
          if (length == 3) {
            std::array<char, 3> strBuf;
            NetWorkRecv(netHandle[i], strBuf.data(), length);
            if (std::string(strBuf.data(), 3) == "fok") {
              respResult[i] = true;
            } else {
              NetWorkSend(netHandles[i], sendMessage[i].c_str(), sendMessage[i].length());
            }
			    } else {
			      NetWorkSend(netHandles[i], sendMessage[i].c_str(), sendMessage[i].length());
			    }
			  }
      }
      if (boost::all_of(respResult, [](bool x) {return x;})) {
        return;
      }
		}
		sequence = [this] {SendTumo();};
  }

  void SendTumo() {
    const std::string tumoStr = "";
    NetWorkSend(netHandles[currentHouse], tumoStr.c_str(), tumoStr.length());
  }
 
  int CheckLost() const {
    const auto it = boost::find(netHandles, GetLostNetWork());
    if (it != networkHandle.end()) {
      const int lostID = std::distance(netHandles.begin(), it);
      std::cout << lostID << " network lost." << std::endl; 
      return lostID;
    }
    return -1;
  }
  */
  
  Input input;
  std::array<int, 2> netHandles;
  int waitCount = 0;
  std::function<void ()> sequence = [] {};
  View::Server serverView;
  std::unique_ptr<Sequence> next = nullptr;
};
}}

#endif

