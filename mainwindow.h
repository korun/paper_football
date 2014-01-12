#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QKeyEvent>

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

private slots:
    void keyPressEvent(QKeyEvent *e);
};

#endif // MAINWINDOW_H
