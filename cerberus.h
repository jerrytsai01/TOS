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
    int cbrHP = 700;
private slots:
    void cbrDEF();
    void cbrATK();
private:
    int cbrATKpow = 400;
    int cbrATKcd = 5;
};

#endif // CERBERUS_H






