#ifndef CERBERUS_H
#define CERBERUS_H

#include "attribute.h"
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsView>
#include <vector>

using namespace std;

class cerberus: public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
public:
    cerberus(int cbrX, int cbrY, QObject *parent = nullptr);
    Attribute: 1;
    void cbrATK();
    int cbrHP = 700;
signals:
    void updateDamageC(int cATK);

private slots:
    void cbrDEF();
private:
    int cbrATKpow = 400;
    int cbrATKcd = 5;
};

#endif // CERBERUS_H






