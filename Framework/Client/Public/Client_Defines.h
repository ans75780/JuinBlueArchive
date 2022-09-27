#pragma once

namespace Client 
{
	static const unsigned int g_iWinCX = 1280;
	static const unsigned int g_iWinCY = 720;

	enum LEVEL { LEVEL_STATIC, LEVEL_LOADING,
		LEVEL_LOGO, LEVEL_LOBBY,
		LEVEL_CAFE, LEVEL_SCHEDULE, LEVEL_STUDENTS,
		LEVEL_FORMATION, LEVEL_CIRCLE, LEVEL_MANUFACTURE,
		LEVEL_SHOP, LEVEL_GACHA,
		LEVEL_WORK,
		LEVEL_GAMEPLAY, LEVEL_MAPTOOL, LEVEL_END };
}

extern HWND g_hWnd;
extern HINSTANCE g_hInst;

using namespace Client;