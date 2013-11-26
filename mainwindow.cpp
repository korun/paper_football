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

void MainWindow::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QPen light_blue_pen(QColor("#add8e6"), 1);
    QPen blue_pen(QColor("#00f"), 1);
    QPen red_pen(QColor("#f00"), 1);
    QPen std_pen(QColor("#000"), 1);

    painter.begin(ui->FieldWidget);
    painter.setPen(light_blue_pen);
    for(int i = 0; i < 600 / PX_SCALE; i++){
        painter.drawLine(PX_SCALE * i, 21 + 0, PX_SCALE * i, 21 + 780);
    }
    for(int i = 0; i < 780 / PX_SCALE; i++){
        painter.drawLine(0, 21 + PX_SCALE * i, 600, 21 + PX_SCALE * i);
    }

    painter.setPen(blue_pen);
    painter.drawLine(600 / 2 - PX_SCALE * 3, 21 + 780, 600 / 2 - PX_SCALE * 3, 21 + 780 - PX_SCALE);
    painter.drawLine(600 / 2 + PX_SCALE * 3, 21 + 780, 600 / 2 + PX_SCALE * 3, 21 + 780 - PX_SCALE);

    painter.setPen(red_pen);
    painter.drawLine(600 / 2 - PX_SCALE * 3, 21 + 0, 600 / 2 - PX_SCALE * 3, 21 + 0 + PX_SCALE);
    painter.drawLine(600 / 2 + PX_SCALE * 3, 21 + 0, 600 / 2 + PX_SCALE * 3, 21 + 0 + PX_SCALE);

    painter.setPen(std_pen);
    painter.drawEllipse(QPointF(600 / 2, 21 + 1), 1, 1);
    painter.drawEllipse(QPointF(600 / 2, 21 + 780 - 1), 1, 1);
    painter.drawEllipse(QPointF(600 / 2, 21 + 780 / 2), 1, 1);
/*
    TkcOval.new(canvas, WIDTH/2 + 1, 1, WIDTH/2 - 1, 3) {fill("black")}

    QPointF a[temp_max];
    sx = this -> width();
    sy = this -> height();
    gcx = sx / max;
    gcy = sy / temp_max;
    gc = max - min;
    for (i=0; i<=temp_max; i++){
        x = (temp[i] - gc) * gcx;
        y = i * gcy;
        a[i] = QPointF (x, y);
    }
    painter.drawPolyline (a, temp_max);
*/
}
