#include "endwindow.h"
#include "mainwindow.h"
#include <QPushButton>

endwindow::endwindow(QWidget *parent) : QMainWindow(parent)
{
    setFixedSize(400,350);
    setWindowTitle("end");
    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, 400,350);
    view = new QGraphicsView(scene);
    view->setFixedSize(400,350);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 關閉水平滾動條
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 關閉垂直滾動條

    result = new QLabel();

    restart = new QPushButton(view);
    restart->setIcon(QIcon(":/new/prefix1/dataset/restart.png"));
    restart->setFixedSize(266,45);
    restart->setIconSize(restart->size());
    restart->move(200-(265/2),250);
    connect(restart,&QPushButton::clicked,this,&QWidget::close);
    connect(restart,&QPushButton::clicked,this,&endwindow::openl);
    setCentralWidget(view);
}

void endwindow::gg(){
    if(MainWindow::gamephase >3){
        result->setText("YOU WIN");
    }
    else
        result->setText("YOU LOSE");

    result->setStyleSheet("QLabel { color : black; font-size: 40px; }");
    result->move(120,150);
    scene->addWidget(result);
}
