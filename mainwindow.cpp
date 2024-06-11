#include "mainwindow.h"
#include "stone.h"
#include "math.h"
#include "hpbar.h"
#include "cdbar.h"
#include "slime.h"
#include "babyhoneymon.h"
#include "cerberus.h"
#include "characters.h"
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
    // 创建阴影效果作为边框
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect();
    shadowEffect->setOffset(0);  // 使阴影紧贴文本，产生边框效果
    shadowEffect->setBlurRadius(5);  // 控制边框的模糊半径
    shadowEffect->setColor(Qt::black);  // 边框颜色为黑色
    showcombo->setGraphicsEffect(shadowEffect);
    scene->addItem(showcombo);

    connect(this,&MainWindow::tofall,this,&MainWindow::fall);
    addEnemy();
    HPbar *hpBar = new HPbar();
    connect( this, &MainWindow::updateHP,hpBar, &HPbar::updateHPBar);
    scene->addItem(hpBar);
    //erasestone();
    //fall();

    setBtn = new QPushButton(this);
    setBtn->setIcon(QIcon(":/new/prefix1/dataset/character/ID4.png"));
    setBtn->setFixedSize(40, 40);
    setBtn->setIconSize(setBtn->size());
    setBtn->move(500,0);
    s = new setwindow();
    connect(setBtn, &QPushButton::clicked, s, &setwindow::show);
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

    qDebug() <<"EnemyOBjs :";
    for(auto it: enemyObj){
        if(it->b != nullptr){
            qDebug() << "Slime";
        }
        else if(it->c!=nullptr){
            qDebug() <<"cerb";
        }
        else if(it->s != nullptr){
            qDebug() << "baby";
        }
        else {
            qDebug() <<"null";
        }
    }
    qDebug() << enemy;
    combo=0;
    for(int i=1;i<=6;i++){
        erasestonenum[i]=0;
    }
    erasestone();

    //攻擊寫這裡
    CharsATK();
    //
    if(std::find(enemy.begin(),enemy.end(), 2) != enemy.end()){
        weatherStone();
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
    moveTime = true;
}

void MainWindow::CharsATK(){
    qDebug()<<"ATK";
    if((gamephase == 1)&&(!resetENEhp)){
        for(int i = 0; i < 3; i++){
            EnemyHP[i] = 100;
        }
    }
    else if((gamephase == 2)&&(!resetENEhp)){
        EnemyHP[0] = 100;
        EnemyHP[1] = 300;
        EnemyHP[2] = 100;
    }
    else if((gamephase == 3)&&(!resetENEhp)){
        EnemyHP[1] = 700;
    }
    if((gamephase < 3)&&(EnemyHP[0] + EnemyHP[1] + EnemyHP[2] == 0)){
        resetENEhp = false;
        gamephase++;
    }
    for(int i = 0; i < 6; i++){
        for(int j = 1; j <= 5; j++){
            if(characters[i] == j){
                ATKofChars[i] = allpoints[j - 1];
                break;
            }
        }
    }
    for(int i = 0; i < 6; i++){
        int atk = 0;
        for(int j = 0; j < 3; j++){
            if(EnemyHP[j] == 0)
                continue;
            else {
                if(gamephase == 1){
                    if(j == 0){
                        switch(characters[i]){
                        case(1) :
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
                    else if(j == 1){
                        switch(characters[i]){
                        case(2) :
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
                            atk = ATKofChars[i] * 2;
                            break;
                        case(2) :
                            atk = ATKofChars[i] * 0.5;
                            break;
                        default :
                            atk = ATKofChars[i];
                            break;
                        }
                    }
                }
                else if(gamephase == 2){
                    if(j == 0){
                        switch(characters[i]){
                        case(5) :
                            atk = ATKofChars[i] * 2;
                            break;
                        default :
                            atk = ATKofChars[i];
                            break;
                        }
                    }
                    else if(j == 1){
                        switch(characters[i]){
                        case(1) :
                            atk = ATKofChars[i] * 2;
                            break;
                        case(2) :
                            atk = ATKofChars[i] * 0.5;
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
                    case(2) :
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
            }
            if(EnemyHP[j] <= atk){
                EnemyHP[j] = 0;
            }
            else {
                EnemyHP[j] = EnemyHP[j] - atk;
            }
            if((gamephase == 2)&&(enemyObj[j]->b!=nullptr)&&(EnemyHP[j]!=0)) {
                if(enemyObj[j]->b!=nullptr){
                    enemyObj[j]->b->bhmHP = EnemyHP[j];
                    charObj[i]->ATKanimation(enemyObj[j]->b->pos());
                }
            }
            else if((gamephase == 3)&&(enemyObj[j]->c!=nullptr)&&(EnemyHP[j]!=0)) {
                if(enemyObj[j]->c!=nullptr){
                    enemyObj[j]->c->cbrHP = EnemyHP[j];
                    charObj[i]->ATKanimation(enemyObj[j]->c->pos());
                }
            }
            else {
                if(enemyObj[j]->s!=nullptr&&(EnemyHP[j]!=0)){
                    enemyObj[j]->s->slmHP = EnemyHP[j];
                    charObj[i]->ATKanimation(enemyObj[j]->s->pos());
                }
            }
            if(EnemyHP[j] == 0){
                qDebug() << "Delete " << enemy[j];
                enemy.erase(enemy.begin()+j);
                enemyObj.erase(enemyObj.begin()+j);

            }
        }
    }
}

void MainWindow::addEnemy(){
    if(gamephase == 1){
        vector<tuple<int, int, int>> slimeInitValues = {
            {1, 80, 250},
            {3, 200, 250},
            {2, 320, 250}
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
                {4, 100, 250},
                {5, 320, 250}
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
            babyhoneymon *Babyhoneymon = new babyhoneymon(200, 250);
            scene->addItem(Babyhoneymon);
            connect(Babyhoneymon, &babyhoneymon::updateDamageB, this, &MainWindow::handleBATKChanged);
            weatherStone();

            enemy.push_back(2);

            enemys *a = new enemys;
            a->b = Babyhoneymon;
            enemyObj.push_back(a);
    }
    else if(gamephase == 3){
        cerberus *Cerberus = new cerberus(200, 250);
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
    if(gamephase == 2){
        if(savestone[newY][newX]->weather){
            //qDebug() << "Triggering mouse release on weather stone at:" << newY << "," << newX;
            emit forceRel();
            HP-=100;
            emit updateHP(HP);
            /*
            std::swap(savestone[oldY][oldX], savestone[newY][newX]);
            std::swap(stonepos[oldY][oldX], stonepos[newY][newX]);
            return; // 提前返回，不执行后续逻辑*/
        }
    }
    else if(gamephase == 3){
        if(savestone[newY][newX]->burn){
            HP-=30;
            emit updateHP(HP);
        }
        else{
            savestone[newY][newX]->burn = true;
            savestone[newY][newX]->skin(stonepos[newY][newX], false, true);
        }
    }
    std::swap(savestone[oldY][oldX], savestone[newY][newX]);
    std::swap(stonepos[oldY][oldX], stonepos[newY][newX]);
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
    for(int i = 0; i < 5; i++){
        allpoints[i] = erasestonenum[i] * combo;
    }
    allpoints[5] = erasestonenum[5] * combo *5;
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
        int recovery = combo * allpoints[5] * 5;
        if(HP + recovery >= 2000)
            HP = 2000;
        else HP = HP + recovery;
    }
    else if(DEFform){
        if(HP <= damage)
            HP = 0;
        else HP = HP - damage;
    }
    emit updateHP(HP);
}
