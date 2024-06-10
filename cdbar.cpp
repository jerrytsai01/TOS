#include "cdbar.h"
#include <QGraphicsPixmapItem>
#include <QBrush>
#include "qdebug.h"

CDbar::CDbar() {
    fullPixmap = QPixmap(":/new/prefix1/dataset/cd_bar.png");
    setPixmap(fullPixmap);
    setPos(52, 480);
    setZValue(2);
    // Initialize the background item
    background = new QGraphicsRectItem(0, 0, 100, 10, this);  // Adjust size as needed
    background->setBrush(QBrush(Qt::black));
    background->setZValue(1);  // Make sure the background is behind the CD bar
    background->setPos(52, 480);
}

void CDbar::updateBar(float remainingTime) {
    qDebug() << remainingTime;
    // Calculate the width of the pixmap to display
    int newWidth = static_cast<int>(fullPixmap.width() * remainingTime);
    QPixmap croppedPixmap = fullPixmap.copy(0, 0, newWidth, fullPixmap.height());
    setPixmap(croppedPixmap);
}
