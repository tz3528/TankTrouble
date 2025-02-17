#include "Window.h"

#include "point.h"
#include "bullet.h"
#include "Wall.h"
#include "Tank.h"

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
			return 1; // ����Windows��Ϣ�Ѿ�������
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
			return 1; // ����Windows��Ϣ�Ѿ�������
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
			return 1; // ����Windows��Ϣ�Ѿ�������
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
		radioButtonInit(hwnd);

	}

	void buttonInit(HWND hwnd) {
		hwndButtonSingleGame = CreateWindow(
			L"BUTTON",  // ��ť����
			L"������Ϸ",  // ��ť�ı�
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // ��ť��ʽ
			WindowWidth / 2 - PushButtonWidth / 2, WindowHeight / 5 - PushButtonHeight / 2,
			PushButtonWidth, PushButtonHeight,
			hwnd, (HMENU)SINGLE_GAME,  // �����ھ��,��ťID
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),nullptr
		);
		SendMessage(hwndButtonSingleGame, WM_SETFONT, (WPARAM)hFont, TRUE);

		hwndButtonOnlineGame = CreateWindow(
			L"BUTTON",  // ��ť����
			L"���߶�ս",  // ��ť�ı�
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // ��ť��ʽ
			WindowWidth / 2 - PushButtonWidth / 2, WindowHeight / 5 + PushButtonHeight / 2 + ButtonGap,
			PushButtonWidth, PushButtonHeight,
			hwnd, (HMENU)ONLINE_GAME,  // �����ھ��,��ťID
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), nullptr
		);
		SendMessage(hwndButtonOnlineGame, WM_SETFONT, (WPARAM)hFont, TRUE);

		hwndButtonCampaign = CreateWindow(
			L"BUTTON",  // ��ť����
			L"ս��",  // ��ť�ı�
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // ��ť��ʽ
			WindowWidth / 2 - PushButtonWidth / 2, WindowHeight / 5 + 3 * PushButtonHeight / 2 + 2 * ButtonGap,
			PushButtonWidth, PushButtonHeight,
			hwnd, (HMENU)CAMPAIGN,  // �����ھ��,��ťID
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), nullptr
		);
		SendMessage(hwndButtonCampaign, WM_SETFONT, (WPARAM)hFont, TRUE);

		//ShowWindow(hwndButtonSingleGame, SW_HIDE);
        //ShowWindow(hwndButtonOnlineGame, SW_HIDE);
        //ShowWindow(hwndButtonCampaign, SW_HIDE);

		hwndButtonBeginGame = CreateWindow(
			L"BUTTON",  // ��ť����
			L"��ʼ��Ϸ",  // ��ť�ı�
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // ��ť��ʽ
			WindowWidth / 2 - PushButtonWidth - ButtonGap / 2, 3 * WindowHeight / 4 - PushButtonHeight / 2,
			PushButtonWidth, PushButtonHeight,
			hwnd, (HMENU)BEGIN_GAME,  // �����ھ��,��ťID
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), nullptr
		);
		SendMessage(hwndButtonBeginGame, WM_SETFONT, (WPARAM)hFont, TRUE);

		hwndButtonBack = CreateWindow(
			L"BUTTON",  // ��ť����
			L"����",  // ��ť�ı�
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // ��ť��ʽ
			WindowWidth / 2 + ButtonGap / 2, 3 * WindowHeight / 4 - PushButtonHeight / 2,
			PushButtonWidth, PushButtonHeight,
			hwnd, (HMENU)BACK,  // �����ھ��,��ťID
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),nullptr
		);
		SendMessage(hwndButtonBack, WM_SETFONT, (WPARAM)hFont, TRUE);

		ShowWindow(hwndButtonBeginGame, SW_HIDE);
		ShowWindow(hwndButtonBack, SW_HIDE);

	}

	void radioButtonInit(HWND hwnd){
		//���������Ϣ
        ControlsInfo PlayerNumberInfo[MAX_PLAYER] = {
            {ONE_PLAYER, L"1"},
            {TWO_PLAYER, L"2"},
            {THREE_PLAYER, L"3"},
            {FOUR_PLAYER, L"4"},
        };
		/*��ѡ��ť������϶��������ͨ������ó�
		* ����ļ��㷽ʽ��
		* left+(num*ButtonWidth + (num-1)*ButtonGap))/2= WindowWidth/2
		*/
		long groupLeft = (WindowWidth - MAX_PLAYER * RadioButtonWidth - (MAX_PLAYER - 1) * ButtonGap) / 2;
		CreateRadioGroupHorizontal(
			hwnd, groupLeft,100,
			RadioButtonWidth, RadioButtonHeight,
			MAX_PLAYER, PlayerNumberInfo, hwndRadioGroupPlayerNumber
		);

		EditInfo PlayerNumberEditInfo = {
			{PLAYER_NUMBER, L"�˻�����"},
			25 * wcslen(L"�˻�����")
		};

		hwndEditPlayerNumber = CreateWindow(
			L"EDIT", PlayerNumberEditInfo.ctrInfo.text,
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | ES_CENTER,
			groupLeft - PlayerNumberEditInfo.Width - ButtonGap, 100 + 10,
			PlayerNumberEditInfo.Width, EditHeight,
			hwnd, (HMENU)PlayerNumberEditInfo.ctrInfo.id,
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), nullptr
		);
		SendMessage(hwndEditPlayerNumber, WM_SETFONT, (WPARAM)hFont, TRUE);
		SendMessage(hwndEditPlayerNumber, EM_SETREADONLY, TRUE, 0);
		ShowWindow(hwndEditPlayerNumber, SW_HIDE);

		//��ͼ��С
		ControlsInfo MapInfo[3] = {
			{SMALL_MAP, L"С"},
			{MEDIUM_MAP, L"��"},
            {LARGE_MAP, L"��"}
		};
		CreateRadioGroupHorizontal(
			hwnd, groupLeft, 100 + RadioButtonHeight + ButtonGap,
			RadioButtonWidth, RadioButtonHeight,
			3, MapInfo, hwndRadioGroupMapType
		);
		EditInfo MapEditInfo = {
			{MAP_TYPE, L"��ͼ��С"},
			25 * wcslen(L"��ͼ��С")
		};
		hwndEditMapType = CreateWindow(
			L"EDIT", MapEditInfo.ctrInfo.text,
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | ES_CENTER,
			groupLeft - MapEditInfo.Width - ButtonGap, 100 + 10 + RadioButtonHeight + ButtonGap,
			MapEditInfo.Width, EditHeight,
			hwnd, (HMENU)MapEditInfo.ctrInfo.id,
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), nullptr
		);
		SendMessage(hwndEditMapType, WM_SETFONT, (WPARAM)hFont, TRUE);
		SendMessage(hwndEditMapType, EM_SETREADONLY, TRUE, 0);
		ShowWindow(hwndEditMapType, SW_HIDE);

		//̹����ɫ
		ControlsInfo TankColorInfo[5] = {
			{RED, L"��ɫ"}, {BLUE, L"��ɫ"}, {GREEN, L"��ɫ"}, {YELLOW, L"��ɫ"}, {BROWN, L"��ɫ"},
		};
		CreateRadioGroupHorizontal(
			hwnd, groupLeft, 100 + 2 * (RadioButtonHeight + ButtonGap),
			RadioButtonWidth, RadioButtonHeight,
			5, TankColorInfo, hwndRadioGroupTankColor
		);
		EditInfo TankColorEditInfo = {
			{TANK_COLOR, L"̹����ɫ"},
			25 * wcslen(L"̹����ɫ")
		};
		hwndEditTankColor = CreateWindow(
			L"EDIT", TankColorEditInfo.ctrInfo.text,
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | ES_CENTER,
			groupLeft - TankColorEditInfo.Width - ButtonGap, 100 + 10 + 2 * (RadioButtonHeight + ButtonGap),
			TankColorEditInfo.Width, EditHeight,
			hwnd, (HMENU)TankColorEditInfo.ctrInfo.id,
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), nullptr
		);
		SendMessage(hwndEditTankColor, WM_SETFONT, (WPARAM)hFont, TRUE);
		SendMessage(hwndEditTankColor, EM_SETREADONLY, TRUE, 0);
		ShowWindow(hwndEditTankColor, SW_HIDE);
	}

	void buttonDown(HWND hwnd, WPARAM wParam){
		//�����ж��Ƿ�ѡ������Ϸģʽ
		switch (LOWORD(wParam))
		{
		case SINGLE_GAME:
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

			for (int i = 0;i < MAX_PLAYER;i++) {
                ShowWindow(hwndRadioGroupPlayerNumber[i], SW_HIDE);
				UpdateWindow(hwndRadioGroupPlayerNumber[i]);
				if (SendMessage(hwndRadioGroupPlayerNumber[i], BM_GETCHECK, 0, 0) == BST_CHECKED) {
					computers = GetDlgCtrlID(hwndRadioGroupPlayerNumber[i]) - NO_PLAYER;
				}
			}
			ShowWindow(hwndEditPlayerNumber, SW_HIDE);

			for (int i = 0;i < 3;i++) {
                ShowWindow(hwndRadioGroupMapType[i], SW_HIDE);
                UpdateWindow(hwndRadioGroupMapType[i]);
				if (SendMessage(hwndRadioGroupMapType[i], BM_GETCHECK, 0, 0) == BST_CHECKED) {
					MapSize = GetDlgCtrlID(hwndRadioGroupPlayerNumber[i]);
				}
			}
			ShowWindow(hwndEditMapType, SW_HIDE);

			for (int i = 0;i < 5;i++) {
				ShowWindow(hwndRadioGroupTankColor[i], SW_HIDE);
				UpdateWindow(hwndRadioGroupTankColor[i]);
				if (SendMessage(hwndRadioGroupTankColor[i], BM_GETCHECK, 0, 0) == BST_CHECKED) {
					PlayerColor = GetDlgCtrlID(hwndRadioGroupTankColor[i]);
				}
			}
			ShowWindow(hwndEditTankColor, SW_HIDE);

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

		//������ͼ��Ե��ǽ
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

		for (int i = 0;i < MAX_PLAYER;i++) {
			ShowWindow(hwndRadioGroupPlayerNumber[i], SW_HIDE);
		}
		ShowWindow(hwndEditPlayerNumber, SW_HIDE);

		for (int i = 0;i < 3;i++) {
			ShowWindow(hwndRadioGroupMapType[i], SW_HIDE);
		}
		ShowWindow(hwndEditMapType, SW_HIDE);

		for (int i = 0;i < 5;i++) {
            ShowWindow(hwndRadioGroupTankColor[i], SW_HIDE);
		}
		ShowWindow(hwndEditTankColor, SW_HIDE);
	}

	void paint(HWND hwnd) {
		PAINTSTRUCT ps = { 0 };
		HDC hdc = BeginPaint(hwnd, &ps);

		// �������ݵ��ڴ��豸�����ĺ�λͼ
		hdcMem = CreateCompatibleDC(hdc);
		hbmMem = CreateCompatibleBitmap(hdc, WindowWidth, WindowHeight);
		SelectObject(hdcMem, hbmMem);

		// ��䱳��Ϊ��ɫ
		HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
		HBRUSH oldBrush = (HBRUSH)SelectObject(hdcMem, whiteBrush);
		RECT rect = { 0, 0, WindowWidth, WindowHeight };
		FillRect(hdcMem, &rect, whiteBrush);
		SelectObject(hdcMem, oldBrush);
		DeleteObject(whiteBrush);

		// ����̹��
		for (auto& Tank : TankPool) {
			Tank->draw(hdcMem);
		}

		// ������ɫ��ˢ���ڻ���ǽ���ӵ�
		HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
		oldBrush = (HBRUSH)SelectObject(hdcMem, brush);

		// �����ӵ�
		for (auto& bullet : bulletPool) {
			bullet->draw(hdcMem);
		}

		// ����ǽ
		for (auto& wall : WallPool) {
			wall->draw(hdcMem);
		}

		// �ָ��ɻ�ˢ��ɾ���»�ˢ
		SelectObject(hdcMem, oldBrush);
		DeleteObject(brush);

		// ���ڴ��豸�����ĵ����ݸ��Ƶ������豸������
		BitBlt(hdc, 0, 0, WindowWidth, WindowHeight, hdcMem, 0, 0, SRCCOPY);

		// ɾ���ڴ��豸�����ĺ�λͼ
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

		// �ͷ���Դ
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

		// ���ٴ���
		DestroyWindow(hwnd);

		return 0;

	}

}
