#include "stone.h"
#include "attribute.h"
#include "math.h"
#include "mainwindow.h"
#include <vector>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QApplication>
#include <QDebug>
#include <QTimer>
// 构造函数实现
stone::stone(int type, int X, int Y, QGraphicsScene *scene, QObject *parent)
    : QObject(parent), type(static_cast<Attribute>(type)) {
    setPos(X,Y);
    skin(type, weather, burn);
    pressTimer = new QTimer(this);
    pressTimer->setInterval(10); // Set timer interval, e.g., 2000 milliseconds (2 seconds)
    connect(pressTimer, SIGNAL(timeout()), this, SLOT(emitTimer()));
    // Initialize the cdBar
    cdBar = new CDbar(); // Initially full
    cdBar->setParentItem(this); // Make it a child of the stone
    scene->addItem(cdBar);
    cdBar->setVisible(false);
    cdBar->setZValue(9);
    QPixmap cdIconPixmap(":/new/prefix1/dataset/cd_icon.png");
    cdIcon = new QGraphicsPixmapItem(cdIconPixmap);
    cdIcon->setPos(0,465);
    scene->addItem(cdIcon);
    cdIcon->setVisible(false);
}

// 获取符石类型的方法实现
Attribute stone::getType() const {
    return type;
}

// 获取符石类型的整数值
int stone::getTypeAsInt() const {
    return static_cast<int>(type);
}
// mouse event
void stone::mousePressEvent(QGraphicsSceneMouseEvent *event) {
/*
    pressed = true;
    oldGridPos.setX(floor(pos().x() / 90) * 90);
    oldGridPos.setY(510 + floor((pos().y() - 510) / 90) * 90);
    mousePoint = event->scenePos();
    //qDebug() << "Mouse Pressed at Scene Position:" << mousePoint;
    pressTimer->start(10);
    cdBar->setVisible(true);
    cdIcon->setVisible(true);
    */
    if(MainWindow::moveTime){
            pressed = true;
            oldGridPos.setX(floor(pos().x() / 90) * 90);
            oldGridPos.setY(510 + floor((pos().y() - 510) / 90) * 90);
            mousePoint = event->scenePos();
            //qDebug() << "Mouse Pressed at Scene Position:" << mousePoint;
            pressTimer->start(10);
            cdBar->setVisible(true);
            cdIcon->setVisible(true);
        }
}
void stone::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    qDebug() << "mouseReleaseEvent triggered";
    pressed = false;
    pressTimer->stop(); // Stop the timer
    pressTimeCount = 1000;
    emit CDover();
    //qDebug() <<"Release";
    // Calculate the grid position for the stone
    int newX = qRound(pos().x() / 90) * 90 + 45; // Round to the nearest multiple of 90 (grid size) and add offset for center
    int newY = 510 + qRound((pos().y() - 510) / 90) * 90 + 45; // Round to the nearest multiple of 90 (grid size) and add offset for center
    setPos(newX - 45, newY - 45); // Update stone position to the center of the grid
    //qDebug() << "Stone dropped at:" << QPointF(newX, newY);
    QGraphicsItem::mouseReleaseEvent(event); // Call the base class implementation
    // Ensure cdBar is reset or hidden
    cdBar->setVisible(false);
    cdIcon->setVisible(false);
    cdBar->updateBar(1.0f);
}

void stone::emitTimer()
{
    if(!pressed)
        pressTimeCount = 0;
    else{
        pressTimeCount++;
    }
    // Calculate remaining time fraction
    float remainingTimeFraction = static_cast<float>(1000 - pressTimeCount) / 1000.0f;
    cdBar->updateBar(remainingTimeFraction); // Update the cdBar
}

void stone::forceRelease()
{
    qDebug() << "forceReleaseEvent triggered";
    pressTimeCount = 1000;
}

void stone::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    // Update the width of the timer rectangle
    if (pressed and pressTimeCount <= 1000) {
        QPointF newPos = event->scenePos(); // Get new mouse position
        if(event->scenePos().y() <= 510){
            newPos.setY(510);
        }
        if(event->scenePos().x() <= 0){
            newPos.setX(45);
        }
        if(event->scenePos().x() >= 540){
            newPos.setX(495);
        }
        if(event->scenePos().y() >= 960){
            newPos.setY(915);
        }
        setPos(newPos.x()-45,newPos.y()-45); // Update stone position
        newGridPos.setX(floor(newPos.x() / 90) * 90);
        newGridPos.setY(510 + floor((newPos.y() - 510) / 90) * 90);
        //qDebug() << "Mouse Moved to Scene Position:" << mousePoint;
        if (newGridPos != oldGridPos) {
            //qDebug() << "Move from "<<oldGridPos<<" to "<<newGridPos;
            emit stoneMoved(newGridPos, oldGridPos);
            oldGridPos = newGridPos;
        }
    }
    else{
        pressed = false;
        pressTimer->stop();
        pressTimeCount = 0;
        //qDebug() << "Drag time limit reached. Stopping drag.";
        emit CDover();
        // Calculate the grid position for the stone
        int newX = floor(pos().x() / 90) * 90 + 45; // Round to the nearest multiple of 90 (grid size) and add offset for center
        int newY = 510 + floor((pos().y() - 510) / 90) * 90 + 45; // Round to the nearest multiple of 90 (grid size) and add offset for center
        setPos(newX - 45, newY - 45); // Update stone position to the center of the grid
        //qDebug() << "Stone dropped at:" << QPointF(newX, newY);
        QGraphicsItem::mouseReleaseEvent(event); // Call the base class implementation
        // Ensure cdBar is reset or hidden
        cdBar->setVisible(false);
        cdIcon->setVisible(false);
        cdBar->updateBar(1.0f);
    }
}
void stone::skin(int type, bool weather, bool burn)
{
    if(weather){
        switch (type) {
        case 1:
            setPixmap(QPixmap(":/new/prefix1/dataset/runestone/weathered_fire_stone.png"));
            break;
        case 2:
            setPixmap(QPixmap(":/new/prefix1/dataset/runestone/weathered_water_stone.png"));
            break;
        case 3:
            setPixmap(QPixmap(":/new/prefix1/dataset/runestone/weathered_earth_stone.png"));
            break;
        case 4:
            setPixmap(QPixmap(":/new/prefix1/dataset/runestone/weathered_light_stone.png"));
            break;
        case 5:
            setPixmap(QPixmap(":/new/prefix1/dataset/runestone/weathered_dark_stone.png"));
            break;
        default:
            setPixmap(QPixmap(":/new/prefix1/dataset/runestone/weathered_heart_stone.png"));
            break;
        }
    }
    else if(burn){
        switch (type) {
        case 1:
            setPixmap(QPixmap(":/new/prefix1/dataset/runestone/burning_fire_stone.png"));
            break;
        case 2:
            setPixmap(QPixmap(":/new/prefix1/dataset/runestone/burning_water_stone.png"));
            break;
        case 3:
            setPixmap(QPixmap(":/new/prefix1/dataset/runestone/burnibg_earth_stone.png"));
            break;
        case 4:
            setPixmap(QPixmap(":/new/prefix1/dataset/runestone/burning_light_stone.png"));
            break;
        case 5:
            setPixmap(QPixmap(":/new/prefix1/dataset/runestone/burning_dark_stone.png"));
            break;
        default:
            setPixmap(QPixmap(":/new/prefix1/dataset/runestone/burning_heart_stone.png"));
            break;
        }
    }
    else{
        switch (type) {
        case 1:
            setPixmap(QPixmap(":/new/prefix1/dataset/runestone/fire_stone.png"));
            break;
        case 2:
            setPixmap(QPixmap(":/new/prefix1/dataset/runestone/water_stone.png"));
            break;
        case 3:
            setPixmap(QPixmap(":/new/prefix1/dataset/runestone/earth_stone.png"));
            break;
        case 4:
            setPixmap(QPixmap(":/new/prefix1/dataset/runestone/light_stone.png"));
            break;
        case 5:
            setPixmap(QPixmap(":/new/prefix1/dataset/runestone/dark_stone.png"));
            break;
        default:
            setPixmap(QPixmap(":/new/prefix1/dataset/runestone/heart_stone.png"));
            break;
        }
    }
}


