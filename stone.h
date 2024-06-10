#ifndef STONE_H
#define STONE_H
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QObject>
#include <QGraphicsView>
#include "attribute.h"
#include "cdbar.h"

class stone: public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
public:
    stone(int type,int X,int Y, QGraphicsScene *scene, QObject *parent = nullptr); // 使用整数类型的构造函数
    Attribute getType() const;
    Attribute type = getType();
    int getTypeAsInt() const;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *reEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void skin(int type, bool weather, bool burn);
    bool weather = false, burn = false;
    int pressTimeCount = 0;
    bool pressed = false;
signals:
    void stoneMoved(QPointF newGirdPos, QPointF oldGridPos);
    void CDover();
public slots:
    void emitTimer();
    void forceRelease();
private:
    QTimer *pressTimer;
    QPointF mousePoint, originalPos, newGridPos, oldGridPos;
    int const totalTime = 1000;
    CDbar *cdBar;
    QGraphicsPixmapItem *cdIcon;
};

#endif // STONE_H
