#include "Tank.h"
#include "bullet.h"
#include "Window.h"
#include "Wall.h"
#include "point.h"

#include <graphics.h>
#include <windows.h>
#include <mmsystem.h>
#include <gdiplus.h>

#pragma comment (lib,"Gdiplus.lib")
#pragma comment(lib, "Winmm.lib")
using namespace Gdiplus;

namespace TankTrouble {

	std::list<std::shared_ptr<Tank>> TankPool;

	Tank::Tank(int id, int controller, point position, point direction, int size, const COLORREF& color) :
		Object(position, direction, color) {
		this->id = id;
		this->controller = controller;
		this->size = size;
		movingStep = 2;

		if (this->size == 1) {
			length = 4 * WindowWidth / 60.0;
			width = 3 * WindowWidth / 60.0;
		}
		if (this->size == 2) {
			length = 4 * WindowWidth / 80.0;
			width = 3 * WindowWidth / 80.0;
		}
		if (this->size == 3) {
			length = 4 * WindowWidth / 100.0;
			width = 3 * WindowWidth / 100.0;
		}

		getTank();

	}
	Tank::~Tank() {
		PlaySoundW(L"res/Sound Effects/TankExplosion.wav", NULL, SND_FILENAME | SND_ASYNC);
	}

	void Tank::getTank() {
		body[0] = position + direction * length / 2 + direction.normalVector() * width / 2;
		body[1] = position + direction * length / 2 - direction.normalVector() * width / 2;
		body[2] = position - direction * length / 2 - direction.normalVector() * width / 2;
		body[3] = position - direction * length / 2 + direction.normalVector() * width / 2;
		barrel[0] = position + direction * length * 3 / 4 + direction.normalVector() * width / 4;
		barrel[1] = position + direction * length * 3 / 4 - direction.normalVector() * width / 4;
		barrel[2] = position - direction * length / 6 - direction.normalVector() * width / 4;
		barrel[3] = position - direction * length / 6 + direction.normalVector() * width / 4;
	}

	void Tank::forward() {
		tmpPosition = position;
		tmpDirection = direction;
		position = position + direction * movingStep;
		getTank();
		if (CollisionWall()) {
			position = tmpPosition;
			direction = tmpDirection;
			getTank();
		}
	}

	void Tank::backwards() {
		tmpPosition = position;
		tmpDirection = direction;
		position = position - direction * movingStep;
		getTank();
		if (CollisionWall()) {
			position = tmpPosition;
			direction = tmpDirection;
			getTank();
		}
	}

	void Tank::left() {
		tmpPosition = position;
		tmpDirection = direction;
		point normalVector = direction.normalVector();
		if ((direction ^ normalVector) > 0) {
			if (isBackward) {
				direction = direction + normalVector / 40;
			}
			else {
				direction = direction - normalVector / 40;
			}
		}
		else {
			if (isBackward) {
				direction = direction - normalVector / 40;
			}
			else {
				direction = direction + normalVector / 40;
			}
		}
		direction = direction / norm(direction);
		getTank();
		if (CollisionWall()) {
			position = tmpPosition;
			direction = tmpDirection;
			getTank();
		}
	}

	void Tank::right() {
		tmpPosition = position;
		tmpDirection = direction;
		point normalVector = direction.normalVector();
		if ((direction ^ normalVector) > 0) {
			if (isBackward) {
				direction = direction - normalVector / 40;
			}
			else {
				direction = direction + normalVector / 40;
			}

		}
		else {
			if (isBackward) {
				direction = direction + normalVector / 40;
			}
			else {
				direction = direction - normalVector / 40;
			}
		}
		direction = direction / norm(direction);
		getTank();
		if (CollisionWall()) {
			position = tmpPosition;
			direction = tmpDirection;
			getTank();
		}
	}

	void Tank::attack() {
		if (bullets == 0) return;
		point bpos = position + direction * length * 3 / 4;

		//HANDLE g_hOutput = 0;
		//g_hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  //      char tmp[256] = {0};
  //      sprintf_s(tmp, sizeof(tmp), "%d %d %d %d %d %d\n", (int)bpos.x, (int)bpos.y,LeftWall, RightWall, UpWall, BottomWall);
  //      WriteConsoleA(g_hOutput, tmp, (DWORD)strlen(tmp), nullptr, nullptr);

		if (bpos.x < LeftWall ||
			bpos.x > RightWall ||
			bpos.y < UpWall ||
			bpos.y > BottomWall)
			return;
		bullets--;

		{
			std::unique_lock<std::mutex> lock(bpMutex);
			bulletPool.emplace_back(std::make_shared<bullet>(id, controller, bpos, direction, size, BLACK));
		}

		bpCv.notify_one();

	}

	void Tank::addbullet() {
		bullets++;
	}

	void Tank::draw(HDC hdcMem) {
		HBRUSH brush = CreateSolidBrush(color);
		HBRUSH oldBrush = (HBRUSH)SelectObject(hdcMem, brush);
		Polygon(hdcMem, toPOINT(body, 4), 4);
		Polygon(hdcMem, toPOINT(barrel, 4), 4);
	}

	bool Tank::CollisionWall() {
		for (int i = 0;i < 4;i++) {
			for (auto& wall : WallPool) {
				if (IntersectSegSeg(barrel[i], barrel[(i + 1) % 4],
					wall->LeftUp, wall->LeftDown))
					return true;
				if (IntersectSegSeg(barrel[i], barrel[(i + 1) % 4],
					wall->RightUp, wall->RightDown))
					return true;
				if (IntersectSegSeg(barrel[i], barrel[(i + 1) % 4],
					wall->LeftUp, wall->RightUp))
					return true;
				if (IntersectSegSeg(barrel[i], barrel[(i + 1) % 4],
					wall->LeftDown, wall->RightDown))
					return true;

				if (IntersectSegSeg(body[i], body[(i + 1) % 4],
					wall->LeftUp, wall->LeftDown))
					return true;
				if (IntersectSegSeg(body[i], body[(i + 1) % 4],
					wall->RightUp, wall->RightDown))
					return true;
				if (IntersectSegSeg(body[i], body[(i + 1) % 4],
					wall->LeftUp, wall->RightUp))
					return true;
				if (IntersectSegSeg(body[i], body[(i + 1) % 4],
					wall->LeftDown, wall->RightDown))
					return true;
			}
		}
		return false;
	}

	int Tank::getId() const {
		return id;
	}

	int Tank::getController() const {
		return controller;
	}

	void Tank::setColor(COLORREF newColor) {
		color = newColor;
	}

	ExMessage m = {};

	void TankControl() {
		while (1) {
			for (auto& Tank : TankPool) {
				if (Tank->getController() == PLAYER) {
					PlayerControl(Tank.get());
				}
				else {
					ComputerControl(Tank.get());
				}
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(40));

		}
	}

	void PlayerControl(Tank* Player) {
		if (Player == nullptr) return;

		//没有任何指令
		if (!Player->isForward && !Player->isBackward &&
			!Player->isLeft && !Player->isRight && !Player->isAttack)
			return;

		if (Player->isAttack) {
			Player->attack();
		}
		if (Player->isForward && !Player->isBackward) {
			Player->forward();
		}
		if (!Player->isForward && Player->isBackward) {
			Player->backwards();
		}
		if (Player->isLeft && !Player->isRight) {
			Player->left();
		}
		if (!Player->isLeft && Player->isRight) {
			Player->right();
		}
		HWND hwnd = FindWindow(L"TankTrouble", nullptr);
		if (hwnd) {
			InvalidateRect(hwnd, nullptr, TRUE);
		}
	}

	void ComputerControl(Tank* Computer) {

	}

}
