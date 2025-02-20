#include "SingleGame.h"
#include "Map.h"
#include "Tank.h"
#include "bullet.h"
#include "Window.h"

using std::make_shared;

namespace TankTrouble 
{

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
		TankPool.emplace_back(std::make_shared<Tank>(0, 0, point(2 * WindowWidth / 20, 2 * WindowHeight / 20),
			point(1, 0), MapSize, PlayerColor));
		GenerateMap(MapSize);
	}

	void singleGameRestart() {
		// 释放资源
		for (auto& tank : TankPool) {
			tank.reset();
		}
		TankPool.clear();

		{
			std::unique_lock<std::mutex> lock(bpMutex);
			for (auto& bullet : bulletPool) {
				bullet.reset();
			}
			bulletPool.clear();
		}

		bpCv.notify_one();

		

		for (auto& wall : WallPool) {
			wall.reset();
		}
		WallPool.clear();

        singleGameInit();
	}

}
