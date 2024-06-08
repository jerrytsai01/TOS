#ifndef CDBAR_H
#define CDBAR_H

#include <QGraphicsPixmapItem>
#include <QObject>
class CDbar:public QGraphicsPixmapItem
{
public:
    CDbar();
    void updateBar(float remainingTime);
private:
    QPixmap fullPixmap;
};

#endif // CDBAR_H
