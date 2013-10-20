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
    PreparationListenNetWork(10000);
    sequence = [this] {WaitConnect();};
    clientView.Init([this] {
      next = GotoTitle();
    });
  }

  ~Client() {
    StopListenNetWork();
  }

  SequencePtr Update() {
    input.Update();
    clientView.Draw(input);
    try {
      sequence();
    } catch (const Utility::NetWorkError &error) {
      MessageBox(GetMainWindowHandle(), error.what(), "�G���[", 0);
      return GotoTitle();
    } catch (...) {
      throw;
    }
    return std::move(next);
  }

 private:
  void WaitConnect() {
    char iptemp[17];
    KeyInputString(0, 40, 17, iptemp, FALSE); 
    if (const auto  ip = Utility::GetIP(std::string{iptemp})) {
      netHandles[0] = Utility::NetWorkHandle{ConnectNetWork(*ip, 10000)};
      if (!netHandles[0]) {
        throw Utility::NetWorkError{"�ڑ����s"};
      } else {
        sequence = [this] {WaitResponse();};
      }
    } else {
      MessageBox(GetMainWindowHandle(), "IP�A�h���X�̓��͂���", "�G���[", 0);
    }
  }

  void WaitResponse() {
    constexpr int recvSize = 2 + 1 + 2 + std::numeric_limits<std::uint32_t>::digits10 + 1 + 4 + 15;
    if (const auto recv = netHandles[0].Receive(recvSize)) {
      if (const auto data = Utility::GetInitData(*recv)) {
        if (data->func == "ipsd") {
          FEVERMJ_LOG("�҂�\n ��M�T�C�Y %d", recvSize);
          if (const auto ip = Utility::GetIP(data->ip)) {
            netHandles[1] = Utility::NetWorkHandle{ConnectNetWork(*ip, 10000)};
            if (!netHandles[1]) {
              throw Utility::NetWorkError{"�ڑ����s"};
            } else {
              netHandles[0].Send("connect");
              next = GotoGame(data->firstParent, data->seed, std::move(netHandles));
            }
          } else {
            throw Utility::NetWorkError{"�ڑ����s"};
          }
        } else if (data->func == "wait") {
          FEVERMJ_LOG("�҂�\n ��M�T�C�Y %d\n", recvSize);
          PreparationListenNetWork(10000);
          netHandles[0].Send("wait");
          sequence = [this, data] {WaitConnect2(data->firstParent, data->seed);};
        } else {
          throw Utility::NetWorkError{"�ڑ����s"};
        }
      } else {
        throw Utility::NetWorkError{"�f�[�^�擾���s"};
      }
    }
  }

  void WaitConnect2(int firstParent, int seed) {
    netHandles[1] = Utility::NetWorkHandle{GetNewAcceptNetWork()}; 
    if (netHandles[1]) {
      netHandles[0].Send("connect");
      StopListenNetWork();
      std::swap(netHandles[0], netHandles[1]);
      next = GotoGame(firstParent, seed, std::move(netHandles));
    }
  }
  
  Input input;
  Utility::NetHandleArray netHandles = {{}};
  std::function<void ()> sequence = [] {};
  View::Client clientView;
  SequencePtr next = nullptr;
};
}}

#endif

