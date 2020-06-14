#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QList>
#include<qpainter.h>
#include<vector>
#include<QTimer>
#include"towerposition.h"
#include"tower.h"
#include"waypoint.h"
#include"enemy.h"
#include"bullet.h"
using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void getHpDamage(int damage = 1);
    void removeEnemy(EnemyBase *enemy);
    void addBullet(BulletBase *bullet);
    void removedBullet(BulletBase *bullet);
    void removeTower(TowerBase*tower);
    void awardGold(int money);
    vector<EnemyBase*> getEnemies() const;
    vector<TowerBase*> getTowers() const;
private slots:
    void updateMap();
protected:
    void paintEvent(QPaintEvent *);
    bool canbuytower();
    void mousePressEvent(QMouseEvent *);

private:
    Ui::MainWindow *ui;
    vector <TowerPosition> Towerpositions;
    vector <TowerBase*> Towers;
    vector <WayPoint*> Waypoints;
    vector <EnemyBase*> Enemies;
    vector <BulletBase*> Bullets;
    void drawWave(QPainter *painter);
    void drawHP(QPainter *painter);
    void drawPlayerGold(QPainter *painter);
    bool gameWin;
    bool gameLost;
    int  wave;
    int  playerHp;
    int  gold;
    bool loadwave();
    void loadTowerPositions();//载入“地标”位置
    void loadWayPoints();//载入航点(“碰撞点”）
    void doGameOver();

};
#endif // MAINWINDOW_H
