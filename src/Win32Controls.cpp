#include "Win32Controls.h"

namespace TankTrouble {

	HFONT hFont = CreateFont(
		25, 0,                      // ����߶ȺͿ��
		0, 0,                       // ���ֽǶȺͻ��߽Ƕ�
		FW_NORMAL, FALSE, FALSE,    // ����������б�塢�»���
		FALSE, DEFAULT_CHARSET,     // ɾ���ߺ��ַ���
		OUT_DEFAULT_PRECIS,        // �������
		CLIP_DEFAULT_PRECIS,       // �ü�����
		DEFAULT_QUALITY,           // �������
		DEFAULT_PITCH | FF_SWISS,  // ���������
		L"Arial"                   // ��������
	);

	void CreateRadioGroupHorizontal(
		HWND hwnd, int x, int y, int width, int height,
		int num, ControlsInfo* info, HWND radioGroup[])
	{
		for (int i = 0;i < num;i++) {
			DWORD style = WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON;
			if (i == 0) {
				style |= WS_GROUP;  // ����һ����ťʹ�� WS_GROUP ��ʽ
			}
			radioGroup[i] = CreateWindow(
				L"BUTTON", info[i].text, style,  // ��ť�������ı�����ʽ
				x + RadioButtonWidth * i + ButtonGap * i, y,
				width, height,
				hwnd, (HMENU)(info[i].id),  // �����ھ��,��ťID
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
				style |= WS_GROUP;  // ����һ����ťʹ�� WS_GROUP ��ʽ
			}
			radioGroup[i] = CreateWindow(
				L"BUTTON", info[i].text, style,  // ��ť�������ı�����ʽ
				x + RadioButtonWidth * i + ButtonGap * i, y,
				width, height,
				hwnd, (HMENU)(info[i].id),  // �����ھ��,��ťID
				(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), nullptr
			);
			SendMessage(radioGroup[i], WM_SETFONT, (WPARAM)hFont, TRUE);
			ShowWindow(radioGroup[i], SW_HIDE);
		}
		SendMessage(radioGroup[0], BM_SETCHECK, BST_CHECKED, 0);
		return ;
	}

}