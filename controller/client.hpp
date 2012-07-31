#ifndef FEVERMJ_CONTROLLER_CLIENT_HPP_
#define FEVERMJ_CONTROLLER_CLIENT_HPP_
#include <array>
#include <functional>
#include <iostream>
#include <string>
#include "input.hpp"
#include "sequence.hpp"
#include "../utility/algorithm.hpp"
#include "../utility/network.hpp"
#include "../view/client.hpp"

namespace FeverMJ { namespace Controller {
class Client: public Sequence {
 public:
  Client() {
    PreparationListenNetWork(18000);
    sequence = [this] {WaitConnect();};
    clientView.Init([this] {
      next = GotoTitle();
    });
  }

  ~Client() {
    StopListenNetWork();
    for (const int handle : netHandles) {
      if (handle != -1) {
        CloseNetWork(handle);
      }
    }
  }

  std::unique_ptr<Sequence> Update() {
    input.Update();
    clientView.Draw(input);
    sequence();
    return std::move(next);
  }

 private:
  void WaitConnect() {
    char iptemp[17];
    KeyInputString(0, 40, 17, iptemp, FALSE); 
    if (const auto  ip = Utility::GetIP(std::string{iptemp})) {
      netHandles[0] = ConnectNetWork(*ip, 10000);
      if (netHandles[0] == -1) {
        MessageBox(GetMainWindowHandle(), "接続失敗", "エラー", 0);
        next = GotoTitle();
      } else {
        sequence = [this] {WaitResponse();};
      }
    } else {
      MessageBox(GetMainWindowHandle(), "IPアドレスの入力が謎", "エラー", 0);
    }
  }

  void WaitResponse() {
    char recv[4];
    if (GetNetWorkDataLength(netHandles[0]) == 4) {
      NetWorkRecv(netHandles[0], recv, 4);
      const std::string str{recv, 4};
      if (str == "ipsd") {
        sequence = [this] {WaitIP();};
      } else if (str == "wait") {
        PreparationListenNetWork(10000);
        sequence = [this] {WaitConnect2();};
      } else {
        MessageBox(GetMainWindowHandle(), "接続失敗", "エラー", 0);
        next = GotoTitle();
      }
    }
  }

  void WaitConnect2() {
    netHandles[1] = GetNewAcceptNetWork(); 
    if (netHandles[1] != -1) {
      StopListenNetWork();
      sequence = [this] {WaitSeed();};  
    } else {
      MessageBox(GetMainWindowHandle(), "接続失敗", "エラー", 0);
      next = GotoTitle();
    }
  }

  void WaitSeed() {
    char send[] = "reqs";
    if (NetWorkSend(netHandles[0], send, 4) == -1) {
      if (NetWorkSend(netHandles[0], send, 4) == -1) {
        MessageBox(GetMainWindowHandle(), "送信失敗", "エラー", 0);
        next = GotoTitle();
      }
    }
  }

  void WaitIP() {
    char send[] = "reqi";
    if (NetWorkSend(netHandles[0], send, 4) == -1) {
      if (NetWorkSend(netHandles[0], send, 4) == -1) {
        MessageBox(GetMainWindowHandle(), "送信失敗", "エラー", 0);
        next = GotoTitle();
      }
    }
    char recv[15];
    if (GetNetWorkDataLength(netHandles[0]) == 15 && NetWorkRecv(netHandles[0], recv, 15)) {
      if (const auto  ip = Utility::GetIP(std::string{recv, 15})) {
        netHandles[1] = ConnectNetWork(*ip, 10000);
        if (netHandles[1] == -1) {
          MessageBox(GetMainWindowHandle(), "接続失敗", "エラー", 0);
          next = GotoTitle();
        } else {
          sequence = [this] {WaitSeed();}; 
        }
      } else {
        MessageBox(GetMainWindowHandle(), "接続失敗", "エラー", 0);
        next = GotoTitle();
      }
    }
  }
  
  Input input;
  std::array<int, 2> netHandles;
  int waitCount = 0;
  std::function<void ()> sequence = [] {};
  View::Client clientView;
  std::unique_ptr<Sequence> next = nullptr;
};
}}

#endif

