#include "Win32Controls.h"
#include "Window.h"

namespace TankTrouble {

	//按钮
	HWND hwndButtonSingleGame;
	HWND hwndButtonOnlineGame;
	HWND hwndButtonCampaign;
	HWND hwndButtonBeginGame;
	HWND hwndButtonBack;
	HWND hwndRadioGroupPlayerNumber[10];
	HWND hwndRadioGroupMapType[10];
	HWND hwndRadioGroupTankColor[10];

	//文本框
	HWND hwndEditPlayerNumber;
	HWND hwndEditMapType;
	HWND hwndEditTankColor;

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

	void menuShow(HWND hwnd) {
		ShowWindow(hwndButtonSingleGame, SW_SHOW);
		ShowWindow(hwndButtonOnlineGame, SW_SHOW);
		ShowWindow(hwndButtonCampaign, SW_SHOW);
	}

	void menuHide(HWND hwnd) {
		ShowWindow(hwndButtonSingleGame, SW_HIDE);
		ShowWindow(hwndButtonOnlineGame, SW_HIDE);
		ShowWindow(hwndButtonCampaign, SW_HIDE);
	}

	void selectionShow(HWND hwnd) {
		for (int i = 0;i < MAX_PLAYER;i++) {
			ShowWindow(hwndRadioGroupPlayerNumber[i], SW_SHOW);
		}
		ShowWindow(hwndEditPlayerNumber, SW_SHOW);

		for (int i = 0;i < 3;i++) {
			ShowWindow(hwndRadioGroupMapType[i], SW_SHOW);
		}
		ShowWindow(hwndEditMapType, SW_SHOW);

		for (int i = 0;i < 5;i++) {
			ShowWindow(hwndRadioGroupTankColor[i], SW_SHOW);
		}
		ShowWindow(hwndEditTankColor, SW_SHOW);
	}

	void selectionHide(HWND hwnd) {
		ShowWindow(hwndButtonBeginGame, SW_HIDE);
		ShowWindow(hwndButtonBack, SW_HIDE);

		for (int i = 0;i < MAX_PLAYER;i++) {
			ShowWindow(hwndRadioGroupPlayerNumber[i], SW_HIDE);
			if (SendMessage(hwndRadioGroupPlayerNumber[i], BM_GETCHECK, 0, 0) == BST_CHECKED) {
				computers = GetDlgCtrlID(hwndRadioGroupPlayerNumber[i]) - NO_PLAYER;
			}
		}
		ShowWindow(hwndEditPlayerNumber, SW_HIDE);

		for (int i = 0;i < 3;i++) {
			ShowWindow(hwndRadioGroupMapType[i], SW_HIDE);
			if (SendMessage(hwndRadioGroupMapType[i], BM_GETCHECK, 0, 0) == BST_CHECKED) {
				MapSize = GetDlgCtrlID(hwndRadioGroupMapType[i]);
			}
		}
		ShowWindow(hwndEditMapType, SW_HIDE);

		for (int i = 0;i < 5;i++) {
			ShowWindow(hwndRadioGroupTankColor[i], SW_HIDE);
			if (SendMessage(hwndRadioGroupTankColor[i], BM_GETCHECK, 0, 0) == BST_CHECKED) {
				PlayerColor = GetDlgCtrlID(hwndRadioGroupTankColor[i]);
			}
		}
		ShowWindow(hwndEditTankColor, SW_HIDE);
	}

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