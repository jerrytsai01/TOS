#include "setwindow.h"

setwindow::setwindow(QWidget *parent) : QMainWindow(parent)
{
    setFixedSize(400,350);
    setWindowTitle("Set");
    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, 400,350);
    view = new QGraphicsView(scene);
    view->setFixedSize(400,350);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 關閉水平滾動條
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 關閉垂直滾動條
}
