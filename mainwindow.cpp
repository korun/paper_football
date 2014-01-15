#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    field = new FootballField;
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete field;
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *e){
    bool success = false;
    signed char pl = field->current_player;
    switch(e->key()){
        case Qt::Key_Z:
        case Qt::Key_1:
            success = field->try_step(1);
            break;
        case Qt::Key_X:
        case Qt::Key_2:
            success = field->try_step(2);
            break;
        case Qt::Key_C:
        case Qt::Key_3:
            success = field->try_step(3);
            break;
        case Qt::Key_A:
        case Qt::Key_4:
            success = field->try_step(4);
            break;
        case Qt::Key_D:
        case Qt::Key_6:
            success = field->try_step(6);
            break;
        case Qt::Key_Q:
        case Qt::Key_7:
            success = field->try_step(7);
            break;
        case Qt::Key_W:
        case Qt::Key_8:
            success = field->try_step(8);
            break;
        case Qt::Key_E:
        case Qt::Key_9:
            success = field->try_step(9);
    }
    if (success){
        show_pointer = false;
        this->repaint();
        if (field->winner){
            QMessageBox::information(this, tr("Game over"), tr(field->winner > 0 ? "Blue player win!" : "Red player win!"));
        }
        else {
            if(pl != field->current_player){
                if(field->current_player < 0)
                    QMessageBox::information(this, tr("Information"), tr("Red player kicks"));
                else
                    QMessageBox::information(this, tr("Information"), tr("Blue player kicks"));
            }
            if(field->penalty_mode){
                QMessageBox::information(this, tr("Information"), tr("PENALTY!"));
            }
            else{
                int direction = field->only_one_way();
                if(direction){
                    qDebug() << "Auto move\n";
                    QKeyEvent key(QEvent::KeyPress, Qt::Key_0 + direction, Qt::NoModifier);
                    QApplication::sendEvent(this, &key);
                    if (!key.isAccepted()) qDebug() << "something wrong\n";
                }
            }
        }
    }
}

void MainWindow::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QPen light_blue_pen(QColor("#add8e6"), 1);
    QPen blue_pen(QColor("#00f"), 1);
    QPen red_pen(QColor("#f00"), 1);
    QPen std_pen(QColor("#000"), 1);
    QPen tip_pen(QColor("#be00be"), 1);

    int top_offset = ui->menuBar->height();

    // Рисуем сетку
    painter.setPen(light_blue_pen);
    for(int i = 0; i < FIELD_WIDTH / PX_SCALE; i++){
        painter.drawLine(PX_SCALE * i, top_offset, PX_SCALE * i, top_offset + FIELD_HEIGHT);
    }
    for(int i = 0; i < FIELD_HEIGHT / PX_SCALE; i++){
        painter.drawLine(0, top_offset + PX_SCALE * i, FIELD_WIDTH, top_offset + PX_SCALE * i);
    }

    // Рисуем ворота синих
    painter.setPen(blue_pen);
    painter.drawLine(FIELD_WIDTH / 2 - PX_SCALE * 3, top_offset + FIELD_HEIGHT, FIELD_WIDTH / 2 - PX_SCALE * 3, top_offset + FIELD_HEIGHT - PX_SCALE);
    painter.drawLine(FIELD_WIDTH / 2 + PX_SCALE * 3, top_offset + FIELD_HEIGHT, FIELD_WIDTH / 2 + PX_SCALE * 3, top_offset + FIELD_HEIGHT - PX_SCALE);

    // Рисуем ворота красных
    painter.setPen(red_pen);
    painter.drawLine(FIELD_WIDTH / 2 - PX_SCALE * 3, top_offset, FIELD_WIDTH / 2 - PX_SCALE * 3, top_offset + PX_SCALE);
    painter.drawLine(FIELD_WIDTH / 2 + PX_SCALE * 3, top_offset, FIELD_WIDTH / 2 + PX_SCALE * 3, top_offset + PX_SCALE);

    // Рисуем стартовую точку, и точки в воротах
    painter.setPen(std_pen);
    painter.drawEllipse(QPointF(FIELD_WIDTH / 2, top_offset + 1), 1, 1);
    painter.drawEllipse(QPointF(FIELD_WIDTH / 2, top_offset + FIELD_HEIGHT - 1), 1, 1);
    painter.drawEllipse(QPointF(FIELD_WIDTH / 2, top_offset + FIELD_HEIGHT / 2), 1, 1);

    int old_x = FIELD_WIDTH  / 2,
        old_y = FIELD_HEIGHT / 2;
    int loop = 1;
    std::vector<signed char>::iterator iterator = field->steps.begin();
    while (iterator != field->steps.end()) {
        if(*iterator == PENALTY_SIGN){
            loop = 6;
        }
        else {
            int index = abs((int) *iterator);
            painter.setPen(*iterator < 0 ? red_pen : blue_pen);

            for(; loop > 0; loop--){
                int x = old_x - PX_SCALE * field->KEYS[index][0];
                int y = old_y - PX_SCALE * field->KEYS[index][1];
                painter.drawLine(old_x, top_offset + old_y, x, top_offset + y);
                old_x = x;
                old_y = y;
            }
            loop = 1;
        }
        ++iterator;
    }

    // Ball marker
    if(field->show_ball){
        painter.setPen(std_pen);
        painter.drawEllipse(QPointF(old_x, top_offset + old_y), 2, 2);
    }

    if(show_pointer && field->winner == 0){
        painter.setPen(tip_pen);
        // Without top_offset!
        painter.drawEllipse(QPointF(mouse_pointer.x(), mouse_pointer.y()), 2, 2);

        int ball_x = field->ball.x,
            ball_y = field->ball.y;

        for(int i = 0; i < 6; i++){
            int index = get_key_from_coord(mouse_pointer.x(), mouse_pointer.y(), old_x, top_offset + old_y);
            if(field->can_penalty(index) || field->can_step_from(field->KEYS[index][0], field->KEYS[index][1], ball_x, ball_y)){
                int x = old_x - PX_SCALE * field->KEYS[index][0];
                int y = old_y - PX_SCALE * field->KEYS[index][1];
                painter.drawLine(old_x, top_offset + old_y, x, top_offset + y);
                old_x = x;
                old_y = y;
                ball_x += field->KEYS[index][0];
                ball_y += field->KEYS[index][1];
            }
            else
                break;
        }
    }

}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    int top_offset = ui->menuBar->height();
    int mx = event->pos().x(),
        my = event->pos().y();
    const int bx = FIELD_WIDTH  / 2 - PX_SCALE * field->ball.x,
              by = FIELD_HEIGHT / 2 - PX_SCALE * field->ball.y + top_offset,
              cells_count = (field->penalty_mode ? 6 : (3 - field->current_step + 1)); // because current_step start from 1
    if((mx <= bx + PX_SCALE * cells_count + PX_SCALE / 3 && mx >= bx - PX_SCALE * cells_count - PX_SCALE / 3) && // mouse x in -3..3 or -6..6
       (my <= by + PX_SCALE * cells_count + PX_SCALE / 3 && my >= by - PX_SCALE * cells_count - PX_SCALE / 3)    // mouse y in -3..3 or -6..6
    ){
        const int local_mx = mx % PX_SCALE,
                  local_my = (my - top_offset) % PX_SCALE;
        if((local_mx <= PX_SCALE / 3 || local_mx >= PX_SCALE / 3 * 2) && // x and y can cling to grid point
           (local_my <= PX_SCALE / 3 || local_my >= PX_SCALE / 3 * 2)
        ){
            if(local_mx <= PX_SCALE / 3)
                mx -= local_mx;
            else // if(local_mx >= PX_SCALE / 3 * 2)
                mx += PX_SCALE - local_mx;
            if(local_my <= PX_SCALE / 3)
                my -= local_my;
            else // if(local_my >= PX_SCALE / 3 * 2)
                my += PX_SCALE - local_my;

            show_pointer = true;

            if(field->penalty_mode){
                int index = get_key_from_coord(mx, my, bx, by);

                mx = bx;
                my = by;
                for(int i = 0; i < 6; i++){
                    mx = mx - PX_SCALE * field->KEYS[index][0];
                    my = my - PX_SCALE * field->KEYS[index][1];
                }
            }
            mouse_pointer.setX(mx);
            mouse_pointer.setY(my);
            this->repaint();
        }
        return;
    }
    if(show_pointer){
        show_pointer = false;
        this->repaint();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    if(show_pointer && field->winner == 0 && event->button() == Qt::LeftButton){
        int top_offset = ui->menuBar->height();
        int need_steps;
        int bx = FIELD_WIDTH  / 2 - PX_SCALE * field->ball.x,
            by = FIELD_HEIGHT / 2 - PX_SCALE * field->ball.y + top_offset;
        int s_x = abs(bx - mouse_pointer.x());
        int s_y = abs(by - mouse_pointer.y());
        if(s_x > s_y)
            need_steps = s_x / PX_SCALE;
        else
            need_steps = s_y / PX_SCALE;
        bool old_penalty = field->penalty_mode;
        for(int i = 0; i < need_steps; i++){
            qDebug() << "Click!" << need_steps << "\n";
            qint32 key_num = get_key_from_coord(mouse_pointer.x(), mouse_pointer.y());
            if(key_num == Qt::Key_5){
                qDebug() << "Ball position!\n";
                return;
            }
            if(field->penalty_mode != old_penalty)
                return;
            QKeyEvent key(QEvent::KeyPress, key_num, Qt::NoModifier);
            QApplication::sendEvent(this, &key);
            if (!key.isAccepted()) qDebug() << "something wrong\n";
        }
    }
}

qint32 MainWindow::get_key_from_coord(int x, int y){
    int top_offset = ui->menuBar->height();
    int bx = FIELD_WIDTH  / 2 - PX_SCALE * field->ball.x,
        by = FIELD_HEIGHT / 2 - PX_SCALE * field->ball.y + top_offset;

    // Don't move - this is a ball-point
    if(x == bx && y == by) return Qt::Key_5;

    if(x <  bx && y >  by) return Qt::Key_1;
    if(x == bx && y >  by) return Qt::Key_2;
    if(x >  bx && y >  by) return Qt::Key_3;
    if(x <  bx && y == by) return Qt::Key_4;
    if(x >  bx && y == by) return Qt::Key_6;
    if(x <  bx && y <  by) return Qt::Key_7;
    if(x == bx && y <  by) return Qt::Key_8;
    if(x >  bx && y <  by) return Qt::Key_9;

    // just in case
    return Qt::Key_5;
}

int MainWindow::get_key_from_coord(int x, int y, int bx, int by){
    // Don't move - this is a ball-point
    if(x == bx && y == by) return 5;

    if(x <  bx && y >  by) return 1;
    if(x == bx && y >  by) return 2;
    if(x >  bx && y >  by) return 3;
    if(x <  bx && y == by) return 4;
    if(x >  bx && y == by) return 6;
    if(x <  bx && y <  by) return 7;
    if(x == bx && y <  by) return 8;
    if(x >  bx && y <  by) return 9;

    // just in case
    return 5;
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
        tr("Text Files (*.txt);;C++ Files (*.cpp *.h)"));

    if (fileName != "") {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }
//        QTextStream in(&file);
//        textEdit->setText(in.readAll());

        file.close();
    }
}

void MainWindow::save()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "",
    tr("Text Files (*.txt);;C++ Files (*.cpp *.h)"));

    if (fileName != "") {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            // error message
        } else {
//            QTextStream stream(&file);
//            stream << textEdit->toPlainText();
//            stream.flush();
            file.close();
        }
    }
}

void MainWindow::new_field()
{
    delete field;
    field = new FootballField;
    this->repaint();
}


