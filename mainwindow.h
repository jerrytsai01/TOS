#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <vector>
#include <QString>
#include "stone.h"

using namespace std;
/*QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void addStone();
    void find(int inx,int iny,int del,int type);

private slots:
    void erasestone();

private:
    QGraphicsScene *scene = new QGraphicsScene();
    QGraphicsView *view = new QGraphicsView(scene);
    vector<vector<bool>> F;
    vector<vector<int>> stonepos;               //符石位置種類
    vector<vector<stone*>> savestone;           //符石位置調用
    vector<vector<int>> combine;                //三連第一顆符石位置
    vector<vector<int>> iffinded;              //有無查找過
    vector<vector<int>> waitdelete;             //消除順序
    vector<vector<int>> counterasestone;        //有三連符石
    int erasestonenum[6];                      //各屬性符石消除數量
    int combo;
    QString combotext;
    QGraphicsTextItem *showcombo;
};
#endif // MAINWINDOW_H
