#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QKeyEvent>
#include <QMessageBox>
#include <QFileDialog>

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
    FootballField *field;
    void paintEvent(QPaintEvent *event);

private:
    Ui::MainWindow *ui;

public slots:
    void keyPressEvent(QKeyEvent *e);
    void new_field();
    void open();
    void save();
};

#endif // MAINWINDOW_H
