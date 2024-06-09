#ifndef HPBAR_H
#define HPBAR_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include "mainwindow.h"
#include "slime.h"

class MainWindow;
class slime;

class HPbar: public QObject, public QGraphicsPixmapItem
{
public:
    HPbar();
    int hp = 2000;
    const int MAXhp = 2000;
public slots:
    void recovery();
    void attackedbyENE();
    void updateHPBar();
private:
    MainWindow *mainwindow;
    slime *Slime;
    QPixmap fullPixmap;
    int damage = 0;
};

#endif // HPBAR_H
