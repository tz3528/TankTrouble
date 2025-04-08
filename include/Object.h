#ifndef OBJECT_H
#define OBJECT_H

#include "pch.h"

namespace TankTrouble {
	class Object
	{
	public:
		Object(point position, point direction, COLORREF color);
		virtual void draw(HDC hdcMem) = 0;

		point position, direction;
	protected:
		COLORREF color;
	};
}


#endif //OBJECT_H
