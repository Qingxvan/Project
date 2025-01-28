#include "music.h"
#include <QUuid>
#include <QMediaPlayer>
#include <QDebug>
#include <QCoreApplication>
#include <QSqlQuery>
#include <QSqlError>

Music::Music()
    : isLike(false)
    , isHistory(false)
{

}

Music::Music(QUrl url)
    : isLike(false)
    , isHistory(false)
    , musicUrl(url)
{
    // 读取Url对应的歌曲信息，解析元数据
    // 歌曲名称，歌曲专辑，歌手，歌曲时长
    // 设置UUID
    musicId = QUuid::createUuid().toString();
    parseMediaMetaMusic();
}

void Music::setMusicId(const QString &musicId)
{
    this->musicId = musicId;
}

void Music::setMusicName(const QString& musicName)
{
    this->musicName = musicName;
}
void Music::setMusicSinger(const QString& musicSinger)
{
    this->musicSinger = musicSinger;
}
void Music::setMusicAlbumn(const QString& musicAlbumn)
{
    this->musicAlbumn = musicAlbumn;
}
void Music::setMusicDuration(qint64 duration)
{
    this->duration = duration;
}
void Music::setIsLike(bool isLike)
{
    this->isLike = isLike;
}
void Music::setIsHistory(bool isHistory)
{
    this->isHistory = isHistory;
}
void Music::setMusicUrl(QUrl musicUrl)
{
    this->musicUrl = musicUrl;
}

QString Music::getMusicName()const
{
    return musicName;
}
QString Music::getMusicSinger()const
{
    return musicSinger;
}
QString Music::getMusicAlbumn()const
{
    return musicAlbumn;
}
qint64 Music::getMusicDuration()const
{
    return duration;
}
bool Music::getIsLike()const
{
    return isLike;
}
bool Music::getIsHistory()const
{
    return isHistory;
}
QUrl Music::getMusicUrl()const
{
    return musicUrl;
}

QString Music::getMusicId() const
{
    return musicId;
}

QString Music::getLrcFilePath() const
{
    QString LrcPath = musicUrl.toLocalFile();
    LrcPath.replace(".mp3", ".lrc");

    return LrcPath;
}

void Music::InsertDB()
{
    QSqlQuery query;
    query.prepare("select exists (select 1 from MusicInfo where musicId=?)");
    query.addBindValue(musicId);
    if(!query.exec())
    {
        qDebug() << "select error !!!" << query.lastError().text();
        return;
    }

    if(query.next())
    {
        bool isExists = query.value(0).toBool();
        if(isExists)
        {
            // 存在不进行插入数据, 将isLike & isHistory更新
            query.prepare("update MusicInfo set isLike = ?, isHistory = ? where musicId = ?");
            query.addBindValue(isLike? 1 : 0);
            query.addBindValue(isHistory? 1 : 0);
            query.addBindValue(musicId);
            if(!query.exec())
            {
                qDebug() << "update error !!!" << query.lastError().text();
                return;
            }
            qDebug() << "update success " << musicName << " " << musicId;
        }
        else
        {
            // 不存在后续进行插入数据
            query.prepare("insert into MusicInfo (musicId, musicName, musicSinger, albumname, musicUrl, duration,\
                            isLike, isHistory) \
                            values (?, ?, ?, ?, ?, ?, ?, ?)");
            query.addBindValue(musicId);
            query.addBindValue(musicName);
            query.addBindValue(musicSinger);
            query.addBindValue(musicAlbumn);
            query.addBindValue(musicUrl.toLocalFile());
            query.addBindValue(duration);
            query.addBindValue(isLike ? 1 : 0);
            query.addBindValue(isHistory ? 1 : 0);
            if(!query.exec())
            {
                qDebug() << "insert error !!!" << query.lastError().text();
                return;
            }
            qDebug() << "insert success " << musicName;
        }
    }
}

void Music::parseMediaMetaMusic()
{
    // 创建对象
    QMediaPlayer player;
    // 设置媒体源，解析元数据
    player.setMedia(musicUrl);
    // setMedia不会等待歌曲加载完毕，会立即返回. mediaMeta获取元数据，前提要保证媒体已经添加完成
    // isMetaDataAvailable检测数据是否有效
    while(!player.isMetaDataAvailable())
    {
        // false 持续检测
        QCoreApplication::processEvents();
    }
    // 获取元数据
    if(player.isMetaDataAvailable())
    {
        musicName = player.metaData("Title").toString();
        musicSinger = player.metaData("Author").toString();
        musicAlbumn = player.metaData("AlbumTitle").toString();
        duration = player.metaData("Duration").toLongLong();

        // music为空时处理
        QString fileName = musicUrl.fileName();
        int index = fileName.indexOf('-');
        if(musicName.isEmpty())
        {
            if(index != -1)
            {
                musicName = fileName.mid(0, index);
            }
            else
            {
                musicName = fileName.mid(0, fileName.indexOf('.'));
            }
        }
        // Singer为空
        if(musicSinger.isEmpty())
        {
            if(index != -1)
            {
                musicSinger = fileName.mid(index + 1, fileName.indexOf('.') - index - 1);
            }
            else
            {
                musicSinger = "未知歌手";
            }
        }
        if(musicAlbumn.isEmpty())
        {
            musicAlbumn = "未知专辑";
        }

        qDebug() << musicName << ":" << musicSinger << ":" << musicAlbumn << ":" << duration;
    }
}










