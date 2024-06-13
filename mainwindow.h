#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <vector>
#include <QString>
#include "stone.h"
#include "characters.h"
#include "hpbar.h"
#include "cdbar.h"
#include "slime.h"
#include "babyhoneymon.h"
#include "cerberus.h"
#include "setwindow.h"
#include "endwindow.h"

using namespace std;
/*QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
*/
struct enemys{    // 一個敵人用一個struct，是哪種就用哪個來指，判斷是哪種的時候就看他的指針是不是空指針
    slime *s = nullptr;
    babyhoneymon *b = nullptr;
    cerberus *c = nullptr;
};

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
    void CharsATK();
    int HP = 2000;
    static bool moveTime;
    int allpoints[7];
    int iffall;
    int damage = 0;
    void calDamage();
    bool RESTform = true, ALIGNform = false, ATKform = false, DEFform = false;
    void weatherStone();
    bool resetENEhp = false;
    vector<int> EnemyHP = {};
    int ATKofChars[6] = {0, 0, 0, 0, 0, 0};
    static int  gamephase ;
signals:
    void toerase();
    void tofall();
    void updateHP(int HP);
    void over();
    void forceRel(QPointF);
    void ATKsign();
    void setshow();
    void lose();
public slots:
    void onCharactersSelected(const std::vector<int> &characters);
    void calHP();
    void handleSATKChanged(int sATK);
    void handleCATKChanged(int cATK);
    void handleBATKChanged(int bATK);
    void ATKevent();
    void reset();
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
    vector<enemys*> enemyObj;
    int erasestonenum[7];                    //各屬性符石消除數量 1~6
    int combo;
    QString combotext;
    QGraphicsTextItem *showcombo;
    QString showdamagetxt[14];
    QGraphicsTextItem *showdamage[14];
    QString showhptxt;
    QGraphicsTextItem *showhp;
    QString roundtxt;
    QGraphicsTextItem *showround[3];
    QPixmap monp[7];
    QGraphicsPixmapItem *monsterhp[7];
    vector<int> characters;                  //角色屬性
    vector<Characters*> charObj;
    int a=90;
    CDbar *cdbar;
    HPbar *hpbar;
    QGraphicsPixmapItem *cdIcon;
    int del;
    int sATK, cATK, bATK;
    QPushButton *setBtn;
    setwindow *s;
    endwindow *e;
    bool calculated = false;
    static int roundCount;    
    bool addWeatherStone = false;
    bool CDoverProcessing = false;
};
#endif // MAINWINDOW_H
