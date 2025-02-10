#include "Object.h"

namespace TankTrouble
{
    Object::Object(point position, point direction, COLORREF color) {
        this->position = position;
        this->direction = direction;
        this->color = color;
    }
}
