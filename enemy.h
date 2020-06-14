#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>
#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <vector>
#include "bullet.h"
#include "target.h"
using namespace std;

class WayPoint;
class QPainter;
class MainWindow;
class Tower;
class TowerBase;
//公共基类
class EnemyBase : public QObject,public Target
{
     Q_OBJECT
public:
    EnemyBase(WayPoint *startWayPoint, MainWindow *game,double speed,const QPixmap &sprite,int maxhp=40);
    virtual void draw(QPainter *painter) const;
    ~EnemyBase();
    virtual void checkAttactingTower();
    void move();
    virtual void getDamage(int damage);
    void getRemoved();
    void getAttacked(TowerBase *attacker);
    void getLostSight(TowerBase *attacker);
    bool check(Target* target);
    QPoint pos()const;
    vector<TowerBase *> m_attackedTowers;
public slots:
    void doActivate();
protected:
    bool			m_active;
    int				m_maxHp;
    int				m_currentHp;
    qreal			m_walkingSpeed;
    qreal			m_rotationSprite;

    QPoint			m_pos;
    WayPoint *		m_destinationWayPoint;
    MainWindow *	m_game;
    const QPixmap	m_sprite;
};

class Enemy : public EnemyBase
{
    Q_OBJECT
public:
    Enemy(WayPoint *startWayPoint, MainWindow *game, const QPixmap &sprite = QPixmap("enemy.png"));
    ~Enemy();
    void draw(QPainter *painter) const;
    void checkAttactingTower() ;
private:
    static const QSize ms_fixedSize;//图片的大小
};

class shootingEnemy : public EnemyBase
{
    Q_OBJECT
public:
    shootingEnemy(WayPoint *startWayPoint, MainWindow *game, const QPixmap &sprite = QPixmap("shootingenemy.png"),
                  int maxhp=60);
    void checkAttactingTower();
    bool checkTarget();
    void draw(QPainter *painter) const;
protected:
    int shootingtimes;
    int maxshootingtimes;
    QTimer * m_fireRateTimer;
    int	     m_fireRate;
    TowerBase* attackingTarget;
    static const QSize ms_fixedSize;
public slots:
    void shootWeapon();
};

class destroyingEnemy : public EnemyBase
{
    Q_OBJECT
public:
    destroyingEnemy(WayPoint *startWayPoint, MainWindow *game, const QPixmap &sprite = QPixmap("destroyingenemy.png"));
    void draw(QPainter *painter) const ;
    void destroy();
    void getDamage(int damage) ;
protected:
    int attackingrange;
    static const QSize ms_fixedSize;
};



#endif // ENEMY_H
