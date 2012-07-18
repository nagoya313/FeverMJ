#define DX_NON_OGGVORBIS
#define DX_NON_OGGTHEORA
#include <DxLib.h>
#include "controller/game.hpp"
#include "view/info.hpp"
#include "model/check_role.hpp"
#include "model/check_tenpai.hpp"
#include "model/player.hpp"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  SetMainWindowText(FeverMJ::View::title);
  ChangeWindowMode(TRUE);
  SetGraphMode(FeverMJ::View::screenWidth, FeverMJ::View::screenHeight, FeverMJ::View::colorDepth);
  if (DxLib_Init() == -1)	{
    MessageBox(nullptr, "アプリの初期化に失敗...", "エラー", MB_OK);
		return -1;
	}
  SetFontSize(FeverMJ::View::defaultGameFontSize);
  ChangeFontType(DX_FONTTYPE_ANTIALIASING_EDGE);
  SetDrawScreen(DX_SCREEN_BACK);
  FeverMJ::Controller::Game game;
  while (!ProcessMessage()) {
    game.Update();
  }
  DxLib_End();
  return 0;
}
