#ifndef WINDOW_H
#define WINDOW_H

#include "pch.h"
#include "bullet.h"
#include "Map.h"
#include "Tank.h"
#include "SingleGame/SingleGame.h"
#include "OnlineGame/OnlineGame.h"
#include "Campaign/Campaign.h"
#include "Win32Controls.h"

#include <graphics.h>
#include <mutex>
#include <condition_variable>
#include <wrl.h>
#include <windows.h>
#include <iostream>
#include <filesystem>

using Microsoft::WRL::ComPtr;

extern TimerManager timerManager;
extern ThreadPool threadPool;

namespace TankTrouble
{

//玩家数的宏定义
#define MAX_PLAYER			4
#define NO_PLAYER			0x2100
#define ONE_PLAYER			(NO_PLAYER + 1)
#define TWO_PLAYER			(NO_PLAYER + 2)
#define THREE_PLAYER		(NO_PLAYER + 3)
#define FOUR_PLAYER			(NO_PLAYER + 4)
#define FIVE_PLAYER			(NO_PLAYER + 5)
#define SIX_PLAYER			(NO_PLAYER + 6)
#define SEVEN_PLAYER		(NO_PLAYER + 7)
#define EIGHT_PLAYER		(NO_PLAYER + 8)

	

	extern HDC hdcMem;
	extern HBITMAP hbmMem;
	extern int Running;
	extern int GameMode;
	extern int computers;
	extern COLORREF PlayerColor;
	
	int start(
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		LPSTR lpCmdLine,
		int nCmdShow);

	LRESULT CALLBACK StartWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	
	void init(HWND hwnd);
	void buttonInit(HWND hwnd);
	void radioButtonInit(HWND hwnd);

	void buttonDown(HWND hwnd, WPARAM wParam);
	void selectGameMode(HWND hwnd);
	void repickMode(HWND hwnd);

	void paintControls(HWND hwnd);
	void paintWall(HWND hwnd);
	void paintGame(HWND hwnd);

	void gameLoop(HWND hwnd);
}

#endif //WINDOW_Hs
