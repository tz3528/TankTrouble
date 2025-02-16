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
//按钮的起始编号
#define BUTTON_BEGIN	0x2000

//按钮编号的宏定义
#define NOSELECT		BUTTON_BEGIN + 0
#define SINGLE_GAME		BUTTON_BEGIN + 1
#define ONLINE_GAME		BUTTON_BEGIN + 2
#define CAMPAIGN		BUTTON_BEGIN + 3
#define BEGIN_GAME		BUTTON_BEGIN + 4
#define BACK			BUTTON_BEGIN + 5

//地图大小的宏定义
#define SMALL_MAP		1
#define MEDIUM_MAP		2
#define LARGE_MAP		3


	static int WindowWidth = 1280;
	static int WindowHeight = 720;
	static int ButtonWidth = 300;
    static int ButtonHeight = 60;
	static int ButtonGap = 20;

	//边缘四面墙的坐标
	extern int LeftWall, RightWall, UpWall, BottomWall;

	extern HDC hdcMem;
	extern HBITMAP hbmMem;

	static int MapSize = 1;
	static int GameMode = NOSELECT;

	//按钮
	static HWND hwndButtonSingleGame;
    static HWND hwndButtonOnlineGame;
    static HWND hwndButtonCampaign;
	static HWND hwndButtonBeginGame;
    static HWND hwndButtonBack;
	
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

	void buttonDown(HWND hwnd, WPARAM wParam);
	void selectGameMode(HWND hwnd);
	void repickMode(HWND hwnd);

	void paint(HWND hwnd);
	void keyDown(HWND hwnd, WPARAM wParam);
	void keyUp(HWND hwnd, WPARAM wParam);
}

#endif //WINDOW_Hs
