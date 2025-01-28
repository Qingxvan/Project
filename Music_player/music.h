#ifndef MUSIC_H
#define MUSIC_H
#include <QUrl>

class Music
{
public:
    Music();
    Music(QUrl url);

    void setMusicId(const QString& musicId);
    void setMusicName(const QString& musicName);
    void setMusicSinger(const QString& musicSinger);
    void setMusicAlbumn(const QString& musicAlbumn);
    void setMusicDuration(qint64 duration);
    void setIsLike(bool isLike);
    void setIsHistory(bool isHistory);
    void setMusicUrl(QUrl musicUrl);

    QString getMusicName()const;
    QString getMusicSinger()const;
    QString getMusicAlbumn()const;
    qint64 getMusicDuration()const;
    bool getIsLike()const;
    bool getIsHistory()const;
    QUrl getMusicUrl()const;
    QString getMusicId()const;
    QString getLrcFilePath()const;

    // 将歌曲写入到数据库
    void InsertDB();
private:
    void parseMediaMetaMusic();

private:
    // 确保唯一性设置一个UUID
    QString musicId;
    // 歌曲名称
    QString musicName;
    // 歌手
    QString musicSinger;
    // 歌曲专辑
    QString musicAlbumn;
    // 歌曲时长 -- long long类型
    qint64 duration;
    // 是否喜欢
    bool isLike;
    // 是否为历史播放
    bool isHistory;
    // 歌曲Url
    QUrl musicUrl;
};

#endif // MUSIC_H
