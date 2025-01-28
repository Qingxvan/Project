#ifndef MUSICLIST_H
#define MUSICLIST_H

#include <QVector>
#include "music.h"
#include <QList>
#include <QUrl>
#include <QSet>

typedef QVector<Music>::iterator iterator;
// 管理所有添加的歌曲
class MusicList
{
public:
    MusicList();
    // 把对象添加到musicList中
    void addMusicsByUrl(const QList<QUrl>& musicUrls);
    // 查找对象Id
    iterator findMusicByMusicId(const QString& musicId);

    iterator begin();
    iterator end();

    // 将数据写入到数据库中
    void writeToQQMusicDB();
    // 读取数据库中信息
    void readFromDB();
private:
    QVector<Music> musicList;
    // set的歌曲文件
    QSet<QString> musicPaths;
};

#endif // MUSICLIST_H
