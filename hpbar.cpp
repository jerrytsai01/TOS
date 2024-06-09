#include "hpbar.h"
#include "mainwindow.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

HPbar::HPbar()
{
    fullPixmap = QPixmap(":/new/prefix1/dataset/hp_bar.png");
    setPixmap(fullPixmap);
    setPos(52,480);
}

void HPbar::updateHPBar(int HP) {
    int fullWidth = fullPixmap.width();
    int displayedWidth = (HP * fullWidth) / MaxHP;
    QPixmap croppedPixmap = fullPixmap.copy(0, 0, displayedWidth, fullPixmap.height());
    setPixmap(croppedPixmap);
}
