#include "Window.h"


namespace TankTrouble
{
	ThreadPool threadPool(10);
	std::mutex uiMutex;
	std::condition_variable uiCv;
	bool update;

	int Running;
	int LeftWall, RightWall, UpWall, BottomWall;
	int GameMode;
	int computers;
	COLORREF PlayerColor;

	HANDLE g_hOutput = 0;

	HDC hdcMem;
	HBITMAP hbmMem;

	LRESULT CALLBACK StartWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_COMMAND: {
			buttonDown(hwnd, wParam);
			break;
		}
		case WM_PAINT: {
            paintGame(hwnd);
			break;
		}
		case WM_ERASEBKGND: {
			return 1;
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

		for (int i = 1;i <= 100;i++) {
			permutation[i] = i;
		}

	}

	void buttonInit(HWND hwnd) {
		hwndButtonSingleGame = CreateWindow(
			L"BUTTON",  // 按钮类名
			L"单机游戏",  // 按钮文本
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // 按钮样式
			WindowWidth / 2 - PushButtonWidth / 2, WindowHeight / 5 - PushButtonHeight / 2,
			PushButtonWidth, PushButtonHeight,
			hwnd, (HMENU)SINGLE_GAME,  // 父窗口句柄,按钮ID
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),nullptr
		);
		SendMessage(hwndButtonSingleGame, WM_SETFONT, (WPARAM)hFont, TRUE);

		hwndButtonOnlineGame = CreateWindow(
			L"BUTTON",  // 按钮类名
			L"在线对战",  // 按钮文本
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // 按钮样式
			WindowWidth / 2 - PushButtonWidth / 2, WindowHeight / 5 + PushButtonHeight / 2 + ButtonGap,
			PushButtonWidth, PushButtonHeight,
			hwnd, (HMENU)ONLINE_GAME,  // 父窗口句柄,按钮ID
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), nullptr
		);
		SendMessage(hwndButtonOnlineGame, WM_SETFONT, (WPARAM)hFont, TRUE);

		hwndButtonCampaign = CreateWindow(
			L"BUTTON",  // 按钮类名
			L"战役",  // 按钮文本
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // 按钮样式
			WindowWidth / 2 - PushButtonWidth / 2, WindowHeight / 5 + 3 * PushButtonHeight / 2 + 2 * ButtonGap,
			PushButtonWidth, PushButtonHeight,
			hwnd, (HMENU)CAMPAIGN,  // 父窗口句柄,按钮ID
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), nullptr
		);
		SendMessage(hwndButtonCampaign, WM_SETFONT, (WPARAM)hFont, TRUE);

		hwndButtonBeginGame = CreateWindow(
			L"BUTTON",  // 按钮类名
			L"开始游戏",  // 按钮文本
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // 按钮样式
			WindowWidth / 2 - PushButtonWidth - ButtonGap / 2, 3 * WindowHeight / 4 - PushButtonHeight / 2,
			PushButtonWidth, PushButtonHeight,
			hwnd, (HMENU)BEGIN_GAME,  // 父窗口句柄,按钮ID
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), nullptr
		);
		SendMessage(hwndButtonBeginGame, WM_SETFONT, (WPARAM)hFont, TRUE);

		hwndButtonBack = CreateWindow(
			L"BUTTON",  // 按钮类名
			L"返回",  // 按钮文本
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // 按钮样式
			WindowWidth / 2 + ButtonGap / 2, 3 * WindowHeight / 4 - PushButtonHeight / 2,
			PushButtonWidth, PushButtonHeight,
			hwnd, (HMENU)BACK,  // 父窗口句柄,按钮ID
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),nullptr
		);
		SendMessage(hwndButtonBack, WM_SETFONT, (WPARAM)hFont, TRUE);

		ShowWindow(hwndButtonBeginGame, SW_HIDE);
		ShowWindow(hwndButtonBack, SW_HIDE);
	}

	void radioButtonInit(HWND hwnd){
		//玩家数量信息
        ControlsInfo PlayerNumberInfo[MAX_PLAYER] = {
            {ONE_PLAYER, L"1"},		{TWO_PLAYER, L"2"},
            {THREE_PLAYER, L"3"},	{FOUR_PLAYER, L"4"},
        };
		/*单选按钮组的坐上顶点横坐标通过计算得出
		* 这里的计算方式是
		* left+(num*ButtonWidth + (num-1)*ButtonGap))/2= WindowWidth/2
		*/
		long groupLeft = (WindowWidth - MAX_PLAYER * RadioButtonWidth - (MAX_PLAYER - 1) * ButtonGap) / 2;
		CreateRadioGroupHorizontal(
			hwnd, groupLeft,100, RadioButtonWidth, RadioButtonHeight,
			MAX_PLAYER, PlayerNumberInfo, hwndRadioGroupPlayerNumber
		);

		EditInfo PlayerNumberEditInfo = {
			{PLAYER_NUMBER, L"人机数量"},
			25 * wcslen(L"人机数量")
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
		//地图大小
		ControlsInfo MapInfo[3] = {
			{SMALL_MAP, L"小"},{MEDIUM_MAP, L"中"},{LARGE_MAP, L"大"}
		};
		CreateRadioGroupHorizontal(
			hwnd, groupLeft, 100 + RadioButtonHeight + ButtonGap,
			RadioButtonWidth, RadioButtonHeight,
			3, MapInfo, hwndRadioGroupMapType
		);
		EditInfo MapEditInfo = {
			{MAP_TYPE, L"地图大小"},
			25 * wcslen(L"地图大小")
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
		//坦克颜色
		ControlsInfo TankColorInfo[5] = {
			{RED, L"红色"}, {BLUE, L"蓝色"}, {GREEN, L"绿色"}, {YELLOW, L"黄色"}, {BROWN, L"棕色"},
		};
		CreateRadioGroupHorizontal(
			hwnd, groupLeft, 100 + 2 * (RadioButtonHeight + ButtonGap),
			RadioButtonWidth, RadioButtonHeight,
			5, TankColorInfo, hwndRadioGroupTankColor
		);
		EditInfo TankColorEditInfo = {
			{TANK_COLOR, L"坦克颜色"},
			25 * wcslen(L"坦克颜色")
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
		//用于判断是否选择了游戏模式
		switch (LOWORD(wParam))
		{
		case SINGLE_GAME:
			selectionShow(hwnd);
			selectGameMode(hwnd);
			GameMode = SINGLE_GAME;
			break;
		case ONLINE_GAME:
			selectGameMode(hwnd);
			GameMode = ONLINE_GAME;
			break;
		case CAMPAIGN:
			GameMode = CAMPAIGN;
			break;
		case BEGIN_GAME:
			selectionHide(hwnd);
			switch (GameMode) 
			{
			case SINGLE_GAME:
				threadPool.addTask(gameLoop, hwnd);
				singleGameInit();
				SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)SingleGameWndProc);
				break;
			case ONLINE_GAME:
				
				SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)OnlineGameWndProc);
				break;
			case CAMPAIGN:
				SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)CampaignWndProc);
				break;
			}
			return ;
		case BACK:
			repickMode(hwnd);
			return ;
		}
		InvalidateRect(hwnd, nullptr, TRUE);
	}

	void selectGameMode(HWND hwnd) {
		menuHide(hwnd);
		
		ShowWindow(hwndButtonBeginGame, SW_SHOW);
        ShowWindow(hwndButtonBack, SW_SHOW);
	}

	void repickMode(HWND hwnd){
		GameMode = NOSELECT;

		menuShow(hwnd);

		ShowWindow(hwndButtonBeginGame, SW_HIDE);
		ShowWindow(hwndButtonBack, SW_HIDE);
		selectionHide(hwnd);
	}

	void paintControls(HWND hwnd) {
		PAINTSTRUCT ps;
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

		// 创建一个 vector 存储所有需要绘制的控件句柄
		std::vector<HWND> hwndControls = {
			hwndButtonSingleGame,
			hwndButtonOnlineGame,
			hwndButtonCampaign,
			hwndButtonBeginGame,
			hwndButtonBack,
			hwndEditPlayerNumber,
			hwndEditMapType,
			hwndEditTankColor,
			hwndRadioGroupPlayerNumber[0],hwndRadioGroupPlayerNumber[1],
			hwndRadioGroupPlayerNumber[2],hwndRadioGroupPlayerNumber[3],
			hwndRadioGroupMapType[0],hwndRadioGroupMapType[1],hwndRadioGroupMapType[2],
			hwndRadioGroupTankColor[0],hwndRadioGroupTankColor[1],
			hwndRadioGroupTankColor[2],hwndRadioGroupTankColor[3],
			hwndRadioGroupTankColor[4]
		};

		// 遍历控件并绘制
		for (HWND hwndControl : hwndControls) {
			if (IsWindowVisible(hwndControl)) {
				// 获取控件的矩形区域
				RECT controlRect;
				GetWindowRect(hwndControl, &controlRect);
				MapWindowPoints(HWND_DESKTOP, hwnd, (LPPOINT)&controlRect, 2);
				// 绘制控件
				HDC hdcControl = GetDC(hwndControl);
				BitBlt(hdcMem, controlRect.left, controlRect.top, controlRect.right - controlRect.left, controlRect.bottom - controlRect.top, hdcControl, 0, 0, SRCCOPY);
				ReleaseDC(hwndControl, hdcControl);
			}
		}

		// 将内存设备上下文的内容复制到窗口设备上下文
		//BitBlt(hdc, 0, 0, WindowWidth, WindowHeight, hdcMem, 0, 0, SRCCOPY);

		// 删除内存设备上下文和位图
		DeleteObject(hbmMem);
		DeleteDC(hdcMem);

		EndPaint(hwnd, &ps);
	}

	void paintGame(HWND hwnd) {
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

		{
			// 绘制坦克
			shared_lock<shared_mutex> lock(tpMutex);
			for (auto& Tank : TankPool) {
				Tank->draw(hdcMem);
			}
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

	void gameLoop(HWND hwnd) {
		while (Running) {
			// 标记整个窗口区域为无效，触发 WM_PAINT 消息
			InvalidateRect(hwnd, NULL, FALSE);

			// 控制帧率
			std::this_thread::sleep_for(std::chrono::milliseconds(8)); // 约60帧每秒
		}
	}

	int start(
		HINSTANCE hInstance,HINSTANCE hPrevInstance,
		LPSTR lpCmdLine,int nCmdShow)
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

		ShowWindow(hwnd, SW_SHOW);
		UpdateWindow(hwnd);

		MSG message;

		AllocConsole();
		g_hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		//char tmp[256] = { 0 };
		//sprintf_s(tmp, sizeof(tmp), "%d %d %d %d\n", LeftWall, RightWall, UpWall, BottomWall);
		//WriteConsoleA(g_hOutput, tmp, (DWORD)strlen(tmp), nullptr, nullptr);

		Running = true;

		onlineGameInit(hwnd);

		threadPool.addTask(bulletPoolUpdate);
		threadPool.addTask(TankControl);

		while (true) {
			if (PeekMessage(&message, nullptr, 0, 0, PM_NOREMOVE)) {
				if (GetMessage(&message, nullptr, 0, 0)) {
					TranslateMessage(&message);
					DispatchMessage(&message);
				}
				else {
					break;
				}
			}
			else {
				//WriteConsole(g_hOutput, L"2222", 4, nullptr,nullptr);
			}
		}
		/*这里停掉线程
		* 避免提前释放了资源,造成访问野指针
		*/

		Running = false;
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		// 释放资源
		for (auto& Tank : TankPool) {
			Tank.reset();
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

		DestroyWindow(hwnd);

		return 0;
	}
}
