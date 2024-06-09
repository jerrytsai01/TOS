#include "babyhoneymon.h"
#include "slime.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

babyhoneymon::babyhoneymon(int bhmX, int bhmY, QObject *parent)
{
    setPixmap(QPixmap(":/new/prefix1/dataset/enemy/267n.png"));
    setPos(bhmX,bhmY);
}

void babyhoneymon::bhmDEF(){
    if(slime::DEFform){
        for(int i = 0; i < 6; i++){
            bhmHP = bhmHP - slime::ATKofPlayer[i];
            slime::ATKofPlayer[i] = 0;
            if(bhmHP <= 0){
                delete this;
                break;
            }
        }
    }
}

void babyhoneymon::bhmATK(){
    if((slime::ATKform) && (bhmHP > 0)){
        if(bhmATKcd == 1){
            int bATK = bhmATKpow;
            emit updateDamageB(bATK);
            bhmATKcd = 3;
        }
        else bhmATKcd--;
    }
}
