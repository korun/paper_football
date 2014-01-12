#include "fpoint.h"

FPoint::FPoint(int new_x, int new_y, bool wall):
    x(new_x),
    y(new_y),
    is_wall(wall)
{
}

FPoint::~FPoint(){}

bool FPoint::include(int x, int y){
    for(int i = 0; i < 8; i++){
        if (points[i] != NULL && points[i]->x == x && points[i]->y == y)
            return true;
    }
    return false;
}

bool FPoint::push(FPoint *point){
    if(point == NULL)
        return false;
    for(int i = 0; i < 8; i++){
        if (points[i] == NULL){
            points[i] = point;
            return true;
        }
        else if (points[i]->x == point->x && points[i]->y == point->y)
            return true;
    }
    return false;
}
