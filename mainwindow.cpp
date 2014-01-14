#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *e){
    bool success = false;
    signed char pl = field.current_player;
    switch(e->key()){
        case Qt::Key_Z:
        case Qt::Key_1:
            success = field.try_step(1);
            break;
        case Qt::Key_X:
        case Qt::Key_2:
            success = field.try_step(2);
            break;
        case Qt::Key_C:
        case Qt::Key_3:
            success = field.try_step(3);
            break;
        case Qt::Key_A:
        case Qt::Key_4:
            success = field.try_step(4);
            break;
        case Qt::Key_D:
        case Qt::Key_6:
            success = field.try_step(6);
            break;
        case Qt::Key_Q:
        case Qt::Key_7:
            success = field.try_step(7);
            break;
        case Qt::Key_W:
        case Qt::Key_8:
            success = field.try_step(8);
            break;
        case Qt::Key_E:
        case Qt::Key_9:
            success = field.try_step(9);
    }
    if (success){
        this->repaint();
        if(pl != field.current_player){
            if(field.current_player < 0)
                QMessageBox::information(this, tr("Information"), tr("Red player kicks"));
            else
                QMessageBox::information(this, tr("Information"), tr("blue player kicks"));
        }
        if(field.penalty_mode){
            QMessageBox::information(this, tr("Information"), tr("PENALTY!"));
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
    std::vector<signed char>::iterator iterator = field.steps.begin();
    while (iterator != field.steps.end()) {
        int index = abs((int) *iterator);
        int x = old_x - PX_SCALE * field.KEYS[index][0];
        int y = old_y - PX_SCALE * field.KEYS[index][1];
        painter.setPen(*iterator < 0 ? red_pen : blue_pen);
        painter.drawLine(old_x, top_offset + old_y, x, top_offset + y);
        old_x = x;
        old_y = y;
        ++iterator;
    }

    if(show_pointer){
        painter.setPen(tip_pen);
        // Without top_offset!
        painter.drawEllipse(QPointF(mouse_pointer.x(), mouse_pointer.y()), 2, 2);
    }

    // Ball marker
    if(field.show_ball){
        painter.setPen(std_pen);
        painter.drawEllipse(QPointF(old_x, top_offset + old_y), 2, 2);
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    int top_offset = ui->menuBar->height();
    int mx = event->pos().x(),
        my = event->pos().y(),
        bx = FIELD_WIDTH  / 2 - PX_SCALE * field.ball.x,
        by = FIELD_HEIGHT / 2 - PX_SCALE * field.ball.y + top_offset;
    int local_mx = mx % PX_SCALE,
        local_my = (my - top_offset) % PX_SCALE;
    if((mx <= bx + PX_SCALE * 3 + PX_SCALE / 3 && mx >= bx - PX_SCALE * 3 - PX_SCALE / 3) && // mouse x in -3..3
       (my <= by + PX_SCALE * 3 + PX_SCALE / 3 && my >= by - PX_SCALE * 3 - PX_SCALE / 3)    // mouse y in -3..3
    ){
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

            mouse_pointer.setX(mx);
            mouse_pointer.setY(my);
            //qDebug() << "--------------!" << QString::number(mx) << "" << QString::number(my) << "\n";
            ui->FieldWidget->setCursor(Qt::BlankCursor);
            this->repaint();
        }
        else{
            ui->FieldWidget->setCursor(Qt::CrossCursor);
        }
        return;
    }
    if(show_pointer){
        show_pointer = false;
        ui->FieldWidget->setCursor(Qt::ArrowCursor);
        this->repaint();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    if(show_pointer && event->button() == Qt::LeftButton){
        qint32 key_num = get_key_from_coord(mouse_pointer.x(), mouse_pointer.y());
        if(key_num == Qt::Key_5){
            qDebug() << "Ball position!\n";
            return;
        }
        QKeyEvent key(QEvent::KeyPress, key_num, Qt::NoModifier);
        QApplication::sendEvent(this, &key);

        if (!key.isAccepted()) {
            qDebug() << "something wrong\n";
        }
    }
}

qint32 MainWindow::get_key_from_coord(int x, int y){
    int top_offset = ui->menuBar->height();
    int bx = FIELD_WIDTH  / 2 - PX_SCALE * field.ball.x,
        by = FIELD_HEIGHT / 2 - PX_SCALE * field.ball.y + top_offset;

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
