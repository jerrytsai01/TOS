#ifndef SLIME_H
#define SLIME_H

#include "attribute.h"
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsView>
#include <vector>

using namespace std;

class slime:public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
public:
    slime(int attr, int slmX, int slmY, QObject *parent = nullptr);
    void readview(QGraphicsView *view);
    int slmHP = 100;
    static bool RESTform, DEFform, ATKform;
    static int ATKofPlayer[6];
    static int ATKofEnemy[5];
private slots:
    void CharacterOfEnemy();
    void slmDEF();
    void slmATK();
private:
    Attribute attr;
    bool reset = true;
    int slmATKpow = 200;
    int slmATKcd = 3;
};

#endif // SLIME_H

