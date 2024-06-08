#include "cdbar.h"
#include <QGraphicsPixmapItem>
#include "qdebug.h"

CDbar::CDbar() {
    fullPixmap = QPixmap(":/new/prefix1/dataset/cd_bar.png");
    setPixmap(fullPixmap);
    setPos(52, 480);
}

void CDbar::updateBar(float remainingTime) {
    qDebug() << remainingTime;
    // Calculate the width of the pixmap to display
    int newWidth = static_cast<int>(fullPixmap.width() * remainingTime);
    QPixmap croppedPixmap = fullPixmap.copy(0, 0, newWidth, fullPixmap.height());
    setPixmap(croppedPixmap);
}
