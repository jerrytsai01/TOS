#ifndef CDBAR_H
#define CDBAR_H

#include <QGraphicsPixmapItem>
#include <QObject>
class CDbar:public QGraphicsPixmapItem
{
public:
    CDbar(float remainingTime);
};

#endif // CDBAR_H
