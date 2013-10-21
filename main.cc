#include <memory>
#include <stdexcept>
#include <DxLib.h>
#include <boost/noncopyable.hpp>
#include "controller/game.hpp"
#include "controller/title.hpp"
#include "view/info.hpp"

namespace FeverMJ { namespace {
struct DXLibInitializer : private boost::noncopyable {
  DXLibInitializer() {
    if (DxLib_Init() == -1) {
      throw std::runtime_error{"アプリの初期化に失敗..."};
    }
  }

  ~DXLibInitializer() {
    DxLib_End();
  }
};

void GameMain() {
  SetMainWindowText(View::title);
  ChangeWindowMode(TRUE);
  SetAlwaysRunFlag(TRUE);
#ifndef NDEBUG
  SetDoubleStartValidFlag(TRUE);
#else
  SetOutApplicationLogValidFlag(FALSE);
#endif
  SetGraphMode(View::screenWidth, View::screenHeight, View::colorDepth);
  const DXLibInitializer dxLib;
  SetFontSize(View::defaultGameFontSize);
  ChangeFontType(DX_FONTTYPE_ANTIALIASING_EDGE);
  SetDrawScreen(DX_SCREEN_BACK);
  std::unique_ptr<Controller::Sequence> seq{new Controller::Title{}};
  while (!ProcessMessage()) {
    if (auto next = seq->Update()) {
      seq = std::move(next);
    }
  } 
}
}}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) try {
  FeverMJ::GameMain();
  return 0;
} catch (const std::exception &error) {
  MessageBox(nullptr, error.what(), "エラー", MB_OK);
  return 0;
}
