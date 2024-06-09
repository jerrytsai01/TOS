#ifndef BABYHONEYMON_H
#define BABYHONEYMON_H

#include "attribute.h"
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsView>
#include <vector>

using namespace std;

class babyhoneymon: public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
public:
    babyhoneymon(int bhmX, int bhmY, QObject *parent = nullptr);
    Attribute: 3;
    int bhmHP = 300;
    void bhmATK();
signals:
    void updateDamageB(int bATK);
private slots:
    void bhmDEF();

private:
    int bhmATKpow = 100;
    int bhmATKcd = 3;
};

#endif // BABYHONEYMON_H




