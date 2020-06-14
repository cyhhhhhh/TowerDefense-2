#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"towerposition.h"
#include<QMouseEvent>
#include<iostream>
using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , gameWin(false)
    , gameLost(false)
    , wave(0)
    , playerHp(5)
    , gold(1000)

{
    ui->setupUi(this);
    loadTowerPositions();
    loadWayPoints();
    loadwave();
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateMap()));
    timer->start(30);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::removeEnemy(EnemyBase *enemy)
{

    for(vector<EnemyBase*>::iterator iter=Enemies.begin();iter!=Enemies.end();iter++)
    {
        if(*iter==enemy)
        {
            Enemies.erase(iter);
            break;
        }
    }//利用迭代器删除Enemies中的指定对象
    delete enemy;//敌人被消灭，需要移除
    if (Enemies.empty())
    {
        wave++;
        if (!loadwave())
        {
            gameWin = true;
            // 游戏胜利转到游戏胜利场景
            // 这里暂时以打印处理
        }
    }
}

vector<EnemyBase *> MainWindow::getEnemies() const
{
    return Enemies;
}

void MainWindow::updateMap()
{
    foreach (EnemyBase *enemy, Enemies)
    {
        enemy->checkAttactingTower();
        enemy->move();
    }
    foreach (TowerBase *tower, Towers)
        tower->checkEnemyInRange();
    update();
}

void MainWindow::paintEvent(QPaintEvent *)
{
    if (gameLost|| gameWin)
    {
        QString text = gameLost ? "YOU LOST!!!" : "YOU WIN!!!";
        QPainter painter(this);
        painter.setPen(QPen(Qt::red));
        painter.drawText(rect(), Qt::AlignCenter, text);
        return;
    }
    QPainter painter(this);
    painter.drawPixmap(0, 0, QPixmap("bg.png"));
    painter.drawPixmap(656,409,QPixmap("base.png"));
    for(int i=0;i<(int)Towerpositions.size();i++)
    {
        Towerpositions[i].draw(&painter);
    }
    for(int i=0;i<(int)Towers.size();i++)
    {
        Towers[i]->draw(&painter);
    }
    for(int i=0;i<(int)Enemies.size();i++)
    {
        Enemies[i]->draw(&painter);
    }
    for(int i=0;i<(int)Bullets.size();i++)
    {
        Bullets[i]->draw(&painter);
    }
    drawWave(&painter);
    drawHP(&painter);
    drawPlayerGold(&painter);
}

bool MainWindow::canbuytower()
{
    if (gold >=300)
        return true;
    else return false;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint pressPos = event->pos();
    for(int i=0;i<(int)Towerpositions.size();i++)
    {
        if (canbuytower() && Towerpositions[i].containPoint(pressPos)
                          && !Towerpositions[i].hasTower())
        {
            gold -= 300;//减去购买所需费用
            Towerpositions[i].setHasTower();//该位置有塔了
            Tower* tower = new Tower(Towerpositions[i].centerPos(), this);
            Towers.push_back(tower);
            update();
            break;
        }
    }
}

bool MainWindow::loadwave()
{
    if (wave >= 30)
        return false;
    WayPoint *startWayPoint = Waypoints.front();
    int enemyStartInterval1[] = {1000,5000,5000,1000,1000,1000};
    int enemyStartInterval2[] = {1000,1000,1000,1000,1000,1000};
    int enemyStartInterval3[] = {100,500,600,1000,1000,1000};
    int enemyStartInterval4[] = {100,100,100,500,500,500};
    for (int i = 0; i < 6; ++i)
    {
        if(wave<5)
        {
             Enemy *enemy = new Enemy(startWayPoint, this);
             Enemies.push_back(enemy);
             QTimer::singleShot(enemyStartInterval1[i], enemy, SLOT(doActivate()));
        }
        else if(wave<10)
        {
            if(i==4||1)
            {
                shootingEnemy *enemy = new shootingEnemy(startWayPoint, this);
                Enemies.push_back(enemy);
                QTimer::singleShot(enemyStartInterval2[i], enemy, SLOT(doActivate()));
            }
            else
            {
                Enemy *enemy = new Enemy(startWayPoint, this);
                Enemies.push_back(enemy);
                QTimer::singleShot(enemyStartInterval2[i], enemy, SLOT(doActivate()));
            }
        }
        else
        {
            if(i==0)
            {
                shootingEnemy *enemy = new shootingEnemy(startWayPoint, this);
                Enemies.push_back(enemy);
                QTimer::singleShot(enemyStartInterval4[i], enemy, SLOT(doActivate()));
            }
            if(i==2||4)
            {
                destroyingEnemy  *enemy = new destroyingEnemy(startWayPoint, this);
                Enemies.push_back(enemy);
                QTimer::singleShot(enemyStartInterval4[i], enemy, SLOT(doActivate()));
            }
            else
            {
                Enemy *enemy = new Enemy(startWayPoint, this);
                Enemies.push_back(enemy);
                QTimer::singleShot(enemyStartInterval4[i], enemy, SLOT(doActivate()));
            }
        }
    }
    return true;
}


void MainWindow::loadTowerPositions()
{
    QPoint pos[]=
    {
        QPoint(166,166),
        QPoint(294,166),
        QPoint(437,166),
        QPoint(557,166),

        QPoint(200,280),
        QPoint(329,280),
        QPoint(465,280),
        QPoint(590,280),

        QPoint(180,405),
        QPoint(307,405),
        QPoint(443,405),
        QPoint(569,405)
    };
    int len=sizeof(pos)/sizeof(pos[0]);
    for(int i=0;i<len;i++)
    {
        Towerpositions.push_back(pos[i]);
    }
}

void MainWindow::loadWayPoints()
{
    WayPoint *wayPoint1 = new WayPoint(QPoint(16,243));
    Waypoints.push_back(wayPoint1);

    WayPoint *wayPoint2 = new WayPoint(QPoint(675,236));
    Waypoints.push_back(wayPoint2);
    wayPoint1->setNextWayPoint(wayPoint2);

    WayPoint *wayPoint3 = new WayPoint(QPoint(680,355));
    Waypoints.push_back(wayPoint3);
    wayPoint2->setNextWayPoint(wayPoint3);

    WayPoint *wayPoint4 = new WayPoint(QPoint(130,364));
    Waypoints.push_back(wayPoint4);
    wayPoint3->setNextWayPoint(wayPoint4);

    WayPoint *wayPoint5 = new WayPoint(QPoint(130,485));
    Waypoints.push_back(wayPoint5);
    wayPoint4->setNextWayPoint(wayPoint5);

    WayPoint *wayPoint6 = new WayPoint(QPoint(677,485));
    Waypoints.push_back(wayPoint6);
    wayPoint5->setNextWayPoint(wayPoint6);
}

void MainWindow::getHpDamage(int damage)
{
    playerHp -= damage;
    if (playerHp <= 0)
        doGameOver();
}

void MainWindow::doGameOver()
{
    if (!gameLost)
    {
        gameLost = true;
        // 此处应该切换场景到结束场景
        // 暂时以打印替代,见paintEvent处理
    }
}

void MainWindow::addBullet(BulletBase *bullet)
{

    Bullets.push_back(bullet);
}


void MainWindow::removedBullet(BulletBase *bullet)
{
    for(vector<BulletBase*>::iterator iter=Bullets.begin();iter!=Bullets.end();iter++)
    {
        if(*iter==bullet)
        {
            Bullets.erase(iter);
            break;
        }
    }//利用迭代器删除Bullets中的指定元素
    delete bullet;
}


void MainWindow::awardGold(int money)
{
    gold += money;//奖励金币
    update();
}

void MainWindow::removeTower(TowerBase *tower)
{
    for(vector<TowerBase*>::iterator iter=Towers.begin();iter!=Towers.end();iter++)
    {
        if(*iter==tower)
        {
            Towers.erase(iter);
            break;
        }
    }
    int i;
    for(i=0;i<(int)Towerpositions.size();i++)
    {
        if(tower->getpos()==Towerpositions[i].centerPos())
            break;
    }
    Towerpositions[i].setLoseTower();
    delete tower;
}

void MainWindow::drawWave(QPainter *painter)
{
    painter->setPen(QPen(Qt::red));
    painter->drawText(QRect(681, 120, 90, 28), QString("Wave: %1").arg(wave + 1));
}

void MainWindow::drawHP(QPainter *painter)
{
    painter->setPen(QPen(Qt::red));
    painter->drawText(QRect(31, 120, 92, 28), QString("Health: %1").arg(playerHp));
}

void MainWindow::drawPlayerGold(QPainter *painter)
{
    painter->setPen(QPen(Qt::red));
    painter->drawText(QRect(356, 120, 98, 28), QString("GOLD: %1").arg(gold));
}

vector<TowerBase*> MainWindow::getTowers()const
{
    return Towers;
}


