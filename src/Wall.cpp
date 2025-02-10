#include "Wall.h"
#include "Window.h"

#include <stdexcept>

using std::swap, std::invalid_argument;

namespace TankTrouble {

    list<shared_ptr<Wall>> WallPool;

    Wall::Wall(point u, point v, int size) {
        if (u == v) {
            throw invalid_argument("u and v cannot be the same point");
        }

        if (size == 1) {
            this->HalfWidth = 8;
        }
        if (size == 2) {
            this->HalfWidth = 4;
        }
        if (size == 3) {
            this->HalfWidth = 2;
        }

        if (u.x == v.x) {
            if (u.y < v.y) swap(u, v);
            LeftUp = u + point{ -HalfWidth, -HalfWidth };
            RightUp = u + point{ HalfWidth, -HalfWidth };
            LeftDown = v + point{ -HalfWidth, HalfWidth };
            RightDown = v + point{ HalfWidth, HalfWidth };
        }
        else if (u.y == v.y) {
            if (u.x > v.x) swap(u, v);
            LeftUp = u + point{ -HalfWidth, -HalfWidth };
            RightUp = v + point{ HalfWidth, -HalfWidth };
            LeftDown = u + point{ -HalfWidth, HalfWidth };
            RightDown = v + point{ HalfWidth, HalfWidth };
        }
        else {
            throw invalid_argument("u or v must have the same coordinate in one dimension.");
        }

    }

    Wall::~Wall() {
    }

    void Wall::draw(HDC hdcMem) {
        Rectangle(hdcMem, (int)LeftUp.x, (int)LeftUp.y, (int)RightDown.x, (int)RightDown.y);
    }
}
