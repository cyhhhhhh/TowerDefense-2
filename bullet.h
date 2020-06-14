#ifndef BULLET_H
#define BULLET_H

#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <QObject>
#include "tower.h"
#include "enemy.h"
#include "target.h"
class QPainter;
class EnemyBase;
class TowerBase;
class MainWindow;


class BulletBase:public QObject
{
    Q_OBJECT
    Q_PROPERTY(QPoint m_currentPos READ currentPos WRITE setCurrentPos)
public:
    BulletBase(QPoint startPos, QPoint targetPoint, int damage, Target* target,
               MainWindow *game, const QPixmap &sprite);
    void draw(QPainter *painter) const;
    void move();
    void setCurrentPos(QPoint pos);
    QPoint currentPos() const;
    virtual bool checkTarget();
protected slots:
    void hitTarget();
protected:
    const QPoint	m_startPos;//�ӵ���ʼλ�ü���������λ��
    const QPoint	m_targetPos;//��ֹλ�ü�����λ��
    const QPixmap	m_sprite;
    QPoint			m_currentPos;//�ӵ���ǰλ��
    Target *		m_target;//Ŀ�����
    MainWindow *	m_game;
    int				m_damage;//���������ɹ���������
};

class Bullet : public BulletBase
{
public:
    Bullet(QPoint startPos, QPoint targetPoint, int damage, EnemyBase* target,
           MainWindow *game, const QPixmap &sprite= QPixmap("bullet.png"));
    bool checkTarget() ;
private:
    static const QSize ms_fixedSize;
};

class Bullet1 :public BulletBase
{
public:
    Bullet1(QPoint startPos, QPoint targetPoint, int damage, TowerBase* target,
            MainWindow *game, const QPixmap &sprite= QPixmap("bullet1.png"));
    bool checkTarget() ;
private:
    static const QSize ms_fixedSize;
};



#endif // BULLET_H
