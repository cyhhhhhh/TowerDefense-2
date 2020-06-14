#ifndef TOWERPOSITION_H
#define TOWERPOSITION_H

#include <QPoint>
#include <QSize>
#include <QPixmap>

class QPainter;

class TowerPosition
{
public:
    TowerPosition(QPoint pos, const QPixmap &sprite = QPixmap("spot.png"));

    void setHasTower(bool hasTower = true);//放置塔，将m_hasTower变为true
    bool hasTower() const;//该位置是否有塔
    void setLoseTower(bool hasTower=false);//塔被摧毁，将m_hasTower变为false
    QPoint centerPos() const;//centerPos表示“塔”实际安放的位置，会与“地标”位置有点小小的偏差
    bool containPoint(const QPoint &pos) const;

    void draw(QPainter *painter) const;

private:
    bool		m_hasTower;
    QPoint		m_pos;
    QPixmap		m_sprite;

    static const QSize ms_fixedSize;
};

#endif // TOWERPOSITION_H
