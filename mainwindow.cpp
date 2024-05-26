#include "mainwindow.h"
#include "stone.h"
#include <QGraphicsPixmapItem>
#include <QScrollBar>
#include <QTimer>
#include <QDebug>
#include <vector>
#include <QLabel>
#include <stdlib.h> /* 亂數相關函數 */
#include <time.h>   /* 時間相關函數 */

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)

{
    setFixedSize(540,840);
    setWindowTitle("Tower of Saviors");
    scene->setSceneRect(0, 0, 540,840);

    QPixmap runstone_bg(":/new/prefix1/dataset/runestone_bg.png");
    QGraphicsPixmapItem *runstone_bg_item = new QGraphicsPixmapItem(runstone_bg);
    runstone_bg_item->setPos(0,390);
    QSize originalSize = runstone_bg.size();
    qreal scaleX = 540.0 / originalSize.width();
    qreal scaleY = 450.0 / originalSize.height();
    runstone_bg_item->setScale(qMin(scaleX, scaleY));  // 按比例縮放
    scene->addItem(runstone_bg_item);

    QPixmap battle_bg(":/new/prefix1/dataset/battle_bg.png");
    QPixmap resize_battle = battle_bg.scaled(QSize(540, 390));
    QGraphicsPixmapItem *battle_bg_item = new QGraphicsPixmapItem(resize_battle);
    scene->addItem(battle_bg_item);

    view->setInteractive(true);
    view->setFixedSize(540,840);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->horizontalScrollBar()->setValue(0);
    setCentralWidget(view);

    for(int i=0;i<5;i++){
        vector<bool> temp;
        for(int j=0;j<6;j++){
            temp.push_back(false);
        }
        F.push_back(temp);
    }
    addStone();
}

void MainWindow::addStone()
{
    srand(time(NULL));
    int a = 90;
    for(int y=0;y<5;y++){
        for(int x=0;x<6;x++){
            if(F[y][x] == false){
                int type = rand()%6;
                stone *Stone = new stone(type, x*a, y*a+390);
                scene->addItem(Stone);
                F[y][x] = true;
            }
        }
    }
}

MainWindow::~MainWindow()
{

}

