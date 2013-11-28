#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    FLD_POINT(0, 0) = QPoint(0, 0);

    FLD_POINT(-3, -25) = QPoint(0, 0);
    FLD_POINT( 3, -25) = QPoint(0, 0);
    FLD_POINT(-3,  25) = QPoint(0, 0);
    FLD_POINT( 3,  25) = QPoint(0, 0);

    for(int y = -26; y <= 26; y++){
        FLD_POINT(-20, y) = QPoint(0, 0);
        FLD_POINT( 20, y) = QPoint(0, 0);
        if((y > -20 && y < -2) || (y > 2 && y < 20)){
            FLD_POINT(y,  26) = QPoint(0, 0);
            FLD_POINT(y, -26) = QPoint(0, 0);
        }
    }

    /*
    @@points[-3][-25]=Wall.new
    @@points[3][-25] =Wall.new
    @@points[-3][25] =Wall.new
    @@points[3][25] = Wall.new
    for y in -26..26
      @@points[-20][y],@@points[20][y] = Wall.new,Wall.new
      @@points[y][26],@@points[y][-26] = Wall.new,Wall.new if (y > -20 && y < -2) || (y > 2 && y < 20)
    end
    */
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

    int top_offset = ui->menuBar->height();

    painter.begin(ui->FieldWidget);
    painter.setPen(light_blue_pen);
    for(int i = 0; i < FIELD_WIDTH / PX_SCALE; i++){
        painter.drawLine(PX_SCALE * i, top_offset, PX_SCALE * i, top_offset + FIELD_HEIGHT);
    }
    for(int i = 0; i < FIELD_HEIGHT / PX_SCALE; i++){
        painter.drawLine(0, top_offset + PX_SCALE * i, FIELD_WIDTH, top_offset + PX_SCALE * i);
    }

    painter.setPen(blue_pen);
    painter.drawLine(FIELD_WIDTH / 2 - PX_SCALE * 3, top_offset + FIELD_HEIGHT, FIELD_WIDTH / 2 - PX_SCALE * 3, top_offset + FIELD_HEIGHT - PX_SCALE);
    painter.drawLine(FIELD_WIDTH / 2 + PX_SCALE * 3, top_offset + FIELD_HEIGHT, FIELD_WIDTH / 2 + PX_SCALE * 3, top_offset + FIELD_HEIGHT - PX_SCALE);

    painter.setPen(red_pen);
    painter.drawLine(FIELD_WIDTH / 2 - PX_SCALE * 3, top_offset, FIELD_WIDTH / 2 - PX_SCALE * 3, top_offset + PX_SCALE);
    painter.drawLine(FIELD_WIDTH / 2 + PX_SCALE * 3, top_offset, FIELD_WIDTH / 2 + PX_SCALE * 3, top_offset + PX_SCALE);

    painter.setPen(std_pen);
    painter.drawEllipse(QPointF(FIELD_WIDTH / 2, top_offset + 1), 1, 1);
    painter.drawEllipse(QPointF(FIELD_WIDTH / 2, top_offset + FIELD_HEIGHT - 1), 1, 1);
    painter.drawEllipse(QPointF(FIELD_WIDTH / 2, top_offset + FIELD_HEIGHT / 2), 1, 1);
}
