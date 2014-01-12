#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    switch(e->key()){
        case Qt::Key_1:
            field.steps.push_back(1);
            break;
        case Qt::Key_2:
            field.steps.push_back(2);
            break;
        case Qt::Key_3:
            field.steps.push_back(3);
            break;
        case Qt::Key_4:
            field.steps.push_back(4);
            break;
        case Qt::Key_6:
            field.steps.push_back(6);
            break;
        case Qt::Key_7:
            field.steps.push_back(7);
            break;
        case Qt::Key_8:
            field.steps.push_back(8);
            break;
        case Qt::Key_9:
            field.steps.push_back(9);
    }
    this->repaint();
}

void MainWindow::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QPen light_blue_pen(QColor("#add8e6"), 1);
    QPen blue_pen(QColor("#00f"), 1);
    QPen red_pen(QColor("#f00"), 1);
    QPen std_pen(QColor("#000"), 1);

    int top_offset = ui->menuBar->height();

    // Рисуем сетку
    painter.begin(ui->FieldWidget);
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
}
