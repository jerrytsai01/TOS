#ifndef STONE_H
#define STONE_H
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsView>
#include "attribute.h"

class stone: public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
public:
    stone(int type,int X,int Y, QObject *parent = nullptr); // 使用整数类型的构造函数

    Attribute getType() const;
    int getTypeAsInt() const;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *reEvent);
    void skin(int type,bool weather, bool burn);
    bool weather = false, burn = false;
signals:
    void stoneMoved(QPointF newGirdPos, QPointF oldGridPos);
public slots:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
private:
    Attribute type = getType();
    bool pressed = false;
    QTimer *pressTimer;
    int pressTimeCount = 0;
    QPointF mousePoint, originalPos, newGridPos, oldGridPos;
};

#endif // STONE_H
