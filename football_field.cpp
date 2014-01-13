#include "football_field.h"

// Конструктор
FootballField::FootballField()
{
    // Зануляем массив.
    for(int i = 0; i < FIELD_WIDTH / PX_SCALE; i++){
        for(int j = 0; j < FIELD_HEIGHT / PX_SCALE; j++){
            points[i][j] = NULL;
        }
    }

    // Стартовая точка.
    FLD_POINT(0, 0) = new FPoint(0, 0);

    // Создаём ворота.
    FLD_POINT(-3, -25) = new FPoint(-3, -25, true);
    FLD_POINT( 3, -25) = new FPoint( 3, -25, true);
    FLD_POINT(-3,  25) = new FPoint(-3,  25, true);
    FLD_POINT( 3,  25) = new FPoint( 3,  25, true);

    // Создаём коробку (окружающие поле стены).
    for(int y = -26; y <= 26; y++){
        FLD_POINT(-20, y) = new FPoint(-20, y, true);
        FLD_POINT( 20, y) = new FPoint(20, y, true);
        if((y > -20 && y < -2) || (y > 2 && y < 20)){
            FLD_POINT(y,  26) = new FPoint(y, 26, true);
            FLD_POINT(y, -26) = new FPoint(y, -26, true);
        }
    }

    // Мячик изначально в позиции 0:0
}

// Деструктор
FootballField::~FootballField(){}

bool FootballField::try_step(int key){
    if (can_step(KEYS[key][0], KEYS[key][1])){
        signed char x = ball.x + KEYS[key][0],
                    y = ball.y + KEYS[key][1];
        steps.push_back(key);
        FLD_POINT(x, y) = new FPoint(x, y);
        FLD_POINT(x, y)->push(FLD_POINT(ball.x, ball.y));
        FLD_POINT(ball.x, ball.y)->push(FLD_POINT(x, y));
        ball.step(KEYS[key][0], KEYS[key][1]);
        return true;
    }
    return false;
}

bool FootballField::can_step(signed char x, signed char y){
    int new_x = ball.x + x,
        new_y = ball.y + y;
    if (FLD_POINT(new_x, new_y) != NULL && FLD_POINT(new_x, new_y)->is_wall)
        return false;
    return (FLD_POINT(new_x, new_y) == NULL) && (!(abs(x) == 1 && abs(y) == 1) || diagstep(x, y));
}

bool FootballField::diagstep(signed char x, signed char y){
    int new_x = ball.x + x,
        new_y = ball.y + y;
    // Находим пересекаемую диагональ
    if      (x ==  1 && y ==  1)
        return can_diagstep(new_x, new_y + 1, new_x + 1, new_y);
    else if (x ==  1 && y == -1)
        return can_diagstep(new_x + 1, new_y, new_x, new_y - 1);
    else if (x == -1 && y == -1)
        return can_diagstep(new_x, new_y - 1, new_x - 1, new_y);
    else if (x == -1 && y ==  1)
        return can_diagstep(new_x - 1, new_y, new_x, new_y + 1);
    return false;
}

bool FootballField::can_diagstep(signed char bx, signed char by, signed char ex, signed char ey){
    if (FLD_POINT(bx, by) == NULL  || FLD_POINT(ex, ey) == NULL || // Если хоть одна из точек пустая
        FLD_POINT(bx, by)->is_wall || FLD_POINT(ex, ey)->is_wall)  // или является стеной
        return true;
    return !(FLD_POINT(bx, by)->include(ex, ey));
}
