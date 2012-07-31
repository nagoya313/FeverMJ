#include <memory>
#include <DxLib.h>
#include "controller/game.hpp"
#include "controller/title.hpp"
#include "view/info.hpp"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  SetMainWindowText(FeverMJ::View::title);
  ChangeWindowMode(TRUE);
#ifndef NDEBUG
  SetDoubleStartValidFlag(TRUE);
#endif
  SetGraphMode(FeverMJ::View::screenWidth, FeverMJ::View::screenHeight, FeverMJ::View::colorDepth);
  if (DxLib_Init() == -1)	{
    MessageBox(nullptr, "アプリの初期化に失敗...", "エラー", MB_OK);
		return -1;
	}
  SetFontSize(FeverMJ::View::defaultGameFontSize);
  ChangeFontType(DX_FONTTYPE_ANTIALIASING_EDGE);
  SetDrawScreen(DX_SCREEN_BACK);
  std::unique_ptr<FeverMJ::Controller::Sequence> seq{new FeverMJ::Controller::Title{}};
  while (!ProcessMessage()) {
    if (auto next = seq->Update()) {
      seq = std::move(next);
    }
  }
  DxLib_End();
  return 0;
}
