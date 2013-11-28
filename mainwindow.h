#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>

#define PX_SCALE     15
#define FIELD_WIDTH  600
#define FIELD_HEIGHT 780
#define PW_CENTER    (FIELD_WIDTH  / PX_SCALE / 2 + 1)
#define PH_CENTER    (FIELD_HEIGHT / PX_SCALE / 2 - 1)
#define FLD_POINT(X, Y) (points[PW_CENTER + (X)][PH_CENTER + (Y)])

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    const int GATE1[5][2] = {{2, -26}, {1, -26}, {0, -26}, {-1, -26}, {-2, -26}};
    const int GATE2[5][2] = {{2, 26}, {1, 26}, {0, 26}, {-1, 26}, {-2, 26}};
    QPoint points[FIELD_WIDTH / PX_SCALE][FIELD_HEIGHT / PX_SCALE]; /* [x, y] */

    void paintEvent(QPaintEvent *event);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
