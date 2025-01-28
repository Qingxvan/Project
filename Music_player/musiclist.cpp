#include "musiclist.h"
#include "music.h"
#include <QMimeDatabase>
#include <QDebug>
#include <QSqlQuery>

MusicList::MusicList()
{

}
// 将所有的音乐放到musicList中管理起来
void MusicList::addMusicsByUrl(const QList<QUrl> &musicUrls)
{
    for(auto e : musicUrls)
    {
        // 过滤重复的文件
        QString musicPath = e.toLocalFile();
        if(musicPaths.contains(musicPath))
        {
            continue;
        }
        musicPaths.insert(musicPath);
        // 判断文件是否是有效文件，通过检测歌曲文件的MIME类型来判断
        QMimeDatabase mimeDB;
        QMimeType mimeType = mimeDB.mimeTypeForFile(e.toLocalFile());
        QString mime = mimeType.name();

        // audio/mepg(mp3)	audio/flac(无损音质)	audio/wav
        if(mime == "audio/mpeg" || mime == "audio/flac" || mime == "audio/wav")
        {
            // 添加到musicList中
            Music music(e);
            musicList.push_back(music);
        }
    }
}

iterator MusicList::findMusicByMusicId(const QString &musicId)
{
    for(auto it = begin(); it != end(); ++it)
    {
        if(it->getMusicId() == musicId)
        {
            return it;
        }
    }
    return end();
}

iterator MusicList::begin()
{
    return musicList.begin();
}

iterator MusicList::end()
{
    return musicList.end();
}

void MusicList::writeToQQMusicDB()
{
    for(auto music : musicList)
    {
        music.InsertDB();
    }
}

void MusicList::readFromDB()
{
    QSqlQuery query;
    query.prepare("select musicId, musicName, musicSinger, albumName, musicUrl, duration, isLike, isHistory from MusicInfo");
    if(!query.exec())
    {
        qDebug() << "select error !!!";
        return;
    }
    while(query.next())
    {
        Music music;
        music.setMusicId(query.value(0).toString());
        music.setMusicName(query.value(1).toString());
        music.setMusicSinger(query.value(2).toString());
        music.setMusicAlbumn(query.value(3).toString());
        music.setMusicUrl("file:///" + query.value(4).toString());
        music.setMusicDuration(query.value(5).toLongLong());
        music.setIsLike(query.value(6).toBool());
        music.setIsHistory(query.value(7).toBool());
        musicList.push_back(music);
        // 将数据库中保存过的歌曲文件恢复至musicPaths中
        musicPaths.insert(music.getMusicUrl().toLocalFile());
    }
}
