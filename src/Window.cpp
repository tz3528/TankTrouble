#include "Window.h"

#include "point.h"
#include "bullet.h"
#include "Wall.h"
#include "Tank.h"

#include <graphics.h>
#include <windows.h>
#include <iostream>
#include <vector>

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

	void init() {

		LeftWall = 8;
		RightWall = WindowWidth - 24;
		UpWall = 8;
		BottomWall = WindowHeight - 48;

		WallPool.push_back(make_shared<Wall>
			(point{ LeftWall ,UpWall }, point{ LeftWall,BottomWall }, 1)
		);
		WallPool.push_back(make_shared<Wall>
			(point{ RightWall ,UpWall }, point{ RightWall,BottomWall }, 1)
		);
		WallPool.push_back(make_shared<Wall>
			(point{ LeftWall ,UpWall }, point{ RightWall,UpWall }, 1)
		);
		WallPool.push_back(make_shared<Wall>
			(point{ LeftWall ,BottomWall }, point{ RightWall,BottomWall }, 1)
		);
	}

	LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
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

		// 启用抗锯齿
		SetGraphicsMode(hdcMem, GM_ADVANCED);
		SetBkMode(hdcMem, TRANSPARENT);

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
		init();

		auto const pClassName = L"TankTrouble";

		// register window C lass
		WNDCLASSEX wc = { 0 };
		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = WndProc;
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

		TankPool.emplace_back(std::make_shared<Tank>(0, 0, point(2 * WindowWidth / 20, 2 * WindowHeight / 20),
			point(1, 0), 1, RED));

		//show the window
		ShowWindow(hwnd, SW_SHOW);

		UpdateWindow(hwnd);

		MSG message;

		AllocConsole();
		g_hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		//char tmp[256] = { 0 };
		//sprintf_s(tmp, sizeof(tmp), "%d %d %d %d\n", LeftWall, RightWall, UpWall, BottomWall);
		//WriteConsoleA(g_hOutput, tmp, (DWORD)strlen(tmp), nullptr, nullptr);

		std::thread bulletThread(bulletPoolUpdate);
		std::thread tankThread(TankControl);

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

		return 0;

	}

}
