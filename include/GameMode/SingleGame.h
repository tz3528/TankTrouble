#ifndef SINGLE_GAME_H
#define SINGLE_GAME_H

#include <windows.h>

namespace TankTrouble 
{

	LRESULT CALLBACK SingleGameWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	void singleGameInit();
	
	void singleGameRestart();

	void keyDown(HWND hwnd, WPARAM wParam);
	void keyUp(HWND hwnd, WPARAM wParam);
}

#endif 
