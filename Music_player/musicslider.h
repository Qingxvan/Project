#ifndef MUSICSLIDER_H
#define MUSICSLIDER_H

#include <QWidget>

namespace Ui {
class MusicSlider;
}

class MusicSlider : public QWidget
{
    Q_OBJECT

public:
    explicit MusicSlider(QWidget *parent = nullptr);
    ~MusicSlider();
    // 进度条增长
    void setStep(float ratio);

signals:
    // 拖动进度条改变当前歌曲播放进度
    void setMusicSliderPositon(float ratio);

protected:
    // 重写鼠标移动、点击、离开函数
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    // 移动进度条
    void moveSlider();
private:
    Ui::MusicSlider *ui;
    // 当前播放进度
    int currentPos;
    // outline总宽度
    int maxWidch;
};

#endif // MUSICSLIDER_H
