#include "bullet.h"
#include "Window.h"
#include "Map.h"
#include "Tank.h"

#include <graphics.h>
#include <memory>
#include <iostream>

namespace TankTrouble {

	list<std::shared_ptr<bullet>> bulletPool;
	std::mutex bpMutex;

	bullet::bullet(int id, int controller, point position, point direction, int size, const COLORREF& color) :
		Object(position, direction, color), radius(0), movingStep(0)
	{
		this->id = id;
		this->controller = controller;
		this->life = 600;

		if (size == SMALL_MAP) {
			this->radius = 10;
			this->movingStep = SMALL_MAP_STEP;
		}
		if (size == MEDIUM_MAP) {
			this->radius = 7;
			this->movingStep = MID_MAP_STEP;
		}
		if (size == LARGE_MAP) {
			this->radius = 4;
			this->movingStep = BIG_MAP_STEP;
		}

	}

	bullet::~bullet() {
		std::lock_guard<std::mutex > lock(tpMutex);
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

	void bullet::getTrack() {
		line l1(position, position + direction);
		point v;
		double legth = 1e18;

		auto f = [&](point a, point b) {
			line l2(a, b);
			if ((l1.v ^ l2.v) == 0) return;
			point tmp = itsLineLine(l1, l2);
			//如果不在移动方向上
			if ((l1.v * (tmp - l1.u)) < 0) return;
			//如果不在两个点之间
			if ((tmp - a) * (tmp - b) > 0) return;
			double len = distPointPoint(tmp, l1.u);
			if (len < legth) {
				v = tmp;
				legth = len;
			}
			};

		for (auto& Wall : WallPool) {
			f(Wall->LeftUp, Wall->LeftDown);
			f(Wall->RightUp, Wall->RightDown);
			f(Wall->LeftUp, Wall->RightUp);
			f(Wall->LeftDown, Wall->LeftDown);
		}

		TrackSegment = Segment{ position, v };
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

		if (invx == -1 || invy == -1) {
			getTrack();
		}

		direction.x *= invx;
		direction.y *= invy;

		if (life >= 590) return;
		{
			std::lock_guard<std::mutex> lock(tpMutex);
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
					(*Tank)->isLife = false;
					Tank = TankPool.erase(Tank);
					life = 0;
					break;
				}
				else {
					Tank++;
				}
			}
		}

	}

	int bulletPoolUpdate() {
		std::lock_guard<std::mutex> lock(bpMutex);
		for (auto bullet = bulletPool.begin(); bullet != bulletPool.end(); ) {
			(*bullet)->move();
			if ((*bullet)->getLife() == 0) {
				bullet = bulletPool.erase(bullet);
			}
			else {
				bullet++;
			}
		}
		if (Running) return 10;
		return 0;
	}

}