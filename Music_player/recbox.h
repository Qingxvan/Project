#ifndef RECBOX_H
#define RECBOX_H

#include <QWidget>
#include <QJsonArray>

namespace Ui {
class RecBox;
}

class RecBox : public QWidget
{
    Q_OBJECT

public:
    explicit RecBox(QWidget *parent = nullptr);
    ~RecBox();

    void initRecBoxUi(QJsonArray data, int row);

private slots:
    void on_btUp_clicked();

    void on_btDown_clicked();

private:
    void createRecBoxItem();

private:
    Ui::RecBox *ui;
    // 一行四列
    int row;
    int col;

    int currentIndex;	// 标记当前第几组图片
    int count;			// 标记图片总数
    // 图片列表
    QJsonArray imageList;
};

#endif // RECBOX_H
