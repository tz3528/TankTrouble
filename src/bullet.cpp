#include "bullet.h"
#include "Window.h"
#include "Wall.h"
#include "Tank.h"
#include "point.h"

#include <graphics.h>
#include <memory>
#include <iostream>
#include <gdiplus.h>

#pragma comment (lib,"Gdiplus.lib")

using namespace Gdiplus;

namespace TankTrouble {

	std::mutex bpMutex;
	std::condition_variable bpCv;

	std::list<std::shared_ptr<bullet>> bulletPool;

	bullet::bullet(int id, int controller, point position, point direction, int size, const COLORREF& color) :
		Object(position, direction, color), radius(0), movingStep(0)
	{
		this->id = id;
		this->controller = controller;
		this->life = 800;

		if (size == 1) {
			this->radius = 8;
			this->movingStep = SMALL_MAP_STEP;
		}
		if (size == 2) {
			this->radius = 4;
			this->movingStep = MID_MAP_STEP;
		}
		if (size == 3) {
			this->radius = 2;
			this->movingStep = BIG_MAP_STEP;
		}

	}

	bullet::~bullet() {
		for (auto& Tank : TankPool) {
			if (Tank->getId() == this->id) {
				Tank->addbullet();
				break;
			}
		}
	}

	int bullet::getLife() const {
		return life;
	}

	int bullet::getController() const {
		return controller;
	}

	int bullet::getId() const {
		return id;
	}

	void bullet::draw(HDC hdcMem) {
		Ellipse(hdcMem, (int)(position.x - radius), (int)(position.y - radius), (int)(position.x + radius), (int)(position.y + radius));
	}

	void bullet::move() {
		position = position + direction * movingStep;
		life--;

		int invx = 1, invy = 1;
		for (auto& wall : WallPool) {
			if (distPointSeg(position, wall->LeftUp, wall->LeftDown) <= radius ||
				distPointSeg(position, wall->RightUp, wall->RightDown) <= radius) {
				invx = -1;
			}
			if (
				distPointSeg(position, wall->LeftUp, wall->RightUp) <= radius ||
				distPointSeg(position, wall->LeftDown, wall->RightDown) <= radius) {
				invy = -1;
			}

		}
		direction.x *= invx;
		direction.y *= invy;

		if (life >= 790) return;
		for (auto Tank = TankPool.begin();Tank != TankPool.end();) {
			bool IsCollision = false;
			for (int i = 0;i < 4;i++) {
				if (distPointSeg(position, (*Tank)->body[i], (*Tank)->body[(i + 1) % 4]) <= radius ||
					distPointSeg(position, (*Tank)->barrel[i], (*Tank)->barrel[(i + 1) % 4]) <= radius) {
					IsCollision = true;
					break;
				}
			}
			if (IsCollision) {
				Tank = TankPool.erase(Tank);
				life = 0;
				break;
			}
			else {
				Tank++;
			}
		}

	}

	void bulletPoolUpdate() {
		while (1) {
			{
				std::unique_lock<std::mutex> lock(bpMutex);
				bpCv.wait(lock, [] {return !bulletPool.empty();});
			}

			{
				std::unique_lock<std::mutex> lock(bpMutex);
				for (auto bullet = bulletPool.begin(); bullet != bulletPool.end(); ) {
					(*bullet)->move();
					if ((*bullet)->getLife() == 0) {
						bullet = bulletPool.erase(bullet);
					}
					else {
						bullet++;
					}
				}
				HWND hwnd = FindWindow(L"TankTrouble", nullptr);
				if (hwnd) {
					InvalidateRect(hwnd, nullptr, TRUE);
				}
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}

}
