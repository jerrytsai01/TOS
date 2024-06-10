#ifndef SETWINDOW_H
#define SETWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <vector>

class setwindow: public QMainWindow
{
    Q_OBJECT
public:
    setwindow(QWidget *parent = nullptr);
private:
    QGraphicsScene *scene;
    QGraphicsView *view;
};

#endif // SETWINDOW_H
