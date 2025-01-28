#ifndef QQMUSIC_H
#define QQMUSIC_H

#include <QWidget>
#include <volumetool.h>
#include "musiclist.h"
#include "lrcpage.h"
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <commonpage.h>
#include <QPropertyAnimation>
#include <QSqlDatabase>
#include <QSqlQuery>


QT_BEGIN_NAMESPACE
namespace Ui { class QQMusic; }
QT_END_NAMESPACE

class QQMusic : public QWidget
{
    Q_OBJECT

public:
    QQMusic(QWidget *parent = nullptr);
    ~QQMusic();
    // 初始化UI
    void initui();

    // 初始化数据库
    void initSqlite();

    // 初始化数据库歌曲列表
    void initMusicList();

    // 初始化媒体播放控制
    void initPlayer();

    // 推荐文本 + 推荐图片路径
    QJsonArray randomPiction();

    // 连接信号槽
    void connectSignalAndSlots();

    // 更新BTForm旁跳动音符动画
    void updateBtformAnimal();

    // 程序退出逻辑
    void onQQMusicQuit();
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

private slots:

    void on_quit_clicked();

    void onBtFormClick(int pageId);

    void on_volume_clicked();

    void on_addLocal_clicked();
    // 处理commonpage发送的信号,更新isLike列表音乐
    void updataLikeMusicAndPage(bool isLike, const QString& musicId);
    // 播放控制槽函数
    void onPlayMusic();
    // 响应播放/暂停状态改变的事件
    void onPlayerStateChanged();
    // 播放上一首事件
    void onPlayUpClicked();
    // 播放下一首事件
    void onPlayDownClicked();
    // 播放模式设置
    void onPlaybackModeClicked();
    // 播放模式发生改变事件
    void onPlaybackModeChanged(QMediaPlaylist::PlaybackMode playbackMode);
    // 播放全部按钮事件
    void onPlayAll(PageType pageType);
    // 播放全部事件
    void playAllMusicOfCommonPage(CommonPage* page, int index);
    // 双击播放事件
    void playMusicByIndex(CommonPage* page, int index);
    // 当前正在播放歌曲的ID
    void onCurrentIndexChanged(int index);
    // 处理VolumeTool静音事件
    void setPlayerMuted(bool isMuted);

    // 设置音量大小
    void setPlayerVolume(int volume);
    // 设置媒体切换的歌曲时长
    void onDurationChanged(qint64 duration);
    // 设置媒体时长的变化
    void onPositionChangde(qint64 position);
    // 拖动进度条改变歌曲播放进度
    void onMusicSliderChanged(float ration);
    // 媒体源发生改变时
    void onMetaDataAvailableChanged(bool available);

    // 歌词弹窗事件
    void onLrcWordClicked();

    void on_skin_clicked();

    void on_min_clicked();

private:
    Ui::QQMusic *ui;

    QPoint dragPosition;

    // SQL对象
    QSqlDatabase sqlite;
    // musiclist用来管理歌曲
    MusicList musiclist;
    // 音量
    VolumeTool* volumeTool;
    // 播放控制
    QMediaPlayer* player;
    // 播放模式设置
    QMediaPlaylist* playList;
    // 当前正在播放的Commonpage页面
    CommonPage* currentPage;
    // 当前歌曲总时间
    qint64 totalTime;
    // 当前正在播放的媒体源
    int currentIndex;
    // 歌词弹窗按钮
    LrcPage* lrcPage;
    // 歌词动画属性
    QPropertyAnimation* lrcPageAnimal;
    // 当前鼠标是否在非按钮下点击拖拽功能
    bool isDrag;
};
#endif // QQMUSIC_H
