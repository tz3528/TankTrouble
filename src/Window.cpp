#include "Window.h"

#include "point.h"
#include "bullet.h"
#include "Wall.h"
#include "Tank.h"
#include "Win32Controls.h"

#include <graphics.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include <commctrl.h>

using std::cout, std::endl, std::make_shared;

namespace TankTrouble
{
	std::mutex uiMutex;
	std::condition_variable uiCv;
	bool update;

	HANDLE g_hOutput = 0;

	HDC hdcMem;
	HBITMAP hbmMem;

	int LeftWall, RightWall, UpWall, BottomWall;

	LRESULT CALLBACK StartWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_COMMAND: {
			buttonDown(hwnd, wParam);
			break;
		}
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
		return 0;
	}

	LRESULT CALLBACK SingleGameWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_KEYDOWN:
			keyDown(hwnd, wParam);
			break;
		case WM_KEYUP:
			keyUp(hwnd, wParam);
			break;
		case WM_PAINT:
			paint(hwnd);
			break;
		case WM_ERASEBKGND:
		{
			return 1; // 告诉Windows消息已经被处理
		}
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
		return 0;
	}

	LRESULT CALLBACK OnlineGameWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_KEYDOWN:
			keyDown(hwnd, wParam);
			break;
		case WM_KEYUP:
			keyUp(hwnd, wParam);
			break;
		case WM_PAINT:
			paint(hwnd);
			break;
		case WM_ERASEBKGND:
		{
			return 1; // 告诉Windows消息已经被处理
		}
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
		return 0;
	}

	LRESULT CALLBACK CAMPAIGNWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_KEYDOWN:
			keyDown(hwnd, wParam);
			break;
		case WM_KEYUP:
			keyUp(hwnd, wParam);
			break;
		case WM_PAINT:
			paint(hwnd);
			break;
		case WM_ERASEBKGND:
		{
			return 1; // 告诉Windows消息已经被处理
		}
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
		return 0;
	}

	void init(HWND hwnd) {

		LeftWall = 8;
		RightWall = WindowWidth - 24;
		UpWall = 8;
		BottomWall = WindowHeight - 48;

		buttonInit(hwnd);

	}

	void buttonInit(HWND hwnd) {
		hwndButtonSingleGame = CreateWindow(
			L"BUTTON",  // 按钮类名
			L"单机游戏",  // 按钮文本
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // 按钮样式
			WindowWidth / 2 - ButtonWidth / 2, WindowHeight / 5 - ButtonHeight / 2,
			ButtonWidth, ButtonHeight,
			hwnd, (HMENU)SINGLE_GAME,  // 父窗口句柄,按钮ID
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),nullptr
		);

		hwndButtonOnlineGame = CreateWindow(
			L"BUTTON",  // 按钮类名
			L"在线对战",  // 按钮文本
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // 按钮样式
			WindowWidth / 2 - ButtonWidth / 2, WindowHeight / 5 + ButtonHeight / 2 + ButtonGap,
			ButtonWidth, ButtonHeight,
			hwnd, (HMENU)ONLINE_GAME,  // 父窗口句柄,按钮ID
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), nullptr
		);

		hwndButtonCampaign = CreateWindow(
			L"BUTTON",  // 按钮类名
			L"战役",  // 按钮文本
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // 按钮样式
			WindowWidth / 2 - ButtonWidth / 2, WindowHeight / 5 + 3 * ButtonHeight / 2 + 2 * ButtonGap,
			ButtonWidth, ButtonHeight,
			hwnd, (HMENU)CAMPAIGN,  // 父窗口句柄,按钮ID
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), nullptr
		);

		//ShowWindow(hwndButtonSingleGame, SW_HIDE);
        //ShowWindow(hwndButtonOnlineGame, SW_HIDE);
        //ShowWindow(hwndButtonCampaign, SW_HIDE);

		hwndButtonBeginGame = CreateWindow(
			L"BUTTON",  // 按钮类名
			L"开始游戏",  // 按钮文本
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // 按钮样式
			WindowWidth / 2 - ButtonWidth - ButtonGap / 2, 3 * WindowHeight / 4 - ButtonHeight / 2,
			ButtonWidth, ButtonHeight,
			hwnd, (HMENU)BEGIN_GAME,  // 父窗口句柄,按钮ID
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), nullptr
		);

		hwndButtonBack = CreateWindow(
			L"BUTTON",  // 按钮类名
			L"返回",  // 按钮文本
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // 按钮样式
			WindowWidth / 2 + ButtonGap / 2, 3 * WindowHeight / 4 - ButtonHeight / 2,
			ButtonWidth, ButtonHeight,
			hwnd, (HMENU)BACK,  // 父窗口句柄,按钮ID
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),nullptr
		);

		ShowWindow(hwndButtonBeginGame, SW_HIDE);
		ShowWindow(hwndButtonBack, SW_HIDE);

	}

	void radioButtonInit(HWND hwnd){
		//玩家数量信息
        ControlsInfo PlayerNumberInfo[MAX_PLAYER_NUMBER] = {
            {ONE_PLAYER, L"1"},
            {TWO_PLAYER, L"2"},
            {THREE_PLAYER, L"3"},
            {FOUR_PLAYER, L"4"},
        };

		HWND* radioGroupNumber = new HWND[MAX_PLAYER_NUMBER];
		CreateRadioGroupHorizontal(
			hwnd, 300, 100, ButtonWidth, ButtonHeight,
			MAX_PLAYER_NUMBER, PlayerNumberInfo, radioGroupNumber
		);

		for (int i = 0;i < 4;i++) {
			ShowWindow(radioGroupNumber[i], SW_HIDE);
		}

	}

	void buttonDown(HWND hwnd, WPARAM wParam){
		//用于判断是否选择了游戏模式
		switch (LOWORD(wParam))
		{
		case SINGLE_GAME:
			GameMode = SINGLE_GAME;
			break;
		case ONLINE_GAME:
			GameMode = ONLINE_GAME;
			break;
		case CAMPAIGN:
			GameMode = CAMPAIGN;
			break;
		case BEGIN_GAME:
            ShowWindow(hwndButtonBeginGame, SW_HIDE);
            ShowWindow(hwndButtonBack, SW_HIDE);
            UpdateWindow(hwndButtonBeginGame);
            UpdateWindow(hwndButtonBack);
			switch (GameMode) 
			{
			case NOSELECT:
				break;
			case SINGLE_GAME:
				SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)SingleGameWndProc);
				break;
			case ONLINE_GAME:
				SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)OnlineGameWndProc);
				break;
			case CAMPAIGN:
				SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)CAMPAIGNWndProc);
				break;
			}
			
			InvalidateRect(hwnd, nullptr, TRUE);
			return ;
		case BACK:
			repickMode(hwnd);
			InvalidateRect(hwnd, nullptr, TRUE);
			return ;
		}
		if (GameMode != NOSELECT) {
			selectGameMode(hwnd);
		}
	}

	void selectGameMode(HWND hwnd) {

		ShowWindow(hwndButtonSingleGame, SW_HIDE);
		ShowWindow(hwndButtonOnlineGame, SW_HIDE);
		ShowWindow(hwndButtonCampaign, SW_HIDE);
		
		InvalidateRect(hwnd, nullptr, TRUE);

		ShowWindow(hwndButtonBeginGame, SW_SHOW);
        ShowWindow(hwndButtonBack, SW_SHOW);

		//创建地图边缘的墙
		WallPool.push_back(make_shared<Wall>
			(point{ LeftWall ,UpWall }, point{ LeftWall,BottomWall }, MapSize)
		);
		WallPool.push_back(make_shared<Wall>
			(point{ RightWall ,UpWall }, point{ RightWall,BottomWall }, MapSize)
		);
		WallPool.push_back(make_shared<Wall>
			(point{ LeftWall ,UpWall }, point{ RightWall,UpWall }, MapSize)
		);
		WallPool.push_back(make_shared<Wall>
			(point{ LeftWall ,BottomWall }, point{ RightWall,BottomWall }, MapSize)
		);
	}

	void repickMode(HWND hwnd){
		GameMode = NOSELECT;

		ShowWindow(hwndButtonSingleGame, SW_SHOW);
		ShowWindow(hwndButtonOnlineGame, SW_SHOW);
		ShowWindow(hwndButtonCampaign, SW_SHOW);

		InvalidateRect(hwnd, nullptr, TRUE);

		ShowWindow(hwndButtonBeginGame, SW_HIDE);
		ShowWindow(hwndButtonBack, SW_HIDE);
	}

	void paint(HWND hwnd) {
		PAINTSTRUCT ps = { 0 };
		HDC hdc = BeginPaint(hwnd, &ps);

		// 创建兼容的内存设备上下文和位图
		hdcMem = CreateCompatibleDC(hdc);
		hbmMem = CreateCompatibleBitmap(hdc, WindowWidth, WindowHeight);
		SelectObject(hdcMem, hbmMem);

		// 填充背景为白色
		HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
		HBRUSH oldBrush = (HBRUSH)SelectObject(hdcMem, whiteBrush);
		RECT rect = { 0, 0, WindowWidth, WindowHeight };
		FillRect(hdcMem, &rect, whiteBrush);
		SelectObject(hdcMem, oldBrush);
		DeleteObject(whiteBrush);

		// 绘制坦克
		for (auto& Tank : TankPool) {
			Tank->draw(hdcMem);
		}

		// 创建黑色画刷用于绘制墙和子弹
		HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
		oldBrush = (HBRUSH)SelectObject(hdcMem, brush);

		// 绘制子弹
		for (auto& bullet : bulletPool) {
			bullet->draw(hdcMem);
		}

		// 绘制墙
		for (auto& wall : WallPool) {
			wall->draw(hdcMem);
		}

		// 恢复旧画刷并删除新画刷
		SelectObject(hdcMem, oldBrush);
		DeleteObject(brush);

		// 将内存设备上下文的内容复制到窗口设备上下文
		BitBlt(hdc, 0, 0, WindowWidth, WindowHeight, hdcMem, 0, 0, SRCCOPY);

		// 删除内存设备上下文和位图
		DeleteObject(hbmMem);
		DeleteDC(hdcMem);

		EndPaint(hwnd, &ps);
	}

	void keyDown(HWND hwnd, WPARAM wParam) {
		for (auto& Tank : TankPool) {
			if (Tank->getController() == COMPUTER)
				continue;

			if (wParam == 'J') {
				Tank->isAttack = true;
			}
			else {
				if (wParam == 'W') {
					Tank->isForward = true;
				}
				if (wParam == 'S') {
					Tank->isBackward = true;
				}
				if (wParam == 'A') {
					Tank->isLeft = true;
				}
				if (wParam == 'D') {
					Tank->isRight = true;
				}
			}
		}

	}

	void keyUp(HWND hwnd, WPARAM wParam) {
		for (auto& Tank : TankPool) {
			if (Tank->getController() == COMPUTER)
				continue;

			if (wParam == 'J') {
				Tank->isAttack = false;
			}
			else {
				if (wParam == 'W') {
					Tank->isForward = false;
				}
				if (wParam == 'S') {
					Tank->isBackward = false;
				}
				if (wParam == 'A') {
					Tank->isLeft = false;
				}
				if (wParam == 'D') {
					Tank->isRight = false;
				}
			}
		}
	}

	int start(
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		LPSTR lpCmdLine,
		int nCmdShow)
	{
		
		auto const pClassName = L"TankTrouble";

		// register window C lass
		WNDCLASSEX wc = { 0 };
		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = StartWndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = nullptr;
		wc.hCursor = nullptr;
		wc.hbrBackground = nullptr;
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = pClassName;
		wc.hbrBackground = CreateSolidBrush(WHITE);
		RegisterClassEx(&wc);

		// create window instance
		HWND hwnd = CreateWindowEx(
			0, pClassName, L"TankTrouble",
			WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
			CW_USEDEFAULT, CW_USEDEFAULT,
			WindowWidth, WindowHeight,
			nullptr, nullptr, hInstance, nullptr);

		init(hwnd);

		TankPool.emplace_back(std::make_shared<Tank>(0, 0, point(2 * WindowWidth / 20, 2 * WindowHeight / 20),
			point(1, 0), MapSize, RED));

		//show the window
		ShowWindow(hwnd, SW_SHOW);

		UpdateWindow(hwnd);

		MSG message;

		//AllocConsole();
		//g_hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		//char tmp[256] = { 0 };
		//sprintf_s(tmp, sizeof(tmp), "%d %d %d %d\n", LeftWall, RightWall, UpWall, BottomWall);
		//WriteConsoleA(g_hOutput, tmp, (DWORD)strlen(tmp), nullptr, nullptr);

		std::thread bulletThread(bulletPoolUpdate);
		std::thread tankThread(TankControl);

		bulletThread.detach();
        tankThread.detach();

		while (true) {
			if (PeekMessage(&message, nullptr, 0, 0, PM_NOREMOVE)) {
				if (GetMessage(&message, nullptr, 0, 0)) {
					TranslateMessage(&message);
					DispatchMessage(&message);
				}
				else {
					return 0;
				}
			}
			else {
				//WriteConsole(g_hOutput, L"2222", 4, nullptr,nullptr);
			}

		}

		// 释放资源
		for (auto& tank : TankPool) {
			tank.reset();
		}
		TankPool.clear();

		for (auto& bullet : bulletPool) {
			bullet.reset();
		}
		bulletPool.clear();

		for (auto& wall : WallPool) {
			wall.reset();
		}
		WallPool.clear();

		// 销毁窗口
		DestroyWindow(hwnd);

		return 0;

	}

}
