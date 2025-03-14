#ifndef ONLINE_GAME_H
#define ONLINE_GAME_H

#include "pch.h"

namespace TankTrouble
{
	
	LRESULT CALLBACK OnlineGameWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	void onlineGameInit();

}

#endif
