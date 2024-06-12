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

public slots:
    void lose();

signals:
    void reset();
    void surrender();

private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    QPushButton *sur;
    QPushButton *goback;
    QPushButton *restart;

};

#endif // SETWINDOW_H
