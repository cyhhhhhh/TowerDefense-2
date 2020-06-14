#include "towerposition.h"
#include <QPainter>

const QSize TowerPosition::ms_fixedSize(63,35);//设置图片分辨率（大小）

TowerPosition::TowerPosition(QPoint pos, const QPixmap &sprite)
    : m_hasTower(false)
    , m_pos(pos)
    , m_sprite(sprite)
{
}

void TowerPosition::setHasTower(bool hasTower/* = true*/)
{
    m_hasTower = hasTower;
}
void TowerPosition::setLoseTower(bool hasTower)
{
    m_hasTower=hasTower;
}

bool TowerPosition::hasTower() const
{
    return m_hasTower;
}

void TowerPosition::draw(QPainter *painter) const
{
    painter->drawPixmap(m_pos.x(), m_pos.y(), m_sprite);
}

QPoint TowerPosition::centerPos() const
{
    QPoint p(25,15);//尝试而得的偏移量
    return m_pos+p;
}

bool TowerPosition::containPoint(const QPoint &pos) const
{
    bool isXInHere = m_pos.x() < pos.x() && pos.x() < (m_pos.x() + ms_fixedSize.width());
    bool isYInHere = m_pos.y() < pos.y() && pos.y() < (m_pos.y() + ms_fixedSize.height());
    return isXInHere && isYInHere;
}
