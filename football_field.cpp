#include "football_field.h"
#include <QDebug>

#define keycontrast(KEY) (10 - (KEY))
#define red_win(X, Y) ((Y) == -26 && (X) <= 2 && (X) >= -2)
#define blue_win(X, Y) ((Y) == 26 && (X) <= 2 && (X) >= -2)

// Конструктор
FootballField::FootballField()
{
    // Зануляем массив.
    for(int i = 0; i < FIELD_WIDTH / PX_SCALE + 1; i++){
        for(int j = 0; j < FIELD_HEIGHT / PX_SCALE + 1; j++){
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
    current_player = 1; // 1 (blue) -1 (red)
}

// Деструктор
FootballField::~FootballField(){}

bool FootballField::try_step(int key){
    if (gameover)
        return false;

    if (penalty_mode){
        return try_penalty(key);
    }

    if (can_step(KEYS[key][0], KEYS[key][1])){
        signed char x = ball.x + KEYS[key][0],
                    y = ball.y + KEYS[key][1];
        if(red_win(x, y)){
            qDebug() << "red player winner!\n";
            gameover = true;
            show_ball = false;
        }
        if(blue_win(x, y)){
            qDebug() << "blue player winner!\n";
            gameover = true;
            show_ball = false;
        }
        if(gameover){
            steps.push_back(key * current_player);
            return true;
        }
        steps.push_back(key * current_player);
        FLD_POINT(x, y) = new FPoint(x, y);
        FLD_POINT(x, y)->push(FLD_POINT(ball.x, ball.y));
        FLD_POINT(ball.x, ball.y)->push(FLD_POINT(x, y));
        ball.step(KEYS[key][0], KEYS[key][1]);
        if(penalty_kick()){
            qDebug() << "PENALTY";
        }
        else if(current_step == 3){
            current_player *= -1;
            current_step = 1;
        }
        else
            ++current_step;

        // qDebug() << "-------- " << steps.size() << " --------";
        // qDebug() << "can_move_1x? " << can_move_1x() << "\n";
        // qDebug() << "can_move_3x? " << can_move_3x() << "\n";
        return true;
    }
    return false;
}

bool FootballField::try_penalty(int key){
    signed char x = ball.x + KEYS[key][0],
                y = ball.y + KEYS[key][1];
    // Validates
    for(int i = 0; i < 6; i++){
        if (FLD_POINT(x, y) != NULL && FLD_POINT(x, y)->is_wall)
            return false;
        if(red_win(x, y)){
            qDebug() << "red player winner!\n";
            gameover = true;
            show_ball = false;
        }
        if(blue_win(x, y)){
            qDebug() << "blue player winner!\n";
            gameover = true;
            show_ball = false;
        }
        if(gameover){
            for(; i >= 0; i--)
                steps.push_back(key * current_player);
            return true;
        }
        x += KEYS[key][0];
        y += KEYS[key][1];
    }

    // return to first point
    x = ball.x + KEYS[key][0];
    y = ball.y + KEYS[key][1];
    // Real ball moves
    for(int i = 0; i < 6; i++){
        steps.push_back(key * current_player);
        if(FLD_POINT(x, y) == NULL)
            FLD_POINT(x, y) = new FPoint(x, y);
        FLD_POINT(x, y)->push(FLD_POINT(ball.x, ball.y));
        FLD_POINT(ball.x, ball.y)->push(FLD_POINT(x, y));
        ball.step(KEYS[key][0], KEYS[key][1]);
        x += KEYS[key][0];
        y += KEYS[key][1];
    }

    if(penalty_kick()){
        qDebug() << "one more PENALTY";
    }
    else if (current_step == 3) {
        current_player *= -1;
        current_step = 1;
    }
    else
        ++current_step;
    return true;
}

bool FootballField::can_step(signed char x, signed char y){
    int new_x = ball.x + x,
        new_y = ball.y + y;
    if (FLD_POINT(new_x, new_y) != NULL && FLD_POINT(new_x, new_y)->is_wall)
        return false;
    return (FLD_POINT(new_x, new_y) == NULL) && (!(abs(x) == 1 && abs(y) == 1) || diagstep(x, y));
}

bool FootballField::diagstep(signed char x, signed char y){
    // Находим пересекаемую диагональ
    if      (x ==  1 && y ==  1)
        return can_diagstep(ball.x, ball.y + 1, ball.x + 1, ball.y);
    else if (x ==  1 && y == -1)
        return can_diagstep(ball.x + 1, ball.y, ball.x, ball.y - 1);
    else if (x == -1 && y == -1)
        return can_diagstep(ball.x, ball.y - 1, ball.x - 1, ball.y);
    else if (x == -1 && y ==  1)
        return can_diagstep(ball.x - 1, ball.y, ball.x, ball.y + 1);
    return false;
}

bool FootballField::can_diagstep(signed char bx, signed char by, signed char ex, signed char ey){
    if (FLD_POINT(bx, by) == NULL  || FLD_POINT(ex, ey) == NULL || // Если хоть одна из точек пустая
        FLD_POINT(bx, by)->is_wall || FLD_POINT(ex, ey)->is_wall)  // или является стеной
        return true;
    return !(FLD_POINT(bx, by)->include(ex, ey));
}

bool FootballField::penalty_kick(){
    if(current_step == 3){
        penalty_mode = !can_move_3x();
    }
    else {
        penalty_mode = !can_move_1x();
        if(penalty_mode){
            // You has clamped yourself!!!
            current_player *= -1;
            current_step = 3;
        }
    }
    return penalty_mode;
}

bool FootballField::can_move_1x(){
    for(int i = 1; i <= 9; i++){
        if(i == 5) continue;
        if(can_step(KEYS[i][0], KEYS[i][1])) return true;
    }
    return false;
}

bool FootballField::can_move_3x(){
    for(int i = 1; i <= 9; i++){
        if(i == 5) continue;
        for(int j = 1; j <= 9; j++){
            if(j == 5 || j == keycontrast(i)) continue;
            for(int k = 1; k <= 9; k++){
                if(k == 5 || k == keycontrast(j)) continue;

                if(!can_step(KEYS[i][0], KEYS[i][1])) continue;

                ball.step(KEYS[i][0], KEYS[i][1]);
                if(!can_step(KEYS[j][0], KEYS[j][1])){
                    ball.step(KEYS[keycontrast(i)][0], KEYS[keycontrast(i)][1]);
                    continue;
                }

                ball.step(KEYS[j][0], KEYS[j][1]);
                bool result = can_step(KEYS[k][0], KEYS[k][1]);
                ball.step(KEYS[keycontrast(j)][0], KEYS[keycontrast(j)][1]);
                ball.step(KEYS[keycontrast(i)][0], KEYS[keycontrast(i)][1]);
                if(result) return true;
            }
        }
    }
    return false;
}
