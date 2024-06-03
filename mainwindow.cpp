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
    fall();
    // initialize timerRect
    timeStripBack = new QGraphicsRectItem(0, 500, 530, 20);
    timeStrip = new QGraphicsRectItem(0, 500, 530, 20);
    timeStrip->setBrush(Qt::green);
    timeStripBack->setBrush(Qt::black);
    scene->addItem(timeStripBack);
    scene->addItem(timeStrip);
}

void MainWindow::addStone()
{
    srand(time(NULL));


    for(int y=0;y<5;y++){
        vector<stone*> temps;
        for(int x=0;x<6;x++){
            if(F[y][x] == false){
                int type = rand()%6+1;
                stone *Stone = new stone(type, x*a, y*a+510);
                connect(Stone, &stone::stoneMoved, this, &MainWindow::handleStoneMove);
                connect(Stone, &stone::updateTimer, this, &MainWindow::updateTimerStrip);
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
void MainWindow::updateTimerStrip(int remainingTime) {
    // 计算剩余时间的比例
    float remainingTimeFraction = static_cast<float>(remainingTime) / 1000.0f;
    qDebug() << remainingTimeFraction <<" " <<remainingTime;
    // 更新计时器矩形的宽度
    timeStrip->setRect(0, 500, 540 * remainingTimeFraction, 20);
}
void MainWindow::handleStoneMove(QPointF newGridPos, QPointF oldGridPos) {
    int oldY = floor((oldGridPos.y()-510)/90);
    int oldX = floor(oldGridPos.x()/90);
    int newY = floor((newGridPos.y()-510)/90);
    int newX = floor(newGridPos.x()/90);
    qDebug() << "Stone Grid positions moved from " << oldGridPos << " to " << newGridPos;
    qDebug() << "Stone positions moved from (" << oldX << "," << oldY << ") to (" << newX << "," << newY << ")";
    //std::swap(F[oldY][oldX], F[newY][newX]);
    //savestone[oldY][oldX]->setPos(newGridPos);
    savestone[newY][newX]->setPos(oldGridPos);
    std::swap(savestone[oldY][oldX], savestone[newY][newX]);
    std::swap(stonepos[oldY][oldX], stonepos[newY][newX]);
    qDebug() << "Swapped grid positions in array";
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
                erasestonenum[counterasestone[y][x]-1]++;
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
                    stonepos[y][x]=0;
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

void MainWindow::fall(){
    for(int y=4;y>0;y--){
        for(int x=0;x<6;x++){
            if(savestone[y][x]==nullptr and savestone[y-1][x]!=nullptr and y>0){
                swap(savestone[y][x],savestone[y-1][x]);
                swap(stonepos[y][x],stonepos[y-1][x]);
                QTimer *timer = new QTimer(this);
                int count = 0;
                connect(timer, &QTimer::timeout, this, [=]() mutable {
                    count++;
                    savestone[y][x]->setPos( x*a, y*a+510+count*10);
                    qDebug()<<"fall"<<count;
                    if(count==5)
                        timer->stop();
                });
                timer->start(10);

            }
        }
    }
}
