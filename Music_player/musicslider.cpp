#include "musicslider.h"
#include "ui_musicslider.h"
#include <QMouseEvent>

MusicSlider::MusicSlider(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MusicSlider)
{
    ui->setupUi(this);
    maxWidch = this->width();
}

MusicSlider::~MusicSlider()
{
    delete ui;
}

void MusicSlider::setStep(float ratio)
{
    currentPos = maxWidch * ratio;
    moveSlider();
}

void MusicSlider::mousePressEvent(QMouseEvent *event)
{
    // QMouseEvent 返回的是该widget的坐标,可以直接调用
    currentPos = event->pos().x();
    moveSlider();
}

void MusicSlider::mouseMoveEvent(QMouseEvent *event)
{
    // 检测移动时鼠标是否在矩形内的限制
    QRect musicSliderRect = QRect(0, 0, geometry().width(), geometry().height());
    if(!musicSliderRect.contains(event->pos()))
    {
        return;
    }

    if(event->buttons() == Qt::LeftButton)
    {
        currentPos = event->pos().x();
        if(currentPos < 0)
        {
            currentPos = 0;
        }

        if(currentPos > maxWidch)
        {
            currentPos = maxWidch;
        }
        moveSlider();
        // emit setMusicSliderPositon(ui->outLine->width() / (float)maxWidch);
    }
}

void MusicSlider::mouseReleaseEvent(QMouseEvent *event)
{
    currentPos = event->pos().x();
    moveSlider();
    emit setMusicSliderPositon(ui->outLine->width() / (float)maxWidch);
}

void MusicSlider::moveSlider()
{
    ui->outLine->setGeometry(ui->outLine->x(), ui->outLine->y(), currentPos, ui->outLine->height());
}
