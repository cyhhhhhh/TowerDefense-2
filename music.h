#ifndef MUSIC_H
#define MUSIC_H
#include <QObject>

class QMediaPlayer;

enum SoundType
{
    TowerPlaceSound,		// 放塔时的声音
    LifeLoseSound,			// 基地费血时的声音
    LaserShootSound,		// 打中敌人时的生意
    EnemyDestorySound		// 消灭敌人的时的声音
};

class AudioPlayer : public QObject
{
public:
    explicit AudioPlayer(QObject *parent = 0);

    void startBGM();
    void playSound(SoundType soundType);

private:
    QMediaPlayer *m_backgroundMusic; // 只用来播放背景音乐
};


#endif // MUSIC_H
