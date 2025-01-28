#ifndef COMMONPAGE_H
#define COMMONPAGE_H

#include <QWidget>
#include "musiclist.h"
#include <QMediaPlaylist>

namespace Ui {
class CommonPage;
}

enum PageType
{
    LIKE_PAGE,		// 喜欢
    LOCAL_PAGE,		// 本地和下载
    HISTORY_PAGE	// 最近播放
};

class CommonPage : public QWidget
{
    Q_OBJECT

public:

    explicit CommonPage(QWidget *parent = nullptr);
    ~CommonPage();

    void setCommonPageUi(const QString& text, const QString& imagePath);

    void setMusicListType(PageType pageType);
    // 添加
    void addMusicToMusicPage(MusicList& musicList);
    // 将歌曲更新到界面
    void reFresh(MusicList& musicList);
    // 将歌曲添加到播放列表中
    void addMusicToPlaylist(MusicList& musicList, QMediaPlaylist* playList);
    // 通过索引获取歌曲下标
    QString getMusicIdByIndex(int index);
    // 设置musicimage图片
    void setMusicImage(QPixmap Pixmap);
signals:
    // 通知qqmusic处理拦截信号
    void updataLikeMusic(bool isLike, const QString& musicId);
    // 处理播放全部列表信号
    void playAll(PageType);
    // 双击page页面歌曲信号
    void playMusicByIndex(CommonPage*, int);

private:
    Ui::CommonPage *ui;

    PageType pageType; // 页面类型
    QVector<QString> musicOfPage; // PageType对应页面的歌曲ID
};

#endif // COMMONPAGE_H
