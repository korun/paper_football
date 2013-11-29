#ifndef FPOINT_H
#define FPOINT_H

#include <QMainWindow>

namespace Ui {
    class FPoint;
}

class FPoint : public QPoint
{

public:
    FPoint(int x, int y, bool is_wall);
    ~FPoint();
    int x;
    int y;
    bool is_wall;
    FPoint *points[8];
};

#endif // FPOINT_H
