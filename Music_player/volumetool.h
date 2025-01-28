#ifndef VOLUMETOOL_H
#define VOLUMETOOL_H

#include <QWidget>

namespace Ui {
class VolumeTool;
}

class VolumeTool : public QWidget
{
    Q_OBJECT

public:
    explicit VolumeTool(QWidget *parent = nullptr);
    ~VolumeTool();

    void paintEvent(QPaintEvent *event);
    // 触发静音信号
    void onSilenceBtnClicked();
    // 鼠标调节音量事件
    bool eventFilter(QObject *watched, QEvent *event);
    // 计算音量大小
    void calcVolume();
signals:
    // 向上层发送静音信号
    void setMusicMuted(bool);
    // 设置音量大小
    void setMusicVolume(int volume);
private:
    Ui::VolumeTool *ui;
    // 静音状态
    bool isMuted;
    // 音量大小
    int volumeRatio;
};

#endif // VOLUMETOOL_H
