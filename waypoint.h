#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <QPoint>

class QPainter;

// 敌人移动的航线
class WayPoint
{
public:
    WayPoint(QPoint pos);

    void setNextWayPoint(WayPoint *nextPoint);
    WayPoint* nextWayPoint() const;
    QPoint pos() const;//返回该航点位置坐标

    void draw(QPainter *painter) const;

private:
    const QPoint		m_pos;
    WayPoint *			m_nextWayPoint;
};

#endif // WAYPOINT_H
