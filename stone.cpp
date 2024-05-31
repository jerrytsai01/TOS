#include "stone.h"
#include "attribute.h"
#include <QGraphicsPixmapItem>
// 构造函数实现
stone::stone(int type, int X, int Y) : type(static_cast<Attribute>(type)) {
    setPos(X,Y);
    skin(type, weather, burn);
}

// 获取符石类型的方法实现
Attribute stone::getType() const {
    return type;
}

// 获取符石类型的整数值
int stone::getTypeAsInt() const {
    return static_cast<int>(type);
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
            setPixmap(QPixmap(":/new/prefix1/dataset/runestone/burning_earth_stone.png"));
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

