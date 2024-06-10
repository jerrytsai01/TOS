#ifndef CHARACTERS_H
#define CHARACTERS_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include "attribute.h"

class Characters: public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
public:
    Characters(int ID, QObject *parent = nullptr);
    Attribute type;
    void ATKanimation(QPointF enemyPos);
private:
    int id;
};

#endif // CHARACTERS_H
