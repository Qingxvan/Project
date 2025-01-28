#include "recboxitem.h"
#include "ui_recboxitem.h"
#include <QPropertyAnimation>
#include <QDebug>

RecBoxItem::RecBoxItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RecBoxItem)
{
    ui->setupUi(this);

    // 事件过滤器，监听鼠标进入事件，当鼠标进入时启动动画
    ui->musicImageBox->installEventFilter(this);
}

RecBoxItem::~RecBoxItem()
{
    delete ui;
}

void RecBoxItem::setRecText(const QString &text)
{
    ui->recBoxItemText->setText(text);
}

void RecBoxItem::setRecImage(const QString &imagePath)
{
    QString style = "background-image:url(" + imagePath +");";
    ui->recMusicImage->setStyleSheet(style);
}

// 鼠标进入时控件发生向上移动效果
bool RecBoxItem::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->musicImageBox)
    {
        if(QEvent::Enter == event->type())
        {
            // 上移动画
            QPropertyAnimation* animation = new QPropertyAnimation(ui->musicImageBox, "geometry");
            animation->setDuration(200);
            animation->setStartValue(QRect(9, 9, ui->musicImageBox->width(), ui->musicImageBox->height()));
            animation->setEndValue(QRect(9, 0, ui->musicImageBox->width(), ui->musicImageBox->height()));
            animation->start();
            connect(animation, &QPropertyAnimation::finished, this, [=](){
                delete animation;
            });
        }
        else if(QEvent::Leave == event->type())
        {
            // 下移动画
            QPropertyAnimation* animation = new QPropertyAnimation(ui->musicImageBox, "geometry");
            animation->setDuration(100);
            animation->setStartValue(QRect(9, 0, ui->musicImageBox->width(), ui->musicImageBox->height()));
            animation->setEndValue(QRect(9, 9, ui->musicImageBox->width(), ui->musicImageBox->height()));
            animation->start();
            connect(animation, &QPropertyAnimation::finished, this, [=](){
                delete animation;
            });
        }
        return true;
    }

    return QObject::eventFilter(watched, event);
}
