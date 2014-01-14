#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QKeyEvent>
#include <QMessageBox>

#include "football_field.h"

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
    FootballField field;
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::MainWindow *ui;
    bool show_pointer = false;
    QPoint mouse_pointer;
    std::vector<QPoint> mouse_points[6];

private slots:
    void keyPressEvent(QKeyEvent *e);
};

#endif // MAINWINDOW_H
