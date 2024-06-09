#include "mainwindow.h"
#include "stone.h"
#include "math.h"
#include "hpbar.h"
#include "cdbar.h"
#include "slime.h"
#include "babyhoneymon.h"
#include "cerberus.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QScrollBar>
#include <QTimer>
#include <QMouseEvent>
#include <QDebug>
#include <vector>
#include <QLabel>
#include <QString>
#include <stdlib.h> /* 亂數相關函數 */
#include <time.h>   /* 時間相關函數 */

using namespace std;

bool MainWindow::moveTime = true;
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

    connect(this,&MainWindow::tofall,this,&MainWindow::fall);
    addEnemy();
    //erasestone();
    //fall();
}

void MainWindow::addStone()
{
    srand(time(NULL));
    for(int y=0;y<5;y++){
        vector<stone*> temps;
        for(int x=0;x<6;x++){
            if(F[y][x] == false){
                int type = rand()%6+1;
                stone *Stone = new stone(type, x*a, y*a+510, scene);
                connect(Stone, &stone::stoneMoved, this, &MainWindow::handleStoneMove);
                connect(Stone, &stone::CDover, this, &MainWindow::CDoverEvent);
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

void MainWindow::CDoverEvent()
{
    combo=0;
    erasestone();
    //fall();


}
void MainWindow::handleStoneMove(QPointF newGridPos, QPointF oldGridPos) {
    int oldY = floor((oldGridPos.y()-510)/90);
    int oldX = floor(oldGridPos.x()/90);
    int newY = floor((newGridPos.y()-510)/90);
    int newX = floor(newGridPos.x()/90);
    //qDebug() << "Stone Grid positions moved from " << oldGridPos << " to " << newGridPos;
    //qDebug() << "Stone positions moved from (" << oldX << "," << oldY << ") to (" << newX << "," << newY << ")";
    //std::swap(F[oldY][oldX], F[newY][newX]);
    //savestone[oldY][oldX]->setPos(newGridPos);
    savestone[newY][newX]->setPos(oldGridPos);
    std::swap(savestone[oldY][oldX], savestone[newY][newX]);
    std::swap(stonepos[oldY][oldX], stonepos[newY][newX]);
    //qDebug() << "Swapped grid positions in array";
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
    moveTime = false;
    //reset
    iffall=0;
    for(int y=0;y<5;y++){
        for(int x=0;x<6;x++){
            combine[y][x]=0;
            iffinded[y][x]=0;
            counterasestone[y][x]=0;
            waitdelete[y][x]=0;
        }
    }
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
                iffall=1;
                combo++;
                combotext="Combo: "+ QString::number(combo);

                find(x,y,combo,stonepos[y][x]);

            }
        }
    }
    //erase animation

    if(iffall){
        QTimer *timer = new QTimer(this);
        int count = 0;
        connect(timer, &QTimer::timeout, this, [=]() mutable {
            count++;
            for(int y=0;y<5;y++){
                for(int x=0;x<6;x++){
                    if(waitdelete[y][x]==count){
                        delete savestone[y][x];
                        savestone[y][x] = nullptr;
                        stonepos[y][x]=0;
                        qDebug()<<"delete x:"<<x<<" y:"<<y;
                        //qDebug()<<combo;
                    }
                }
            }
            //qDebug()<<"combo"<<count;
            //erase end
            if(count>=combo){
                timer->stop();
                qDebug()<<"tofall";
                //checkmatrix();
                moveTime = true;
                emit tofall();
            }
        });
        timer->start(500);
    }

}

void MainWindow::fall(){
    qDebug()<<"falling";
    for(int x=0;x<6;x++){
        for(int y=4;y>0;y--){
            if(stonepos[y][x]==0){
                for(int ty=y-1;ty>=0;ty--){
                    if(stonepos[ty][x]!=0){
                        swap(savestone[ty][x],savestone[y][x]);
                        swap(stonepos[ty][x],stonepos[y][x]);
                        /*qDebug()<<"stonepos: ";
                        for(int y=0;y<5;y++){
                            QString a;
                            for(int x=0;x<6;x++){
                                a.append(stonepos[y][x]);
                            }
                            qDebug()<<a;
                        }*/
                        //qDebug()<<"tofallanimation";
                        fallanimation(y,x,ty);
                        break;
                    }
                }
            }
        }
    }

    qDebug()<<"filling";
    for(int x=0;x<6;x++){
        for(int y=0;y<5;y++){
            if(stonepos[y][x]==0){
                int type = rand()%6+1;
                stone *Stone=new stone(type, x*a, y*a+510, scene);
                if(y!=0){
                    //fill fall animation
                    Stone->setPos(x*a,510);
                    fallanimation(y,x,0);
                }
                connect(Stone, &stone::stoneMoved, this, &MainWindow::handleStoneMove);
                connect(Stone, &stone::CDover, this, &MainWindow::CDoverEvent);
                savestone[y][x]=Stone;
                stonepos[y][x]=type;
                scene->addItem(Stone);
            }
        }
    }

    erasestone();
}

void MainWindow::addEnemy(){
    if(gamephase == 1){
        vector<tuple<int, int, int>> slimeInitValues = {
            {1, 100, 300},
            {3, 270, 300},
            {2, 440, 300}
        };
        for (const auto& [attr, x, y] : slimeInitValues) {
            slime* Slime = new slime(attr, x, y);
            scene->addItem(Slime);
        }
    }
    else if(gamephase == 2){
        babyhoneymon *Babyhoneymon = new babyhoneymon(150, 200);
        scene->addItem(Babyhoneymon);
    }
    else if(gamephase == 3){
        cerberus *Cerberus = new cerberus(200, 200);
        scene->addItem(Cerberus);
    }
}

void MainWindow::fallanimation(int goal,int x,int now){

    QTimer *timerr = new QTimer(this);
    int count = 0;
    connect(timerr, &QTimer::timeout, this, [=]() mutable {
        count++;
        savestone[goal][x]->setPos( x*a, now*a+510+count*10);
        //qDebug()<<"fall"<<count;
        if(count==(goal-now)*9)
            timerr->stop();
    });
    timerr->start(10);
    //savestone[goal][x]->setPos( x*a, goal*a+510);
}


void MainWindow::checkmatrix(){

    qDebug()<<"stonepos: ";
    for(int y=0;y<5;y++){
        QString a;
        for(int x=0;x<6;x++){
            a.append(stonepos[y][x]);
        }
        qDebug()<<a;
    }
    qDebug()<<"iffinded :";
    for(int y=0;y<5;y++){
        QString a;
        for(int x=0;x<6;x++){
            a.append(iffinded[y][x]);
        }
        qDebug()<<a;
    }
    qDebug()<<"counterasestone :";
    for(int y=0;y<5;y++){
        QString a;
        for(int x=0;x<6;x++){
            a.append(counterasestone[y][x]);
        }
        qDebug()<<a;
    }
    qDebug()<<"waitdelete :";
    for(int y=0;y<5;y++){
        QString a;
        for(int x=0;x<6;x++){
            a.append(waitdelete[y][x]);
        }
        qDebug()<<a;
    }
}
