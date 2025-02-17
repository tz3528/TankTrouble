#ifndef WINDOW_H
#define WINDOW_H

#include "Win32Controls.h"

#include <graphics.h>
#include <mutex>
#include <condition_variable>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl.h>
#include <windows.h>

using Microsoft::WRL::ComPtr;

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

//地图大小的宏定义
#define SMALL_MAP			(0x2200 + 1)
#define MEDIUM_MAP			(0x2200 + 2)
#define LARGE_MAP			(0x2200 + 3)


	static int WindowWidth = 1280;
	static int WindowHeight = 720;

	//边缘四面墙的坐标
	extern int LeftWall, RightWall, UpWall, BottomWall;

	extern HDC hdcMem;
	extern HBITMAP hbmMem;

	static int GameMode = NOSELECT;
	static int MapSize = 1;
	static int computers = 0;
	COLORREF PlayerColor = RED;
	
	int start(
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		LPSTR lpCmdLine,
		int nCmdShow);

	LRESULT CALLBACK StartWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK SingleGameWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnlineGameWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK CAMPAIGNWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	void init(HWND hwnd);
	void buttonInit(HWND hwnd);
	void radioButtonInit(HWND hwnd);

	void buttonDown(HWND hwnd, WPARAM wParam);
	void selectGameMode(HWND hwnd);
	void repickMode(HWND hwnd);

	void paint(HWND hwnd);
	void keyDown(HWND hwnd, WPARAM wParam);
	void keyUp(HWND hwnd, WPARAM wParam);
}

#endif //WINDOW_Hs
