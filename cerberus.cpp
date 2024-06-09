#include "cerberus.h"
#include "slime.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

cerberus::cerberus(int cbrX, int cbrY, QObject *parent)
{
    setPixmap(QPixmap(":/new/prefix1/dataset/enemy/180n.png"));
    setPos(cbrX,cbrY);
}

void cerberus::cbrDEF(){
    if(slime::DEFform){
        for(int i = 0; i < 6; i++){
            cbrHP = cbrHP - slime::ATKofPlayer[i];
            slime::ATKofPlayer[i] = 0;
            if(cbrHP <= 0){
                delete this;
                break;
            }
        }
    }
}

void cerberus::cbrATK(){
    if((slime::ATKform) && (cbrHP > 0)){
        if(cbrATKcd == 1){
                int cATK = cbrATKpow;
                emit updateDamageC(cATK);
                cbrATKcd = 5;
        }
        else cbrATKcd--;
    }
}

