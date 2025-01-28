#include "commonpage.h"
#include "ui_commonpage.h"
#include "listitembox.h"
#include <QtDebug>

CommonPage::CommonPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommonPage)
{
    ui->setupUi(this);
    // 关闭水平滚动条
    ui->PageMusicList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 播放全部歌曲事件
    connect(ui->playAllBtn, &QPushButton::clicked, this, [=](){
        // 触发播放全部事件时，通知QQMusic获取当前PageType中page页面所以歌曲
        emit playAll(pageType);
    });
    // 鼠标双击page中this页面歌曲，通知QQMusic播放事件
    connect(ui->PageMusicList, &QListWidget::doubleClicked, this, [=](const QModelIndex &index){
        emit playMusicByIndex(this, index.row());
    });
}

CommonPage::~CommonPage()
{
    delete ui;
}

void CommonPage::setMusicListType(PageType pageType)
{
    this->pageType = pageType;
}


void CommonPage::reFresh(MusicList &musicList)
{
    // 添加前需要清空列表内已存在的元素
    ui->PageMusicList->clear();
    // 添加歌曲
    addMusicToMusicPage(musicList);
    for(auto musicId : musicOfPage)
    {
        auto it = musicList.findMusicByMusicId(musicId);
        // 没有找到
        if(it == musicList.end())
            continue;
        // 将ListBoxItem对象放到pageMusicList中
        ListItemBox* listItemBox = new ListItemBox(this);
        // 歌曲名称，作者，专辑更新到界面
        listItemBox->setMusicName(it->getMusicName());
        listItemBox->setMusicSinger(it->getMusicSinger());
        listItemBox->setMusicAlbum(it->getMusicAlbumn());
        listItemBox->setLikeMusic(it->getIsLike());

        QListWidgetItem* item = new QListWidgetItem(ui->PageMusicList);
        item->setSizeHint(QSize(listItemBox->width(), listItemBox->height()));
        ui->PageMusicList->setItemWidget(item, listItemBox);

        // 拦截收藏时触发的信号
        connect(listItemBox, &ListItemBox::setIsLike, this, [=](bool isLike){
            // 更新歌曲库状态,通知qqmusic,让其中likePage,localPage,recentPage更新歌曲库中信息
            // 给qqmusic发送信号
            emit updataLikeMusic(isLike, it->getMusicId());
        });
    }
    // 窗口重绘paintEvent
    repaint(); // 立马响应
}

void CommonPage::setCommonPageUi(const QString &text, const QString &imagePath)
{
    ui->PageTittle->setText(text);

    ui->musicImageLabel->setPixmap(QPixmap(imagePath));
    ui->musicImageLabel->setScaledContents(true); // 根据尺寸图片自动填充拉伸

}

void CommonPage::addMusicToMusicPage(MusicList &musicList)
{
    // 添加前需要清空列表内已存在的元素
    musicOfPage.clear();
    for(auto music : musicList)
    {
        switch (pageType)
        {
        case LIKE_PAGE:
            if(music.getIsLike())
            {
                musicOfPage.push_back(music.getMusicId());
            }
            break;
        case LOCAL_PAGE:
            musicOfPage.push_back(music.getMusicId());
            break;
        case HISTORY_PAGE:
            if(music.getIsHistory())
            {
                musicOfPage.push_back(music.getMusicId());
            }
            break;
        default:
            qDebug() << "暂未支持";
        }
    }
}

void CommonPage::addMusicToPlaylist(MusicList &musicList, QMediaPlaylist *playList)
{
    for(auto music : musicList)
    {
        switch (pageType) {
        case LIKE_PAGE:
            if(music.getIsLike())
            {
                playList->addMedia(music.getMusicUrl());
            }
            break;
        case LOCAL_PAGE:
            playList->addMedia(music.getMusicUrl());
            break;
        case HISTORY_PAGE:
            if(music.getIsHistory())
            {
                playList->addMedia(music.getMusicUrl());
            }
            break;
        default:
            qDebug() << "暂未支持";
            break;
        }
    }
}

QString CommonPage::getMusicIdByIndex(int index)
{
    if(index < musicOfPage.size())
    {
        return musicOfPage[index];
    }
    else
    {
        qDebug() << "无此歌曲";
        return "";
    }

}

void CommonPage::setMusicImage(QPixmap Pixmap)
{
    ui->musicImageLabel->setPixmap(Pixmap);
    ui->musicImageLabel->setScaledContents(true);
}









