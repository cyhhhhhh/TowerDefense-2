#ifndef TOWER_H
#define TOWER_H

#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <QObject>
#include <vector>
#include "enemy.h"
#include "bullet.h"
#include "target.h"
class QPainter;
class Enemy;
class EnemyBase;
class MainWindow;
class QTimer;
class TowerBase:public QObject,public Target
{
    Q_OBJECT
public:
    TowerBase(QPoint pos, MainWindow *game,const QPixmap &sprite,int co);
    virtual void draw(QPainter *painter) const;
    void checkEnemyInRange();
    void targetKilled();
    void attackEnemy();
    bool check(Target*target) ;
    void chooseEnemyForAttack(EnemyBase* enemy);
    QPoint getpos();
    void lostSightOfEnemy();
    int  knowcost();
    void getDamage(int damage);
    void getremoved();
    void beAttacked(EnemyBase* enemy);
protected slots:
    virtual void    shootWeapon();
protected:
    bool			m_attacking;
    int				m_attackRange;//���������Թ��������˵ľ���
    int				m_damage;//����������ʱ��ɵ��˺�
    int				m_fireRate;//�����ٴι������˵�ʱ����
    int             cost;//�����������Ľ��
    int             hitedtimes;
    qreal			m_rotationSprite;

    EnemyBase*      m_chooseEnemy;
    MainWindow *	m_game;
    QTimer *		m_fireRateTimer;

    const QPoint	m_pos;
    const QPixmap	m_sprite;
    QList<EnemyBase*> attackingEnemy;
};

class Tower :public  TowerBase
{
    Q_OBJECT
public:
    Tower(QPoint pos, MainWindow *game, const QPixmap &sprite = QPixmap("tower.png"));
    //~Tower();
    Tower();
    void draw(QPainter *painter) const;
    static const QSize ms_fixedSize;
protected slots:
    void shootWeapon();
};


#endif // TOWER_H
