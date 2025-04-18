#include "SingleGame/SingleGame.h"
#include "Map.h"
#include "Tank.h"
#include "bullet.h"
#include "Window.h"
#include "SingleGame/AI.h"

using std::make_shared;

namespace TankTrouble 
{

	LRESULT CALLBACK SingleGameWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		char tmp[256] = { 0 };
			switch (message)
		{
		case WM_KEYDOWN:
			keyDown(hwnd, wParam);
			break;
		case WM_KEYUP:
			keyUp(hwnd, wParam);
			break;
		case WM_COMMAND:
			buttonDown(hwnd, wParam);
			break;
		case WM_PAINT:
			paintGame(hwnd);
			break;
		case WM_ERASEBKGND: {
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

	void singleGameInit() {
		//创建地图边缘的墙
		WallPool.emplace_back(make_shared<Wall>
			(point{ LeftWall ,UpWall }, point{ LeftWall,BottomWall }, MapSize)
		);
		WallPool.emplace_back(make_shared<Wall>
			(point{ RightWall ,UpWall }, point{ RightWall,BottomWall }, MapSize)
		);
		WallPool.emplace_back(make_shared<Wall>
			(point{ LeftWall ,UpWall }, point{ RightWall,UpWall }, MapSize)
		);
		WallPool.emplace_back(make_shared<Wall>
			(point{ LeftWall ,BottomWall }, point{ RightWall,BottomWall }, MapSize)
		);

		GenerateMap(MapSize);
		shuffle(permutation + 1, permutation + Row * Column + 1, gen);
		
		{
			std::lock_guard<std::mutex> lock(tpMutex);

			TankPool.emplace_back(make_shared<Tank>(0, PLAYER, getCentrePoint(NtoP(permutation[1])),
			directions[IntervalRN(gen)], MapSize, PlayerColor));
		
			for (int i = 1;i <= computers;i++) {
				TankPool.emplace_back(make_shared<Tank>(i, COMPUTER, getCentrePoint(NtoP(permutation[i+1])),
					directions[IntervalRN(gen)], MapSize, LIGHTGRAY));
				threadPool.addTask(AIControl, TankPool.back());
			}
		}
		
	}

	void singleGameRestart() {
		// 释放资源
		{
			std::lock_guard<std::mutex> lock(tpMutex);
			for (auto& tank : TankPool) {
				tank.reset();
			}
			TankPool.clear();
		}
		
		for (auto& bullet : bulletPool) {
			bullet.reset();
		}
		bulletPool.clear();

		for (auto& wall : WallPool) {
			wall.reset();
		}
		WallPool.clear();

        singleGameInit();
	}

	void keyDown(HWND hwnd, WPARAM wParam) {
		for (auto& Tank : TankPool) {
			if (Tank->getController() == COMPUTER)
				continue;
			{
                std::lock_guard<std::mutex> lock(tpMutex);
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
	}

	void keyUp(HWND hwnd, WPARAM wParam) {
		for (auto& Tank : TankPool) {
			if (Tank->getController() == COMPUTER)
				continue;
			{
				std::lock_guard<std::mutex> lock(tpMutex);
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
	}

	static void buttonDown(HWND hwnd, WPARAM wParam) {
		switch (LOWORD(wParam))
		{
		case BEGIN_GAME:
			selectionHide(hwnd);
			ShowWindow(hwndButtonBeginGame, SW_HIDE);
			ShowWindow(hwndButtonBack, SW_HIDE);
			singleGameInit();
			threadPool.addTask(gameLoop, hwnd);
			break;
		case BACK:
			repickMode(hwnd);
			SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)StartWndProc);
			break;
		}
	}

}
