#include "babyhoneymon.h"
#include "slime.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

babyhoneymon::babyhoneymon(int bhmX, int bhmY)
{
    setPixmap(QPixmap(":/new/prefix1/dataset/enemy/267n.png"));
    setPos(bhmX,bhmY);
}

//wu{
void babyhoneymon::killbhm(){
    if(bhmHP == 0) {
        delete this;
    }
}

void babyhoneymon::bhmATK(){
    if(bhmHP > 0){
        if(bhmATKcd == 1){
            int bATK = bhmATKpow;
            emit updateDamageB(bATK);
            bhmATKcd = 3;
        }
        else bhmATKcd--;
    }
}
//}wu
