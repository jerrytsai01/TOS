#include "cerberus.h"
#include "slime.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

cerberus::cerberus(int cbrX, int cbrY)
{
    setPixmap(QPixmap(":/new/prefix1/dataset/enemy/180n.png"));
    setPos(cbrX,cbrY);
}

//wu{
void cerberus::killcbr(){
    if(cbrHP == 0) {
        scene()->removeItem(this);
        delete this;
    }
}

void cerberus::cbrATK(){
    if(cbrHP > 0){
        if(cbrATKcd == 1){
                int cATK = cbrATKpow;
                emit updateDamageC(cATK);
                cbrATKcd = 5;
        }
        else cbrATKcd--;
    }
}
//}wu
