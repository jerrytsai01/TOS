#include "mainwindow.h"
#include "stone.h"
#include "math.h"
#include "hpbar.h"
#include "cdbar.h"
#include "slime.h"
#include "babyhoneymon.h"
#include "cerberus.h"
#include "characters.h"
#include "endwindow.h"
#include "setwindow.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QScrollBar>
#include <QTimer>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QApplication>
#include <vector>
#include <QLabel>
#include <QString>
#include <QGraphicsDropShadowEffect>
#include <QFont>
#include <stdlib.h> /* 亂數相關函數 */
#include <time.h>   /* 時間相關函數 */

using namespace std;

bool MainWindow::moveTime = true;
int MainWindow::gamephase =1;
int MainWindow::roundCount =0;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), sATK(0), cATK(0), bATK(0)

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
    showcombo->setPos(300,800);
    showcombo->setPlainText(combotext);      // 设置初始文本
    showcombo->setFont(QFont("Arial", 24));     // 设置字体和大小
    showcombo->setDefaultTextColor(Qt::yellow);     // 设置文本颜色
    showcombo->setZValue(10);

    for(size_t i=0;i<6;i++){
        showdamage[i] = new QGraphicsTextItem();
        showdamage[i]->setPos(i*90+10, 410);
        showdamage[i]->setPlainText(showdamagetxt[i]);
        showdamage[i]->setFont(QFont("Arial", 25));
        showdamage[i]->setDefaultTextColor(Qt::black);
        showdamage[i]->setZValue(15);
        scene->addItem(showdamage[i]);
    }
    showdamage[6] = new QGraphicsTextItem();
    showdamage[6]->setPos(250,475);
    showdamage[6]->setPlainText(showdamagetxt[6]);
    showdamage[6]->setFont(QFont("Arial", 20));
    showdamage[6]->setDefaultTextColor(Qt::black);
    showdamage[6]->setZValue(15);
    scene->addItem(showdamage[6]);
    for(int i=0;i<7;i++){
        showdamage[i+7] = new QGraphicsTextItem();
        showdamage[i+7]->setPos((i%3)*120+80, 210);
        showdamage[i+7]->setPlainText(showdamagetxt[i+7]);
        showdamage[i+7]->setFont(QFont("Arial", 18));
        showdamage[i+7]->setDefaultTextColor(Qt::red);
        showdamage[i+7]->setZValue(20);
        scene->addItem(showdamage[i+7]);
    }

    // 创建阴影效果作为边框
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect();
    shadowEffect->setOffset(0);  // 使阴影紧贴文本，产生边框效果
    shadowEffect->setBlurRadius(5);  // 控制边框的模糊半径
    shadowEffect->setColor(Qt::black);  // 边框颜色为黑色
    showcombo->setGraphicsEffect(shadowEffect);
    scene->addItem(showcombo);

    connect(this,&MainWindow::tofall,this,&MainWindow::fall);
    connect(this,&MainWindow::ATKsign, this, &MainWindow::ATKevent);
    addEnemy();
    HPbar *hpBar = new HPbar();
    QPixmap hpbtn= QPixmap(":/new/prefix1/dataset/hp_icon.png");;
    QGraphicsPixmapItem *hpIcon = new QGraphicsPixmapItem(hpbtn);
    hpIcon->setPos(0,465);
    scene->addItem(hpIcon);
    connect( this, &MainWindow::updateHP,hpBar, &HPbar::updateHPBar);
    scene->addItem(hpBar);
    //erasestone();
    //fall();

    setBtn = new QPushButton(this);
    setBtn->setIcon(QIcon(":/new/prefix1/dataset/S__9814023.jpg"));
    setBtn->setFixedSize(89, 60);
    setBtn->setIconSize(setBtn->size());
    setBtn->move(540-89,0);
    connect(setBtn, &QPushButton::clicked, this, &MainWindow::setshow);
}

void MainWindow::onCharactersSelected(const std::vector<int> &charactersIn)
{
    // 处理接收到的角色数据
    for(int character : charactersIn){
        //qDebug() << "Character selected:" << character;
        characters.push_back(character);
    }
    addCharacters(characters);
}

void MainWindow::addCharacters(vector<int> &cha)
{
    for(size_t i=0;i <cha.size();i++){
        //qDebug() << cha[i];
        Characters *character = new Characters(cha[i]);
        character->setPos(i*90, 390);
        charObj.push_back(character);
        scene->addItem(character);
    }
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
                connect(this, &MainWindow::forceRel, Stone, &stone::forceRelease);
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
    moveTime = false;

    //qDebug() <<"EnemyOBjs :";
    for(auto it: enemyObj){
        if(it->b != nullptr){
            //qDebug() << "baby";
        }
        else if(it->c!=nullptr){
            //qDebug() <<"cerb";
        }
        else if(it->s != nullptr){
            //qDebug() << "Slime";
        }
        else {
            //qDebug() <<"null";
        }
    }
    //qDebug() << enemy;
    combo=0;
    for(int i=1;i<=6;i++){
        erasestonenum[i]=0;
    }
    erasestone();
    //qDebug() << "combo" << combo;
    //qDebug() << "erase" << erasestonenum[1] << "fire stones";
    //qDebug() << "erase" << erasestonenum[2] << "water stones";
    //qDebug() << "erase" << erasestonenum[3] << "earth stones";
    //qDebug() << "erase" << erasestonenum[4] << "light stones";
    //qDebug() << "erase" << erasestonenum[5] << "dark stones";
    //qDebug() << "erase" << erasestonenum[6] << "heart stones";

    moveTime = true;
}

void MainWindow::ATKevent()
{
    calculated = false;
    calATKandHEAL();

    ATKform = true;
    calHP();
    CharsATK();
    ATKform = false;
    DEFform = true;
    calHP();
    if(std::find(enemy.begin(),enemy.end(), 2) != enemy.end()){
        //weatherStone();
    }
    if(enemy.empty()){
        if(gamephase<3){
            gamephase++;
            addEnemy();
        }
        else{
            emit over();
        }
    }
}

void MainWindow::CharsATK(){
    for(int i=7;i<14;i++){
        showdamagetxt[i]="";
    }
    qDebug()<<"ATK";
    if((gamephase == 1)&&(!resetENEhp)){
        for(int i = 0; i < 3; i++){
            EnemyHP.push_back(100);
            //EnemyHP[i] = 100;
        }
        resetENEhp = true;
    }
    else if((gamephase == 2)&&(!resetENEhp)){
        EnemyHP.push_back(100);
        EnemyHP.push_back(300);
        EnemyHP.push_back(100);
        //EnemyHP[0] = 100;
        //EnemyHP[1] = 300;
        //EnemyHP[2] = 100;
        resetENEhp = true;
    }
    else if((gamephase == 3)&&(!resetENEhp)){
        EnemyHP.push_back(700);
        //EnemyHP[0] = 700;
        resetENEhp = true;
    }
    for(size_t i = 0; i < characters.size(); i++){
        for(int j = 1; j <= 5; j++){
            if(characters[i] == j){
                if(j == 1){
                    ATKofChars[i] = allpoints[2];
                }
                else if(j == 2){
                    ATKofChars[i] = allpoints[1];
                }
                else {
                    ATKofChars[i] = allpoints[j];
                }
                //ATKofChars[i] = allpoints[j];
                qDebug() << "character" << i << "has" << ATKofChars[i] << "ATK";
                break;
            }
        }
    }
    for(size_t i = 0; i < characters.size(); i++){
        int atk = 0;
        bool finishedATK = false;
        if(!enemyObj.empty()){
            for(size_t j = 0; j < enemyObj.size(); j++){
                if(EnemyHP.empty())
                    continue;
                else {
                    finishedATK = true;
                    if(gamephase == 1){
                        if(j == 0 && enemyObj[j]->s!=nullptr){
                            switch(characters[i]){
                            case(2) :
                                atk = ATKofChars[i] * 0.5;
                                break;
                            case(3) :
                                atk = ATKofChars[i] * 2;
                                break;
                            default :
                                atk = ATKofChars[i];
                                break;
                            }
                        }
                        else if(j == 1 && enemyObj[j]->s!=nullptr){
                            switch(characters[i]){
                            case(1) :
                                atk = ATKofChars[i] * 2;
                                break;
                            case(3) :
                                atk = ATKofChars[i] * 0.5;
                                break;
                            default :
                                atk = ATKofChars[i];
                                break;
                            }
                        }
                        else {
                            switch(characters[i]){
                            case(1) :
                                atk = ATKofChars[i] * 0.5;
                                break;
                            case(2) :
                                atk = ATKofChars[i] * 2;
                                break;
                            default :
                                atk = ATKofChars[i];
                                break;
                            }
                        }
                    }
                    else if(gamephase == 2){
                        if(j == 0 && enemyObj[j]->s!=nullptr){
                            switch(characters[i]){
                            case(5) :
                                atk = ATKofChars[i] * 2;
                                break;
                            default :
                                atk = ATKofChars[i];
                                break;
                            }
                        }
                        else if(j == 1 && enemyObj[j]->b!=nullptr){
                            switch(characters[i]){
                            case(1) :
                                atk = ATKofChars[i] * 0.5;
                                break;
                            case(2) :
                                atk = ATKofChars[i] * 2;
                                break;
                            default :
                                atk = ATKofChars[i];
                                break;
                            }
                        }
                        else {
                            switch(characters[i]){
                            case(4) :
                                atk = ATKofChars[i] * 2;
                                break;
                            default :
                                atk = ATKofChars[i];
                                break;
                            }
                        }
                    }
                    else {
                        switch(characters[i]){
                        case(1) :
                            atk = ATKofChars[i] * 2;
                            break;
                        case(3) :
                            atk = ATKofChars[i] * 0.5;
                            break;
                        default :
                            atk = ATKofChars[i];
                            break;
                        }
                    }


                    qDebug() << "character" << i << "attack" << atk << "to enemy" << j;
                    if(EnemyHP[j] <= atk){
                        EnemyHP[j] = 0;
                    }
                    else {
                        EnemyHP[j] = EnemyHP[j] - atk;
                    }
                    if((gamephase == 2)&&(enemyObj[j]->b!=nullptr)) {
                        if(enemyObj[j]->b!=nullptr){
                            enemyObj[j]->b->bhmHP = EnemyHP[j];
                            qDebug() << "babyhoneymon's hp:" << EnemyHP[j];
                            if(atk != 0){
                                charObj[i]->ATKanimation(enemyObj[j]->b->pos());
                                showdamage[j+10]->setVisible(true);
                                showdamagetxt[j+10]+=QString::number(atk)+" ";
                                showdamage[j+10]->setPlainText(showdamagetxt[j+10]);
                            }
                            if(EnemyHP[j] == 0){
                                qDebug() <<"delete baby";
                                showdamage[j+10]->setVisible(false);
                                scene->removeItem(enemyObj[j]->b);
                                delete enemyObj[j]->b;
                                enemyObj[j]->b=nullptr;
                                enemy.erase(enemy.begin()+j);
                                EnemyHP.erase(EnemyHP.begin()+j);
                                enemyObj.erase(enemyObj.begin()+j);
                            }
                        }
                    }
                    else if((gamephase == 3)&&(enemyObj[j]->c!=nullptr)) {
                        if(enemyObj[j]->c!=nullptr){
                            enemyObj[j]->c->cbrHP = EnemyHP[j];
                            qDebug() << "cerberus's hp:" << EnemyHP[j];
                            if(atk != 0){
                                charObj[i]->ATKanimation(enemyObj[j]->c->pos());
                                showdamage[13]->setVisible(true);
                                showdamagetxt[13]+=QString::number(atk)+" ";
                                showdamage[13]->setPlainText(showdamagetxt[13]);
                            }
                            if(EnemyHP[j] == 0){
                                qDebug() <<"delete cerbus";
                                showdamage[13]->setVisible(false);
                                scene->removeItem(enemyObj[j]->c);
                                delete enemyObj[j]->c;
                                enemyObj[j]->c=nullptr;
                                enemy.erase(enemy.begin()+j);
                                EnemyHP.erase(EnemyHP.begin()+j);
                                enemyObj.erase(enemyObj.begin()+j);
                            }
                        }
                    }
                    else {
                        if(enemyObj[j]->s!=nullptr){
                            enemyObj[j]->s->slmHP = EnemyHP[j];
                            qDebug() << "slime's hp:" << EnemyHP[j];
                            if(atk != 0){
                                charObj[i]->ATKanimation(enemyObj[j]->s->pos());
                                showdamage[j+7]->setVisible(true);
                                showdamagetxt[j+7]+=QString::number(atk)+" ";
                                showdamage[j+7]->setPlainText(showdamagetxt[j+7]);

                            }
                            if(EnemyHP[j] == 0){
                                showdamage[j+7]->setVisible(false);
                                qDebug() << "delete slime";
                                scene->removeItem(enemyObj[j]->s);
                                delete enemyObj[j]->s;
                                enemyObj[j]->s=nullptr;
                                enemy.erase(enemy.begin()+j);
                                EnemyHP.erase(EnemyHP.begin()+j);
                                enemyObj.erase(enemyObj.begin()+j);
                            }
                        }
                    }
                }
                if(finishedATK){
                    break;
                }
            }
        }
    }

    for(size_t i=0;i<characters.size();i++){
        qDebug()<<i;
        showdamage[i]->setVisible(true);
        showdamagetxt[i]=QString::number(ATKofChars[characters[i]-1]);
        showdamage[i]->setPlainText(showdamagetxt[i]);
    }
    showdamage[6]->setVisible(true);
    showdamagetxt[6]=QString::number(allpoints[6]);
    showdamage[6]->setPlainText(showdamagetxt[6]);

    if((gamephase < 3)&&(enemyObj.empty())){
        resetENEhp = false;
        qDebug() << "gamephase 1 end";
        gamephase++;
        roundCount = 0;
        addEnemy();
    }
}
void MainWindow::addEnemy(){
    if(gamephase == 1){
        vector<tuple<int, int, int>> slimeInitValues = {
            {2, 80, 230},
            {1, 200, 230},
            {3, 320, 230}
        };
        for (const auto& [attr, x, y] : slimeInitValues) {
            slime* Slime = new slime(attr, x, y);
            scene->addItem(Slime);
            connect(Slime, &slime::updateDamageS, this, &MainWindow::handleSATKChanged);

            enemy.push_back(1);

            enemys *a = new enemys;
            a->s = Slime;
            enemyObj.push_back(a);
        }
    }
    else if(gamephase == 2){
            vector<tuple<int, int, int>> slimeInitValues = {
                {4, 100, 230},
                {5, 320, 230}
            };
            for (const auto& [attr, x, y] : slimeInitValues) {
                slime* Slime = new slime(attr, x, y);
                scene->addItem(Slime);
                connect(Slime, &slime::updateDamageS, this, &MainWindow::handleSATKChanged);

                enemy.push_back(1);

                enemys *a = new enemys;
                a->s = Slime;
                enemyObj.push_back(a);
            }
            babyhoneymon *Babyhoneymon = new babyhoneymon(200, 230);
            scene->addItem(Babyhoneymon);
            connect(Babyhoneymon, &babyhoneymon::updateDamageB, this, &MainWindow::handleBATKChanged);
            weatherStone();

            enemy.push_back(2);

            enemys *a = new enemys;
            a->b = Babyhoneymon;
            enemyObj.push_back(a);
    }
    else if(gamephase == 3){
        cerberus *Cerberus = new cerberus(130, 100);
        scene->addItem(Cerberus);
        connect(Cerberus, &cerberus::updateDamageC, this, &MainWindow::handleCATKChanged);

        enemy.push_back(3);

        enemys *a = new enemys;
        a->c = Cerberus;
        enemyObj.push_back(a);
    }
}

void MainWindow::handleStoneMove(QPointF newGridPos, QPointF oldGridPos) {
    //hpbar->setVisible(false);
    int oldY = floor((oldGridPos.y()-510)/90);
    int oldX = floor(oldGridPos.x()/90);
    int newY = floor((newGridPos.y()-510)/90);
    int newX = floor(newGridPos.x()/90);
    //qDebug() << "Stone Grid positions moved from " << oldGridPos << " to " << newGridPos;
    //qDebug() << "Stone positions moved from (" << oldX << "," << oldY << ") to (" << newX << "," << newY << ")";
    savestone[newY][newX]->setPos(oldGridPos);
    std::swap(savestone[oldY][oldX], savestone[newY][newX]);
    std::swap(stonepos[oldY][oldX], stonepos[newY][newX]);
    if(gamephase == 2){
        if(savestone[oldY][oldX]->weather){
                    savestone[oldY][oldX]->weather = false;
                    savestone[oldY][oldX]->skin(stonepos[oldY][oldX], false, false);
                    qDebug() << "Triggering mouse release on weather stone at:" << oldY << "," << oldX;
            emit forceRel();
            HP-=100;
            emit updateHP(HP);

        }
    }
    else if(gamephase == 3){
        if(savestone[oldY][oldX]->burn){
            HP-=30;
            emit updateHP(HP);
        }
        else{
            savestone[oldY][oldX]->burn = true;
            savestone[oldY][oldX]->skin(stonepos[oldY][oldX], false, true);
        }
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
    //reset
    iffall=0;
    del=0;
    for(int y=0;y<5;y++){
        for(int x=0;x<6;x++){
            combine[y][x]=0;
            iffinded[y][x]=0;
            counterasestone[y][x]=0;
            waitdelete[y][x]=0;
        }
    }
    //qDebug() << "erase";
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
                //qDebug() << "火符石目前消了" << erasestonenum[1] << "顆";
                //qDebug() << "水符石目前消了" << erasestonenum[2] << "顆";
                //qDebug() << "木符石目前消了" << erasestonenum[3] << "顆";
                //qDebug() << "光符石目前消了" << erasestonenum[4] << "顆";
                //qDebug() << "暗符石目前消了" << erasestonenum[5] << "顆";
                //qDebug() << "心符石目前消了" << erasestonenum[6] << "顆";
            }
            if(iffinded[y][x]){
                iffall=1;
                del++;
                find(x,y,del,stonepos[y][x]);
            }
        }
    }
    //erase animation
    if(iffall){
        QTimer *timer = new QTimer(this);
        int count = 0;
        connect(timer, &QTimer::timeout, this, [=]() mutable {
            count++;
            combo++;
            showcombo->setVisible(true);
            combotext="Combo: "+ QString::number(combo);
            showcombo->setPlainText(combotext);

            for(int y=0;y<5;y++){
                for(int x=0;x<6;x++){
                    if(waitdelete[y][x]==count){
                        delete savestone[y][x];
                        savestone[y][x] = nullptr;
                        stonepos[y][x]=0;
                        //qDebug()<<"delete x:"<<x<<" y:"<<y;
                        //qDebug()<<combo;
                    }
                }
            }
            //qDebug()<<"combo"<<count;
            //erase end
            if(count>=del){
                timer->stop();
                //qDebug()<<"tofall";
                //checkmatrix();
                moveTime = true;
                emit tofall();
            }
        });
        timer->start(500);
    }
    else{
        QTimer *timer = new QTimer(this);
        int count = 0;
        connect(timer, &QTimer::timeout, this, [=]() mutable {
            count++;
            combotext="Combo: "+ QString::number(combo);
            showcombo->setPlainText(combotext);
            if(count==1){
                showcombo->setVisible(false);
                emit ATKsign();
                timer->stop();
            }
        });
        timer->start(500);
    }

}

void MainWindow::weatherStone()
{
    bool set1 = false;
    while (!set1) {
        int y1 = rand()%5;
        int x1 = rand()%6;
        if(savestone[y1][x1]->weather == false){
            savestone[y1][x1]->weather = true;
            savestone[y1][x1]->skin(stonepos[y1][x1], true, false);
            set1 = true;
        }
    }
    bool set2 = false;
    while (!set2) {
        int y2 = rand()%5;
        int x2 = rand()%6;
        if(savestone[y2][x2]->weather == false){
            savestone[y2][x2]->weather = true;
            savestone[y2][x2]->skin(stonepos[y2][x2], true, false);
            set2 = true;
        }
    }
}

void MainWindow::fall(){
    //qDebug()<<"falling";
    for(int x=0;x<6;x++){
        for(int y=4;y>0;y--){
            if(stonepos[y][x]==0){
                for(int ty=y-1;ty>=0;ty--){
                    if(stonepos[ty][x]!=0){
                        swap(savestone[ty][x],savestone[y][x]);
                        swap(stonepos[ty][x],stonepos[y][x]);

                        fallanimation(y,x,ty);
                        break;
                    }
                }
            }
        }
    }

    //qDebug()<<"filling";
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
                connect(this, &MainWindow::forceRel, Stone, &stone::forceRelease);
                savestone[y][x]=Stone;
                stonepos[y][x]=type;
                scene->addItem(Stone);
            }
        }
    }

    erasestone();
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

void MainWindow::calATKandHEAL(){
    if(!calculated){
        for(int i = 1; i <= 5; i++){
            allpoints[i] = erasestonenum[i] * combo;
            qDebug() << "第" << i << "種屬性的數值:" << allpoints[i];
        }
        allpoints[6] = erasestonenum[6] * combo *5;
        qDebug() << "第 6 種屬性的數值:" << allpoints[6];
        calculated = true;
    }

}

void MainWindow::handleSATKChanged(int sATK)
{
    this->sATK = sATK;
    calDamage();
}

void MainWindow::handleCATKChanged(int cATK)
{
    this->cATK = cATK;
    calDamage();
}

void MainWindow::handleBATKChanged(int bATK)
{
    this->bATK = bATK;
    calDamage();
}

void MainWindow::calDamage()
{
    damage = sATK + cATK + bATK;
    qDebug() << "Total Damage:" << damage;
}

void MainWindow::calHP(){
    if(ATKform){
        int recovery =allpoints[6];
        if(HP + recovery >= 2000)
            HP = 2000;
        else
            HP = HP + recovery;
    }
    else if(DEFform){
        if(HP <= damage){
            HP = 0;
        }
        else
            HP = HP - damage;
    }
    if(HP==0){
        emit lose();
    }
    emit updateHP(HP);
}

void MainWindow::reset(){
    characters = {};
    enemy.clear();
    charObj.clear();
    enemyObj.clear();

    QList<QGraphicsItem*> Items =scene-> items();
        for(int i =0;i<Items.size();i++){
            QGraphicsItem *item = Items[i];
            if((typeid(*item) == typeid(cerberus)) or (typeid(*item) == typeid(slime)) or (typeid(*item) == typeid(babyhoneymon))){
                scene->removeItem(item);
                delete item;
            }
        }

    gamephase=1;
    roundCount=0;
    addEnemy();
    HP=2000;
    emit updateHP(HP);
    for(int y=0;y<5;y++){
        for(int x=0;x<6;x++){
            F[y][x]=false;
            stonepos[y][x]=0;
            delete savestone[y][x];
            savestone [y][x]=nullptr;
        }
    }
    srand(time(NULL));
    for(int y=0;y<5;y++){
        //vector<stone*> temps;
        for(int x=0;x<6;x++){
            if(F[y][x] == false){
                int type = rand()%6+1;
                stone *Stone = new stone(type, x*a, y*a+510, scene);
                connect(Stone, &stone::stoneMoved, this, &MainWindow::handleStoneMove);
                connect(Stone, &stone::CDover, this, &MainWindow::CDoverEvent);
                connect(this, &MainWindow::forceRel, Stone, &stone::forceRelease);
                //temps.push_back(Stone);
                savestone[y][x]=Stone;
                stonepos[y][x]=type;
                scene->addItem(Stone);
                F[y][x] = true;
            }

        }
        //savestone.push_back(temps);
    }
    for(int i=0;i<14;i++){
        showdamage[i]->setVisible(false);
    }
}
