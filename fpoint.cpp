#include "fpoint.h"

FPoint::FPoint(int new_x, int new_y, bool wall):
    x(new_x),
    y(new_y),
    is_wall(wall)
{
}

FPoint::~FPoint(){}
