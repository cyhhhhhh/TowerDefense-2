#include "enemy.h"
#include "waypoint.h"
#include "tower.h"
#include "func_test_coll.h"
#include "mainwindow.h"
//#include "audioplayer.h"
#include <QPainter>
#include <QColor>
#include <QDebug>
#include <QMatrix>
#include <QVector2D>
#include <QtMath>
#include <QPropertyAnimation>
static const int Health_Bar_Width = 50;

void EnemyBase::draw(QPainter *painter) const
{}
EnemyBase::EnemyBase(WayPoint *startWayPoint, MainWindow *game,double speed,const QPixmap &sprite,int maxhp)
    :QObject(0)
    ,m_active(false)
    ,m_maxHp(maxhp)
    ,m_currentHp(maxhp)
    ,m_walkingSpeed(speed)
    ,m_rotationSprite(0.0)
    ,m_pos(startWayPoint->pos())
    ,m_destinationWayPoint(startWayPoint->nextWayPoint())
    ,m_game(game)
    ,m_sprite(sprite)

{

}
EnemyBase::~EnemyBase()
{
     m_attackedTowers.clear();
     m_destinationWayPoint = NULL;
     m_game = NULL;
}
void EnemyBase::move()
{
    if (!m_active)
        return;

    if (collisionWithCircle(m_pos, 1, m_destinationWayPoint->pos(), 1))
    {
        // ���˵ִ���һ������
        if (m_destinationWayPoint->nextWayPoint())
        {
            // ������һ������
            m_pos = m_destinationWayPoint->pos();
            m_destinationWayPoint = m_destinationWayPoint->nextWayPoint();
        }
        else
        {
            // ��ʾ�������
            m_game->getHpDamage();
            m_game->removeEnemy(this);
            return;
        }
    }

    // ����ǰ�������·��
    // Ŀ�꺽�������
    QPoint targetPoint = m_destinationWayPoint->pos();
    // δ���޸�������������ƶ�״̬,�ӿ�,����,m_walkingSpeed�ǻ�׼ֵ

    // ������׼��
    qreal movementSpeed = m_walkingSpeed;
    QVector2D normalized(targetPoint - m_pos);
    normalized.normalize();//����ǰλ����Ŀ��λ��Ĳ������������������׼��Ϊ��λ����
    m_pos = m_pos + normalized.toPoint() * movementSpeed;

    // ȷ������ѡ����
    // Ĭ��ͼƬ����,��Ҫ����180��ת��
    m_rotationSprite = qRadiansToDegrees(qAtan2(normalized.y(), normalized.x())) + 180;
}
void EnemyBase::getDamage(int damage)
{
    m_currentHp -= damage;

    // ����,��Ҫ�Ƴ�
    if (m_currentHp <= 0)
    {
        m_game->awardGold(200);
        getRemoved();
    }
}
void EnemyBase::getRemoved()
{
    if (m_attackedTowers.empty())
        return;

    foreach (TowerBase *attacker, m_attackedTowers)
        attacker->targetKilled();

    m_game->removeEnemy(this);
}
bool EnemyBase::check(Target *target)
{
    vector<EnemyBase*> Enemylist=m_game->getEnemies();
    for(vector<EnemyBase*>::iterator iter=Enemylist.begin();iter!=Enemylist.end();iter++)
    {
        if(*iter==target)
            return true;//�õ����Դ���
    }
    return false;
}
void EnemyBase::getAttacked(TowerBase *attacker)
{
    m_attackedTowers.push_back(attacker);
}
void EnemyBase::getLostSight(TowerBase *attacker)
{
    for(vector<TowerBase*>::iterator iter=m_attackedTowers.begin();iter!=m_attackedTowers.end();iter++)
    {
        if(*iter==attacker)
        {
            m_attackedTowers.erase(iter);
            break;
        }
    }
}
QPoint EnemyBase::pos() const
{
    return m_pos;
}
void EnemyBase::doActivate()
{
    m_active = true;
}
void EnemyBase::checkAttactingTower()
{return;}
const QSize Enemy::ms_fixedSize(85, 76);

Enemy::Enemy(WayPoint *startWayPoint, MainWindow *game, const QPixmap &sprite)
    : EnemyBase(startWayPoint,game,1.0,sprite)
{
}
Enemy::~Enemy()
{
    m_attackedTowers.clear();
    m_destinationWayPoint = NULL;
    m_game = NULL;
}

void Enemy::checkAttactingTower()
{return;}

void Enemy::draw(QPainter *painter) const
{
    if (!m_active)
        return;

    painter->save();

    QPoint healthBarPoint = m_pos + QPoint(-Health_Bar_Width / 2 - 5, -ms_fixedSize.height() / 2+5);//����λ������Ϸ�
    // ����Ѫ��
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::red);
    QRect healthBarBackRect(healthBarPoint, QSize(Health_Bar_Width, 2));
    painter->drawRect(healthBarBackRect);

    painter->setBrush(Qt::green);
    QRect healthBarRect(healthBarPoint, QSize((double)m_currentHp / m_maxHp * Health_Bar_Width, 2));
    painter->drawRect(healthBarRect);


    // ����ƫת����,������+ƫ��=����
    static const QPoint offsetPoint(-ms_fixedSize.width() / 2, -ms_fixedSize.height() / 2);
    painter->translate(m_pos);
    painter->rotate(m_rotationSprite);
    // ���Ƶ���
    painter->drawPixmap(offsetPoint, m_sprite);
    painter->restore();
}

const QSize shootingEnemy::ms_fixedSize(101, 76);

shootingEnemy::shootingEnemy(WayPoint *startWayPoint, MainWindow *game, const QPixmap &sprite,int maxhp)
    : EnemyBase(startWayPoint,game,1.0,sprite,maxhp)
    , shootingtimes(0)
    , maxshootingtimes(2)
    , m_fireRate(1000)
{
    m_fireRateTimer = new QTimer(this);
    connect(m_fireRateTimer, SIGNAL(timeout()), this, SLOT(shootWeapon()));
}
bool shootingEnemy::checkTarget()
{
    vector<TowerBase*> Towerlist=m_game->getTowers();
    for(vector<TowerBase*>::iterator iter=Towerlist.begin();iter!=Towerlist.end();iter++)
    {
        if(*iter==attackingTarget)
        {
            return true;
            break;
        }
    }
    return false;
}
void shootingEnemy::checkAttactingTower()
{
    if(m_attackedTowers.empty())
    {
        return;
    }
    else
    {
        attackingTarget=m_attackedTowers.front();
        m_fireRateTimer->start(m_fireRate);
        return;
    }
}

void shootingEnemy::shootWeapon()
{
    if(checkTarget())
    {
    Bullet1 *bullet1 = new Bullet1(m_pos, attackingTarget->getpos(), 0, attackingTarget, m_game);
    bullet1->move();
    m_game->addBullet(bullet1);
    }
    else m_fireRateTimer->stop();
}

void shootingEnemy::draw(QPainter *painter) const
{
    if (!m_active)
        return;

    painter->save();

    QPoint healthBarPoint = m_pos + QPoint(-Health_Bar_Width / 2 - 5, -ms_fixedSize.height() / 2+5);//����λ������Ϸ�
    // ����Ѫ��
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::red);
    QRect healthBarBackRect(healthBarPoint, QSize(Health_Bar_Width, 2));
    painter->drawRect(healthBarBackRect);

    painter->setBrush(Qt::green);
    QRect healthBarRect(healthBarPoint, QSize((double)m_currentHp / m_maxHp * Health_Bar_Width, 2));
    painter->drawRect(healthBarRect);


    // ����ƫת����,������+ƫ��=����
    static const QPoint offsetPoint(-ms_fixedSize.width() / 2, -ms_fixedSize.height() / 2);
    painter->translate(m_pos);
    painter->rotate(m_rotationSprite);
    // ���Ƶ���
    painter->drawPixmap(offsetPoint, m_sprite);
    painter->restore();
}

destroyingEnemy::destroyingEnemy(WayPoint *startWayPoint, MainWindow *game, const QPixmap &sprite)
    :EnemyBase(startWayPoint,game,1.0,sprite)
    ,attackingrange(80)
{}
const QSize destroyingEnemy::ms_fixedSize(101, 76);
void destroyingEnemy::draw(QPainter *painter) const
{
    if (!m_active)
        return;

    painter->save();

    painter->setPen(Qt::green);
    // ���ƹ�����Χ
    painter->drawEllipse(m_pos, attackingrange, attackingrange);

    QPoint healthBarPoint = m_pos + QPoint(-Health_Bar_Width / 2 - 5, -ms_fixedSize.height() / 2+5);//����λ������Ϸ�
    // ����Ѫ��
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::red);
    QRect healthBarBackRect(healthBarPoint, QSize(Health_Bar_Width, 2));
    painter->drawRect(healthBarBackRect);

    painter->setBrush(Qt::green);
    QRect healthBarRect(healthBarPoint, QSize((double)m_currentHp / m_maxHp * Health_Bar_Width, 2));
    painter->drawRect(healthBarRect);


    // ����ƫת����,������+ƫ��=����
    static const QPoint offsetPoint(-ms_fixedSize.width() / 2, -ms_fixedSize.height() / 2);
    painter->translate(m_pos);
    painter->rotate(m_rotationSprite);
    // ���Ƶ���
    painter->drawPixmap(offsetPoint, m_sprite);
    painter->restore();
}
void destroyingEnemy::destroy()
{
    vector<TowerBase*> Towerlist=m_game->getTowers();
    foreach(TowerBase* Tower,Towerlist)
    {
        if(collisionWithCircle(m_pos, attackingrange, Tower->getpos(), 1))
        {
            m_game->removeTower(Tower);
        }
    }
}
void destroyingEnemy::getDamage(int damage)
{
    m_currentHp -= damage;

    // ����,��Ҫ�Ƴ�
    if (m_currentHp <= 0)
    {
        m_game->awardGold(200);
        destroy();
        getRemoved();
    }
}