#ifndef LISTITEMBOX_H
#define LISTITEMBOX_H

#include <QWidget>

namespace Ui {
class ListItemBox;
}

class ListItemBox : public QWidget
{
    Q_OBJECT

public:
    explicit ListItemBox(QWidget *parent = nullptr);
    ~ListItemBox();

    // 重写鼠标进入事件
    void enterEvent(QEvent *event);
    // 重写鼠标离开事件
    void leaveEvent(QEvent *event);

    void setMusicName(const QString& musicName);
    void setMusicSinger(const QString& musicSinger);
    void setMusicAlbum(const QString& musicAlbum);
    // 添加喜欢
    void setLikeMusic(bool isLike);
    // 触发事件切换喜欢图标
    void onLikeClicked();

signals:
    void setIsLike(bool IsLike);
private:
    Ui::ListItemBox *ui;
    bool isLike;
};

#endif // LISTITEMBOX_H
