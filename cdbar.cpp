#include "cdbar.h"
#include <QGraphicsPixmapItem>

CDbar::CDbar(float remainingTime)
{
    QPixmap a(":/new/prefix1/dataset/cd_bar.png");
    setPixmap(QPixmap(":/new/prefix1/dataset/cd_bar.png"));
    setPos(52,480);
}
