#ifndef ENDWINDOW_H
#define ENDWINDOW_H
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QLabel>

class endwindow: public QMainWindow
{
    Q_OBJECT

public:
    endwindow(QWidget *parent = nullptr);

signals:
    void openl();

public slots:
    void gg();

private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    QPushButton *restart;
    QLabel *result ;
};

#endif // ENDWINDOW_H
