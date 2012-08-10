#include <memory>
#include <stdexcept>
#include <DxLib.h>
#include <boost/noncopyable.hpp>
#include "controller/game.hpp"
#include "controller/title.hpp"
#include "view/info.hpp"

namespace {
struct DXLibInitializer : private boost::noncopyable {
  DXLibInitializer() {
    if (DxLib_Init() == -1)	{
      throw std::runtime_error{"アプリの初期化に失敗..."};
	  }
  }

  ~DXLibInitializer() {
    DxLib_End();
  }
};
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  try {
    SetMainWindowText(FeverMJ::View::title);
    ChangeWindowMode(TRUE);
    SetAlwaysRunFlag(TRUE);
#ifndef NDEBUG
    SetDoubleStartValidFlag(TRUE);
#else
    SetOutApplicationLogValidFlag(FALSE);
#endif
    SetGraphMode(FeverMJ::View::screenWidth, FeverMJ::View::screenHeight, FeverMJ::View::colorDepth);
    DXLibInitializer dxLib;
    SetFontSize(FeverMJ::View::defaultGameFontSize);
    ChangeFontType(DX_FONTTYPE_ANTIALIASING_EDGE);
    SetDrawScreen(DX_SCREEN_BACK);
    std::unique_ptr<FeverMJ::Controller::Sequence> seq{new FeverMJ::Controller::Title{}};
    while (!ProcessMessage()) {
      if (auto next = seq->Update()) {
        seq = std::move(next);
      }
    }
  } catch (const std::exception &error) {
    MessageBox(nullptr, error.what(), "エラー", MB_OK);
  }
  return 0;
}
