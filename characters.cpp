#include "characters.h"
#include "attribute.h"
#include <QGraphicsPixmapItem>
Characters::Characters(int ID, QObject *parent): QObject(parent), id(ID)
{
    switch (id) {
    case 1:
        setPixmap(QPixmap(":/new/prefix1/dataset/character/ID1.png"));
        type = Attribute::Water;
        break;
    case 2:
        setPixmap(QPixmap(":/new/prefix1/dataset/character/ID2.png"));
        type = Attribute::Fire;
        break;
    case 3:
        setPixmap(QPixmap(":/new/prefix1/dataset/character/ID3.png"));
        type = Attribute::Earth;
        break;
    case 4:
        setPixmap(QPixmap(":/new/prefix1/dataset/character/ID4.png"));
        type = Attribute::Light;
        break;
    default:
        setPixmap(QPixmap(":/new/prefix1/dataset/character/ID5.png"));
        type = Attribute::Dark;
        break;
    }
}
