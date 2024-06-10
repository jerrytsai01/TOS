#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <vector>
#include <QString>
#include "stone.h"
#include "hpbar.h"
#include "cdbar.h"
#include "slime.h"
#include "setwindow.h"

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
    void addEnemy();
    void fallanimation(int y,int x,int ty);
    void addCharacters(vector<int>& cha);
    void checkmatrix();
    int HP = 2000;
    static bool moveTime;
    int allpoints[6];
    int iffall;
    //wu{
    int damage = 0;
    void calDamage();
    bool RESTform = true, ATKform = false, DEFform = false;
    //}wu
    void weatherStone();

signals:
    void toerase();
    void tofall();
    void updateHP(int HP);
    void over();
public slots:
    void onCharactersSelected(const std::vector<int> &characters);
    void calHP();
    void handleSATKChanged(int sATK);
    void handleCATKChanged(int cATK);
    void handleBATKChanged(int bATK);

private slots:
    void erasestone();
    void handleStoneMove(QPointF newGirdPos, QPointF oldGridPos);
    void fall();
    void CDoverEvent();
    void calATKandHEAL();
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
    vector<int> enemy;                          //slime:1 baby:2 boss:3
    int erasestonenum[7];                    //各屬性符石消除數量 1~6
    int combo;
    QString combotext;
    QGraphicsTextItem *showcombo;
    vector<int> characters;                  //角色
    int a=90;
    CDbar *cdbar;
    HPbar *hpbar;
    QGraphicsPixmapItem *cdIcon;
    int gamephase = 2;
    int del;
    int sATK, cATK, bATK;
    QPushButton *setBtn;
    setwindow *s;
};
#endif // MAINWINDOW_H
