#ifndef SINGLE_GAME_H
#define SINGLE_GAME_H

#include "pch.h"

#include <windows.h>

namespace TankTrouble 
{

	LRESULT CALLBACK SingleGameWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	void singleGameInit();
	
	void singleGameRestart();

	static void keyDown(HWND hwnd, WPARAM wParam);
	static void keyUp(HWND hwnd, WPARAM wParam);

	static void buttonDown(HWND hwnd, WPARAM wParam);
}

#endif 
