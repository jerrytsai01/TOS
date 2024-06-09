#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <vector>

class loginwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit loginwindow(QWidget *parent = nullptr);

public slots:
    void characterSelected(int characterID);
    void startGame();
signals:
    void start(std::vector<int> cha);
private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    QPushButton *C1, *C2, *C3, *C4, *C5, *Start;
    std::vector<int> characters;
};

#endif // LOGINWINDOW_H
