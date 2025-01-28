#include "btform.h"
#include "ui_btform.h"

BtForm::BtForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BtForm)
{
    ui->setupUi(this);

    ui->lineBox->hide(); // 隐藏linebox的动画效果
    // line1设置动画效果
    line1Animal = new QPropertyAnimation(ui->line1, "geometry", this);
    line1Animal->setDuration(1800);
    line1Animal->setKeyValueAt(0, QRect(0, 15, 2, 0));
    line1Animal->setKeyValueAt(0.5, QRect(0, 0, 2, 15));
    line1Animal->setKeyValueAt(1, QRect(0, 15, 2, 0));
    line1Animal->setLoopCount(-1); // 设置动画循环次数
    line1Animal->start(); // 启动动画效果

    // line2设置动画效果
    line2Animal = new QPropertyAnimation(ui->line2, "geometry", this);
    line2Animal->setDuration(1700);
    line2Animal->setKeyValueAt(0, QRect(6, 15, 2, 0));
    line2Animal->setKeyValueAt(0.5, QRect(6, 0, 2, 15));
    line2Animal->setKeyValueAt(1, QRect(6, 15, 2, 0));
    line2Animal->setLoopCount(-1); // 设置动画循环次数
    line2Animal->start();

    // line3设置动画效果
    line3Animal = new QPropertyAnimation(ui->line3, "geometry", this);
    line3Animal->setDuration(1900);
    line3Animal->setKeyValueAt(0, QRect(12, 15, 2, 0));
    line3Animal->setKeyValueAt(0.5, QRect(12, 0, 2, 15));
    line3Animal->setKeyValueAt(1, QRect(12, 15, 2, 0));
    line3Animal->setLoopCount(-1); // 设置动画循环次数
    line3Animal->start();

    // line4设置动画效果
    line4Animal = new QPropertyAnimation(ui->line4, "geometry", this);
    line4Animal->setDuration(2000);
    line4Animal->setKeyValueAt(0, QRect(18, 15, 2, 0));
    line4Animal->setKeyValueAt(0.5, QRect(18, 0, 2, 15));
    line4Animal->setKeyValueAt(1, QRect(18, 15, 2, 0));
    line4Animal->setLoopCount(-1); // 设置动画循环次数
    line4Animal->start();
}

BtForm::~BtForm()
{
    delete ui;
}

// LeftBox 中设置按钮的图标 & 文本信息. 将LeftBox中的按钮和Page页面关联
void BtForm::setIconAndText(const QString &btIcon, const QString &btText, int pageId)
{
    ui->btIcon->setPixmap(QPixmap(btIcon));

    ui->btText->setText(btText);

    this->pageId = pageId;
}

int BtForm::getPageId() const
{
    return pageId;
}
// 清除btStyle的当前颜色
void BtForm::clearBackground()
{
    ui->btStyle->setStyleSheet("#btStyle:hover{background-color:#E4E4E4; border-radius:7px;} QLabel{background-color:transparent;}");
}

// 提供一个公有函数，调用该函数显示动画
void BtForm::showAnimal(bool isShow)
{
    if(isShow)
    {
        ui->lineBox->show();
    }
    else
    {
        ui->lineBox->hide();
    }
}

// LeftBox 设置按钮按下后改变颜色. 再切换bodyRight中的Page页面
void BtForm::mousePressEvent(QMouseEvent *event)
{
    (void) event;
    ui->btStyle->setStyleSheet("#btStyle{background-color:rgb(221,221,221); border-radius:7px;} QLabel{background-color:transparent;}");

    emit btClick(pageId);
}
