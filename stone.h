#ifndef STONE_H
#define STONE_H
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsView>
#include "attribute.h"

class stone: public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
public:
    stone(int type,int X,int Y); // 使用整数类型的构造函数

    Attribute getType() const;
    int getTypeAsInt() const;
    void skin(int type,bool weather, bool burn);
    bool weather = false, burn = false;
private:
    Attribute type = getType();
};

#endif // STONE_H
