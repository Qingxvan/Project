#include "listitembox.h"
#include "ui_listitembox.h"
#include <QtDebug>

ListItemBox::ListItemBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListItemBox),
    isLike(false)
{
    ui->setupUi(this);
    // 关联槽函数 执行切换图片事件
    connect(ui->likeBtn, &QPushButton::clicked, this, &ListItemBox::onLikeClicked);
}

ListItemBox::~ListItemBox()
{
    delete ui;
}

void ListItemBox::enterEvent(QEvent *event)
{
    (void)event;
    setStyleSheet("background-color:rgb(234, 234, 234)");
}

void ListItemBox::leaveEvent(QEvent *event)
{
    (void)event;
    setStyleSheet("");
}

void ListItemBox::setMusicName(const QString &musicName)
{
    ui->musicNameLabel->setText(musicName);
}

void ListItemBox::setMusicSinger(const QString &musicSinger)
{
    ui->musicSingerLabel->setText(musicSinger);
}

void ListItemBox::setMusicAlbum(const QString &musicAlbum)
{
    ui->musicAlbumLabel->setText(musicAlbum);
}

void ListItemBox::setLikeMusic(bool isLike)
{
    this->isLike = isLike;
    if(isLike)
    {
        ui->likeBtn->setIcon(QIcon(":/images/like_2.png"));
    }
    else
    {
        ui->likeBtn->setIcon(QIcon(":images/like_3.png"));
    }
}
// 执行切换图片事件
void ListItemBox::onLikeClicked()
{
    isLike = !isLike;
    setLikeMusic(isLike);
    // 通过发射信号来通知修改Music对象中isLike熟悉
    emit setIsLike(isLike);
}

