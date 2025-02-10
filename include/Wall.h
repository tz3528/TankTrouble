#ifndef WALL_H
#define WALL_H

#include "point.h"

#include <list>
#include <memory>
#include <Windows.h>

using std::list, std::shared_ptr, std::pair;

namespace TankTrouble
{
	class Wall
	{
	public:
		Wall(point u, point v, int size);
		~Wall();

		void draw(HDC hdcMem);

		double HalfWidth;
		point LeftUp, LeftDown, RightUp, RightDown;
	};

	extern list<shared_ptr<Wall>> WallPool;

}

#endif // WALL_H
