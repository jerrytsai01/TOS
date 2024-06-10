#ifndef CDBAR_H
#define CDBAR_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsRectItem>

class CDbar:public QGraphicsPixmapItem
{
public:
    CDbar();
    void updateBar(float remainingTime);
private:
    QPixmap fullPixmap;
    QGraphicsRectItem *background;
};

#endif // CDBAR_H
