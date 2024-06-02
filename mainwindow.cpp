#include "mainwindow.h"
#include "stone.h"
#include "math.h"
#include <QGraphicsPixmapItem>
#include <QScrollBar>
#include <QTimer>
#include <QDebug>
#include <vector>
#include <QLabel>
#include <QString>
#include <stdlib.h> /* 亂數相關函數 */
#include <time.h>   /* 時間相關函數 */

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)

{
    setFixedSize(540,960);
    setWindowTitle("Tower of Saviors");
    scene->setSceneRect(0, 0, 540,960);

    QPixmap runstone_bg(":/new/prefix1/dataset/runestone_bg.png");
    QGraphicsPixmapItem *runstone_bg_item = new QGraphicsPixmapItem(runstone_bg);
    runstone_bg_item->setPos(0,510);
    QSize originalSize = runstone_bg.size();
    qreal scaleX = 540.0 / originalSize.width();
    qreal scaleY = 450.0 / originalSize.height();
    runstone_bg_item->setScale(qMin(scaleX, scaleY));  // 按比例縮放
    scene->addItem(runstone_bg_item);

    QPixmap battle_bg(":/new/prefix1/dataset/battle_bg.png");
    QPixmap resize_battle = battle_bg.scaled(QSize(540, 510));
    QGraphicsPixmapItem *battle_bg_item = new QGraphicsPixmapItem(resize_battle);
    scene->addItem(battle_bg_item);

    view->setInteractive(true);
    view->setFixedSize(540,960);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->horizontalScrollBar()->setValue(0);
    setCentralWidget(view);

    //initial
    combo=0;

    for(int i=0;i<5;i++){
        vector<bool> tempf;
        vector<int> tempi;

        for(int j=0;j<6;j++){
            tempf.push_back(false);
            tempi.push_back(0);
        }
        F.push_back(tempf);
        stonepos.push_back(tempi);
        combine.push_back(tempi);
        counterasestone.push_back(tempi);
        waitdelete.push_back(tempi);
        iffinded.push_back(tempi);
    }
    addStone();
    showcombo = new QGraphicsTextItem();
    showcombo->setPos(200,500);
    scene->addItem(showcombo);

    //each round
    erasestone();
}

void MainWindow::addStone()
{
    srand(time(NULL));
    int a = 90;

    for(int y=0;y<5;y++){
        vector<stone*> temps;
        for(int x=0;x<6;x++){
            if(F[y][x] == false){
                int type = rand()%6+1;
                stone *Stone = new stone(type, x*a, y*a+510);
                connect(Stone, &stone::stoneMoved, this, &MainWindow::handleStoneMove);
                temps.push_back(Stone);
                //savestone[y][x]=Stone;
                stonepos[y][x]=type;
                scene->addItem(Stone);
                F[y][x] = true;
            }
        }
        savestone.push_back(temps);

    }
}

void MainWindow::handleStoneMove(QPointF newGridPos, QPointF oldGridPos) {
    qDebug() << "Stone Grid positions moved from " << oldGridPos << " to " << newGridPos;
    // Convert grid positions to pixel positions for QRectF
    QRectF newRect(newGridPos.x(), newGridPos.y(), 90, 90);
    QList<QGraphicsItem *> newItems = scene->items(newRect);

    // Check if stone items exist at the new position
    if (!newItems.isEmpty()) {
        qDebug() << "Number of items at new position:" << newItems.size();
        for (int i = 0; i < newItems.size(); ++i) {
            qDebug() << "Checking item type at index" << i << ":" << newItems[i]->type();
            // Check if the item is of type stone by using typeid
            if (typeid(*newItems[i]) == typeid(stone)) {
                qDebug() << "Found stone at new position";
                // Cast the item to a stone object
                stone *newStone = static_cast<stone *>(newItems[i]);
                if (newStone) {
                    qDebug() << "Stone cast successful. Moving new stone.";
                    // Move the new stone to the old position
                    newStone->setPos(oldGridPos.x(), oldGridPos.y());
                    qDebug() << "New stone moved to old position.";

                    int oldY = floor((oldGridPos.y()-510)/90);
                    int oldX = floor(oldGridPos.x()/90);
                    int newY = floor((newGridPos.y()-510)/90);
                    int newX = floor(newGridPos.x()/90);
                    std::swap(F[oldY][oldX], F[newY][newX]);
                    std::swap(savestone[oldY][oldX], savestone[newY][newX]);
                    std::swap(stonepos[oldY][oldX], stonepos[newY][newX]);
                    qDebug() << "Swapped grid positions in array";

                    return; // Once we've handled the first stone, we can exit the loop
                } else {
                    qDebug() << "Failed to cast to stone.";
                }
            }
        }
    } else {
        qDebug() << "No items found at the new position.";
    }
}

void MainWindow::find(int inx,int iny,int del,int type){
    if(iffinded[iny][inx] && stonepos[iny][inx]==type){
        iffinded[iny][inx]=0;
        waitdelete[iny][inx]=del;
        if(iny>0){  //up
            find(inx,iny-1,del,type);
        }
        if(iny<4){  //down
            find(inx,iny+1,del,type);
        }
        if(inx>0){  //left
            find(inx-1,iny,del,type);
        }
        if(inx<5){  //right
            find(inx+1,iny,del,type);
        }
    }
    else{
        return ;
    }
}

void MainWindow::erasestone(){
    qDebug() << "erase";
    //x line
    for(int y=0;y<5;y++){
        for(int x=0;x<4;x++){
            if((stonepos[y][x] == stonepos[y][x+1]) && (stonepos[y][x+1]==stonepos[y][x+2]) ){
                combine[y][x]=stonepos[y][x];
                iffinded[y][x]=1;
                iffinded[y][x+1]=1;
                iffinded[y][x+2]=1;
                counterasestone[y][x]=stonepos[y][x];
                counterasestone[y][x+1]=stonepos[y][x];
                counterasestone[y][x+2]=stonepos[y][x];
            }
        }
    }
    //y line
    for(int y=0;y<3;y++){
        for(int x=0;x<6;x++){
            if((stonepos[y][x]== stonepos[y+1][x]) && (stonepos[y+1][x]==stonepos[y+2][x])){
                combine[y][x]=stonepos[y][x];
                iffinded[y][x]=1;
                iffinded[y+1][x]=1;
                iffinded[y+2][x]=1;
                counterasestone[y][x]=stonepos[y][x];
                counterasestone[y+1][x]=stonepos[y][x];
                counterasestone[y+2][x]=stonepos[y][x];
            }
        }
    }
    //count
    for(int y=0;y<5;y++){
        for(int x=0;x<6;x++){
            if(counterasestone[y][x]!=0){
                erasestonenum[counterasestone[y][x]]++;
            }
            if(iffinded[y][x]){
                combo++;
                combotext="Combo: "+ QString::number(combo);

                find(x,y,combo,stonepos[y][x]);

            }
        }
    }
    //erase animation
    QTimer *timer = new QTimer(this);
    int count = 0;

    connect(timer, &QTimer::timeout, this, [=]() mutable {
        count++;
        for(int y=0;y<5;y++){
            for(int x=0;x<6;x++){
                if(waitdelete[y][x]==count){
                    delete savestone[y][x];
                     qDebug()<<"delete x:"<<x<<" y:"<<y;
                    qDebug()<<combo;
                }
            }
        }
        qDebug()<<"count"<<count;
        if(count>=combo)
            timer->stop();
    });
    timer->start(500);

}


