#include "slime.h"
#include "hpbar.h"
#include "attribute.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QTimer>
#include <vector>

bool slime::RESTform = true;
bool slime::DEFform = false;
bool slime::ATKform = false;
int slime::ATKofPlayer[6] = {0, 0, 0, 0, 0, 0};


slime::slime(int attr, int slmX, int slmY, QObject *parent)
    : QObject(parent), attr(static_cast<Attribute>(attr))
{
    setPos(slmX,slmY);
    CharacterOfEnemy();
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slmDEF));
    connect(timer, SIGNAL(timeout()), this, SLOT(slmATK));
    timer->start(10);
}

void slime::CharacterOfEnemy(){
    if(reset){
        reset = false;
        switch(attr){
        //fire slime
        case Attribute::Fire:
            setPixmap(QPixmap(":/new/prefix1/dataset/enemy/98n.png"));
            break;

        //water slime
        case Attribute::Water:
            setPixmap(QPixmap(":/new/prefix1/dataset/enemy/96n.png"));
            break;

        //earth slime
        case Attribute::Earth:
            setPixmap(QPixmap(":/new/prefix1/dataset/enemy/100n.png"));
            break;

        //light slime
        case Attribute::Light:
            setPixmap(QPixmap(":/new/prefix1/dataset/enemy/102n.png"));
            break;

        //dark slime
        case Attribute::Dark:
            setPixmap(QPixmap(":/new/prefix1/dataset/enemy/104n.png"));
            break;

        default :
            break;
        }
    }
}

void slime::slmDEF(){
    if(DEFform){
        for(int i = 0; i < 6; i++){
            slmHP = slmHP - ATKofPlayer[i];
            ATKofPlayer[i] = 0;
            if(slmHP <= 0){
                scene()->removeItem(this);
                delete this;
                break;
            }
        }
    }
}

void slime::slmATK(){
    if((ATKform) && (slmHP > 0)){
        if(slmATKcd == 1){
                int sATK = slmATKpow;
                emit updateDamageS(sATK);
                slmATKcd = 3;
        }
        else slmATKcd--;
    }
}


