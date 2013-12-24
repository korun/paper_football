#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>

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

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
