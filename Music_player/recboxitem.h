#ifndef RECBOXITEM_H
#define RECBOXITEM_H

#include <QWidget>

namespace Ui {
class RecBoxItem;
}

class RecBoxItem : public QWidget
{
    Q_OBJECT

public:
    explicit RecBoxItem(QWidget *parent = nullptr);
    ~RecBoxItem();

    // 设置文本
    void setRecText(const QString& text);
    // 设置图片路径
    void setRecImage(const QString& imagePath);

    // 鼠标进入时控件发生向上移动效果
    bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::RecBoxItem *ui;
};

#endif // RECBOXITEM_H
