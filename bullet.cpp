#include "bullet.h"
#include <QDebug>
#include <QTimer>
#include <QBrush>
#include <QPen>
#include <QGraphicsScene>
#include <QList>
#include <QtMath>

bullet::bullet(QGraphicsPixmapItem *parent): QObject(), QGraphicsEllipseItem(parent), speed(10)
{
    // Set the size and color of the bullet
    setRect(0, 0, 20, 20);
    setBrush(Qt::black);
    setPen(Qt::NoPen);
    speed = 10;
    QTimer*timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    timer->start(10);
}

void bullet::setTargetPosition(const QPointF &t, const QPointF &s)
{
    QPointF start(s.x()+45, s.y()+45);
    QPointF target(t.x()+45, t.y()+45);
    //qDebug() << "Target set start" << start << " to " <<target;
    // Calculate the angle between the start position and the target position
    angle = qAtan2(target.y() - (start.y()), target.x() - (start.x()));

    setPos(start);
    this->target = target;  // 保存目标位置
}

void bullet::move()
{
    //qDebug() << "move";
    if (pos() != target)
    {
        qreal dx = speed * qCos(angle);
        qreal dy = speed * qSin(angle);
        QPointF newPos = pos() + QPointF(dx, dy);
        // Set the new position of the bullet
        setPos(newPos);
    }
    else
    {
        scene()->removeItem(this);
        delete this;
    }
}
