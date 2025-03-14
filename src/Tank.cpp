#include "Tank.h"

#include <graphics.h>
#include <windows.h>
#include <mmsystem.h>

#pragma comment(lib, "Winmm.lib")

namespace TankTrouble {

	list<std::shared_ptr<Tank>> TankPool;
	shared_mutex tpMutex;

	Tank::Tank(int id, int controller, point position, point direction, int size, const COLORREF& color) :
		Object(position, direction, color) {
		this->id = id;
		this->controller = controller;
		this->size = size;
		movingStep = 3;

		if (this->size == SMALL_MAP) {
			length = 4 * WindowWidth / 60.0;
			width = 3 * WindowWidth / 60.0;
		}
		if (this->size == MEDIUM_MAP) {
			length = 4 * WindowWidth / 65.0;
			width = 3 * WindowWidth / 65.0;
		}
		if (this->size == LARGE_MAP) {
			length = 4 * WindowWidth / 80.0;
			width = 3 * WindowWidth / 80.0;
		}

		getTank();

		getGridPosition();

	}
	Tank::~Tank() {
		PlaySoundW(L"res/Sound Effects/TankExplosion.wav", NULL, SND_FILENAME | SND_ASYNC);
	}

	void Tank::getTank() {
		body[0] = position + direction * length / 2 + direction.normalVector() * width / 2;
		body[1] = position + direction * length / 2 - direction.normalVector() * width / 2;
		body[2] = position - direction * length / 2 - direction.normalVector() * width / 2;
		body[3] = position - direction * length / 2 + direction.normalVector() * width / 2;
		barrel[0] = position + direction * length * 3 / 4 + direction.normalVector() * width / 6;
		barrel[1] = position + direction * length * 3 / 4 - direction.normalVector() * width / 6;
		barrel[2] = position - direction * length / 6 - direction.normalVector() * width / 6;
		barrel[3] = position - direction * length / 6 + direction.normalVector() * width / 6;
	}

	void Tank::forward() {
		tmpPosition = position;
		tmpDirection = direction;
		position = position + direction * movingStep;
		getTank();
		if (Collision()) {
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
		if (Collision()) {
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
				direction = direction + normalVector / RotationRatio;
			}
			else {
				direction = direction - normalVector / RotationRatio;
			}
		}
		else {
			if (isBackward) {
				direction = direction - normalVector / RotationRatio;
			}
			else {
				direction = direction + normalVector / RotationRatio;
			}
		}
		direction = direction / norm(direction);
		getTank();
		if (Collision()) {
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
				direction = direction - normalVector / RotationRatio;
			}
			else {
				direction = direction + normalVector / RotationRatio;
			}

		}
		else {
			if (isBackward) {
				direction = direction + normalVector / RotationRatio;
			}
			else {
				direction = direction - normalVector / RotationRatio;
			}
		}
		direction = direction / norm(direction);
		getTank();
		if (Collision()) {
			position = tmpPosition;
			direction = tmpDirection;
			getTank();
		}
	}

	void Tank::attack() {
		if (bullets == 0) return;
		point bpos = position + direction * length * 3 / 4;

		if (bpos.x < LeftWall ||
			bpos.x > RightWall ||
			bpos.y < UpWall ||
			bpos.y > BottomWall)
			return;

		auto now = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastAttack).count();
		if (duration < attackInterval) return;
		lastAttack = now;

		bullets--;

		bulletPool.emplace_back(std::make_shared<bullet>(id, controller, bpos, direction, size, BLACK));

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

	bool Tank::Collision() const {
		for (int i = 0;i < 4;i++) {
			for (auto& Tank : TankPool) {
				if (Tank->getId() == id) {
					continue;
				}
				if (itsPolPol(barrel, 4, Tank->barrel, 4)) {
					return true;
				}
				if (itsPolPol(body, 4, Tank->body, 4)) {
					return true;
				}
                if (itsPolPol(barrel, 4, Tank->body, 4)) {
					return true;
				}
                if (itsPolPol(body, 4, Tank->barrel, 4)) {
					return true;
				}
			}
			for (auto& wall : WallPool) {
				if (itsSegSeg(barrel[i], barrel[(i + 1) % 4],
					wall->LeftUp, wall->LeftDown))
					return true;
				if (itsSegSeg(barrel[i], barrel[(i + 1) % 4],
					wall->RightUp, wall->RightDown))
					return true;
				if (itsSegSeg(barrel[i], barrel[(i + 1) % 4],
					wall->LeftUp, wall->RightUp))
					return true;
				if (itsSegSeg(barrel[i], barrel[(i + 1) % 4],
					wall->LeftDown, wall->RightDown))
					return true;

				if (itsSegSeg(body[i], body[(i + 1) % 4],
					wall->LeftUp, wall->LeftDown))
					return true;
				if (itsSegSeg(body[i], body[(i + 1) % 4],
					wall->RightUp, wall->RightDown))
					return true;
				if (itsSegSeg(body[i], body[(i + 1) % 4],
					wall->LeftUp, wall->RightUp))
					return true;
				if (itsSegSeg(body[i], body[(i + 1) % 4],
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

	point Tank::getposition() const {
		return position;
	}

	void Tank::getGridPosition(){
		this->pos = {
			(int)((position.y - UpWall) / yGap) + 1,
			(int)((position.x - LeftWall) / xGap) + 1
		};
		
		return ;
	}

	void Tank::setColor(COLORREF newColor) {
		color = newColor;
	}

	void TankControl() {
		while (Running) {
			{
				std::unique_lock<std::shared_mutex> lock(tpMutex);

				for (auto& Tank : TankPool) {
					if (Tank == nullptr) return;

					//没有任何指令
					if (!Tank->isForward && !Tank->isBackward &&
						!Tank->isLeft && !Tank->isRight && !Tank->isAttack)
						continue;

					if (Tank->isAttack) {
						Tank->attack();
					}
					if (Tank->isForward && !Tank->isBackward) {
						Tank->forward();
					}
					if (!Tank->isForward && Tank->isBackward) {
						Tank->backwards();
					}
					if (Tank->isLeft && !Tank->isRight) {
						Tank->left();
					}
					if (!Tank->isLeft && Tank->isRight) {
						Tank->right();
					}

					Tank->getGridPosition();

					HWND hwnd = FindWindow(L"TankTrouble", nullptr);
					if (hwnd) {
						InvalidateRect(hwnd, nullptr, TRUE);
					}

				}
			}
			
			std::this_thread::sleep_for(std::chrono::milliseconds(40));

		}
	}

}
