#ifndef FPOINT_H
#define FPOINT_H

#include <QMainWindow>

namespace Ui {
    class FPoint;
}

class FPoint : public QPoint
{

public:
    int     x;
    int     y;
    bool    is_wall;
    FPoint *points[8];

    FPoint(int x, int y, bool is_wall = false);
    ~FPoint();
    bool include(int x, int y);
    bool push(FPoint *point);
};

#endif // FPOINT_H
