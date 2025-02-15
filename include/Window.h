#ifndef WINDOW_H
#define WINDOW_H

#include <mutex>
#include <condition_variable>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl.h>
#include <windows.h>

using Microsoft::WRL::ComPtr;

namespace TankTrouble
{

#define SINGLE_GAME		1
#define ONLINE_GAME		2
#define CAMPAIGN		3

#define SMALL_MAP		1
#define MEDIUM_MAP		2
#define LARGE_MAP		3

	static int WindowWidth = 1280;
	static int WindowHeight = 720;
	static int ButtonWidth = 300;
    static int ButtonHeight = 60;
	static int ButtonGap = 20;

	extern int LeftWall, RightWall, UpWall, BottomWall;

	extern HDC hdcMem;
	extern HBITMAP hbmMem;

	int MapSize = 1;

	//°´Å¥
	static HWND hwndButtonSingleGame;
    static HWND hwndButtonOnlineGame;
    static HWND hwndButtonCampaign;
	static HWND hwndButtonBeginGame;
    static HWND hwndButtonBack;

	//int GameMode = 0;

	
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
	void gameInit(HWND hwnd);

	void paint(HWND hwnd);
	void keyDown(HWND hwnd, WPARAM wParam);
	void keyUp(HWND hwnd, WPARAM wParam);
}

#endif //WINDOW_Hs
