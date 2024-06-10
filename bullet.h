#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsPixmapItem>
#include <QGraphicsEllipseItem>
#include <QObject>
#include <QGraphicsView>

class bullet:public QObject, public QGraphicsEllipseItem{
    Q_OBJECT
public:
    bullet(QGraphicsPixmapItem *parent = 0);
    void readview(QGraphicsView *view);
    void setTargetPosition(const QPointF &target, const QPointF &start);
public slots:
    void move();
private:
    qreal angle;
    qreal speed;
    QPointF target;  // 目标位置
};

#endif // BULLET_H

