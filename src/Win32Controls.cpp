#include "Win32Controls.h"

namespace TankTrouble {

	void CreateRadioGroupHorizontal(
		HWND hwnd, int x, int y, int width, int height,
		int num, ControlsInfo* info, HWND radioGroup[])
	{
		for (int i = 0;i < num;i++) {
			radioGroup[i] = CreateWindow(
				L"BUTTON",  // 按钮类名
				info[i].text,  // 按钮文本
				WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,  // 按钮样式
				x + ButtonWidth * i + ButtonGap * i, y,
				width, height,
				hwnd, (HMENU)(info[i].id),  // 父窗口句柄,按钮ID
				(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), nullptr
			);
		}
		return ;
	}

	void CreateRadioGroupVertical(
		HWND hwnd, int x, int y, int width, int height,
		int num, ControlsInfo* info, HWND radioGroup[])
	{
		for (int i = 0;i < num;i++) {
			radioGroup[i] = CreateWindow(
				L"BUTTON",  // 按钮类名
				info[i].text,  // 按钮文本
				WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,  // 按钮样式
				x, y + ButtonHeight * i + ButtonGap * i,
				width, height,
				hwnd, (HMENU)(info[i].id),  // 父窗口句柄,按钮ID
				(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), nullptr
			);
		}
		return ;
	}

}