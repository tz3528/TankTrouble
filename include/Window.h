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

	static int WindowWidth = 1280;
	static int WindowHeight = 720;

	extern int LeftWall, RightWall, UpWall, BottomWall;

	extern HDC hdcMem;
	extern HBITMAP hbmMem;

	void init();
	int start(
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		LPSTR lpCmdLine,
		int nCmdShow);

	LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	void paint(HWND hwnd);
	void keyDown(HWND hwnd, WPARAM wParam);
	void keyUp(HWND hwnd, WPARAM wParam);
}

#endif //WINDOW_Hs
