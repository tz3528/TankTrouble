#include "Win32Controls.h"

namespace TankTrouble {

	HFONT hFont = CreateFont(
		25, 0,                      // 字体高度和宽度
		0, 0,                       // 文字角度和基线角度
		FW_NORMAL, FALSE, FALSE,    // 字体重量、斜体、下划线
		FALSE, DEFAULT_CHARSET,     // 删除线和字符集
		OUT_DEFAULT_PRECIS,        // 输出精度
		CLIP_DEFAULT_PRECIS,       // 裁剪精度
		DEFAULT_QUALITY,           // 输出质量
		DEFAULT_PITCH | FF_SWISS,  // 字体间距和族
		L"Arial"                   // 字体名称
	);

	void CreateRadioGroupHorizontal(
		HWND hwnd, int x, int y, int width, int height,
		int num, ControlsInfo* info, HWND radioGroup[])
	{
		for (int i = 0;i < num;i++) {
			DWORD style = WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON;
			if (i == 0) {
				style |= WS_GROUP;  // 仅第一个按钮使用 WS_GROUP 样式
			}
			radioGroup[i] = CreateWindow(
				L"BUTTON", info[i].text, style,  // 按钮类名、文本、样式
				x + RadioButtonWidth * i + ButtonGap * i, y,
				width, height,
				hwnd, (HMENU)(info[i].id),  // 父窗口句柄,按钮ID
				(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), nullptr
			);
			SendMessage(radioGroup[i], WM_SETFONT, (WPARAM)hFont, TRUE);
			ShowWindow(radioGroup[i], SW_HIDE);
		}
		SendMessage(radioGroup[0], BM_SETCHECK, BST_CHECKED, 0);
		return ;
	}

	void CreateRadioGroupVertical(
		HWND hwnd, int x, int y, int width, int height,
		int num, ControlsInfo* info, HWND radioGroup[])
	{
		for (int i = 0;i < num;i++) {
			DWORD style = WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON;
			if (i == 0) {
				style |= WS_GROUP;  // 仅第一个按钮使用 WS_GROUP 样式
			}
			radioGroup[i] = CreateWindow(
				L"BUTTON", info[i].text, style,  // 按钮类名、文本、样式
				x + RadioButtonWidth * i + ButtonGap * i, y,
				width, height,
				hwnd, (HMENU)(info[i].id),  // 父窗口句柄,按钮ID
				(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), nullptr
			);
			SendMessage(radioGroup[i], WM_SETFONT, (WPARAM)hFont, TRUE);
			ShowWindow(radioGroup[i], SW_HIDE);
		}
		SendMessage(radioGroup[0], BM_SETCHECK, BST_CHECKED, 0);
		return ;
	}

}