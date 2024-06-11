#include "setwindow.h"
#include <QPushButton>

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

    sur=new QPushButton(view);
    sur->setIcon(QIcon(":/new/prefix1/dataset/surrender.png"));
    sur->setFixedSize(265,45);
    sur->setIconSize(sur->size());
    sur->move(200-(265/2),100);

    goback =  new QPushButton(view);
    goback->setIcon(QIcon(":/new/prefix1/dataset/go back.png"));
    goback->setFixedSize(266,45);
    goback->setIconSize(goback->size());
    goback->move(200-(266/2),250);
    connect(sur,&QPushButton::clicked,this,&setwindow::surrender);
    connect(sur,&QPushButton::clicked,this,&QWidget::close);
    connect(goback,&QPushButton::clicked,this,&QWidget::close);
    setCentralWidget(view);
}

