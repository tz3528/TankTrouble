#ifndef OBJECT_H
#define OBJECT_H

#include <windows.h>

#include "point.h"

namespace TankTrouble {
	class Object
	{
	public:
		Object(point position, point direction, COLORREF color);
		virtual void draw(HDC hdcMem) = 0;
	protected:
		point position, direction;
		COLORREF color;
	};
}


#endif //OBJECT_H
