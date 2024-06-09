#ifndef HPBAR_H
#define HPBAR_H

//wu{
#include <QGraphicsPixmapItem>
#include <QObject>

class HPbar:public QObject,public QGraphicsPixmapItem
{
public:
    HPbar();

public slots:
    void updateHPBar(int HP);
private:
    QPixmap fullPixmap;
    const int MaxHP = 2000;
};
//}wu

#endif // HPBAR_H
