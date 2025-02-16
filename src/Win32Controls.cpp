#include "Win32Controls.h"

namespace TankTrouble {

	void CreateRadioGroupHorizontal(
		HWND hwnd, int x, int y, int width, int height,
		int num, ControlsInfo* info, HWND radioGroup[])
	{
		for (int i = 0;i < num;i++) {
			radioGroup[i] = CreateWindow(
				L"BUTTON",  // ��ť����
				info[i].text,  // ��ť�ı�
				WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,  // ��ť��ʽ
				x + ButtonWidth * i + ButtonGap * i, y,
				width, height,
				hwnd, (HMENU)(info[i].id),  // �����ھ��,��ťID
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
				L"BUTTON",  // ��ť����
				info[i].text,  // ��ť�ı�
				WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,  // ��ť��ʽ
				x, y + ButtonHeight * i + ButtonGap * i,
				width, height,
				hwnd, (HMENU)(info[i].id),  // �����ھ��,��ťID
				(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), nullptr
			);
		}
		return ;
	}

}