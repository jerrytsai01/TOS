#include "loginwindow.h"
#include <QGraphicsPixmapItem>
#include <vector>
loginwindow::loginwindow(QWidget *parent) : QMainWindow(parent)
{
    setFixedSize(540,960);
    setWindowTitle("Log in");

    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, 540,960);
    view = new QGraphicsView(scene);
    view->setFixedSize(540,960);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 關閉水平滾動條
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 關閉垂直滾動條

    C1 = new QPushButton(view);
    C1->setIcon(QIcon(":/new/prefix1/dataset/character/ID1.png"));
    C1->setFixedSize(90, 90);
    C1->setIconSize(C1->size());
    C1->move(20,400);

    C2 = new QPushButton(view);
    C2->setIcon(QIcon(":/new/prefix1/dataset/character/ID2.png"));
    C2->setFixedSize(90, 90);
    C2->setIconSize(C1->size());
    C2->move(120,400);

    C3 = new QPushButton(view);
    C3->setIcon(QIcon(":/new/prefix1/dataset/character/ID3.png"));
    C3->setFixedSize(90, 90);
    C3->setIconSize(C3->size());
    C3->move(220,400);

    C4 = new QPushButton(view);
    C4->setIcon(QIcon(":/new/prefix1/dataset/character/ID4.png"));
    C4->setFixedSize(90, 90);
    C4->setIconSize(C4->size());
    C4->move(320,400);

    C5 = new QPushButton(view);
    C5->setIcon(QIcon(":/new/prefix1/dataset/character/ID5.png"));
    C5->setFixedSize(90, 90);
    C5->setIconSize(C5->size());
    C5->move(420,400);

    Start = new QPushButton(view);
    Start->setIcon(QIcon(":/new/prefix1/dataset/start button.jpg"));
    Start->setFixedSize(90, 40);
    Start->setIconSize(Start->size());
    Start->move(220,700);

    connect(C1, &QPushButton::clicked, [this](){ characterSelected(1); });
    connect(C2, &QPushButton::clicked, [this](){ characterSelected(2); });
    connect(C3, &QPushButton::clicked, [this](){ characterSelected(3); });
    connect(C4, &QPushButton::clicked, [this](){ characterSelected(4); });
    connect(C5, &QPushButton::clicked, [this](){ characterSelected(5); });
    connect(Start, &QPushButton::clicked, this, &loginwindow::startGame);
    connect(Start, &QPushButton::clicked, this, &QWidget::close);

    setCentralWidget(view);
    characters = {};
}

void loginwindow::characterSelected(int characterID)
{
    if(characters.size() < 6){
        characters.push_back(characterID);
        QPixmap aP(QString(":/new/prefix1/dataset/character/ID%1.png").arg(characterID));
        QGraphicsPixmapItem *a = new QGraphicsPixmapItem(aP);
        t.push_back(a);
        int x = (characters.size() - 1) * 90;
        a->setPos(x, 800);
        scene->addItem(a);
    }
}

void loginwindow::startGame()
{
    if(characters.empty()){
        for(int i=1;i<6;i++){
                   characters.push_back(i);
               }
    }
    emit start(characters);
}

void loginwindow::reset(){
    for(size_t  i=0;i<t.size();i++){
        scene->removeItem(t[i]);
        delete t[i];
    }
    t.clear();
    characters.clear();
}
