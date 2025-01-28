#include "recbox.h"
#include "ui_recbox.h"

#include "recboxitem.h"
#include <QJsonObject>

RecBox::RecBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RecBox),
    row(1),
    col(4)
{
    ui->setupUi(this);
}

RecBox::~RecBox()
{
    delete ui;
}

void RecBox::initRecBoxUi(QJsonArray data, int row)
{
    if(row == 2)
    {
        this->row = row;
        col = 8;
    }
    else
    {
        ui->recListDown->hide(); // 隐藏第二行
    }

    imageList = data;

    currentIndex = 0;
    count = imageList.size() / col;

    // 在RecBox控件中添加RecBoxItem
    createRecBoxItem();
}

// 创建RecBoxItem对象，往RecBox中添加
void RecBox::createRecBoxItem()
{
    // 添加之前先删除RecBox中的元素
    QList<RecBoxItem*> recUpList = ui->recListUp->findChildren<RecBoxItem*>();
    for(auto e : recUpList)
    {
        ui->recListUpHLayout->removeWidget(e);
        delete e;
    }

    QList<RecBoxItem*> recDownList = ui->recListDown->findChildren<RecBoxItem*>();
    for(auto e : recDownList)
    {
        ui->recListDownHLayout->removeWidget(e);
        delete e;
    }

    int index = 0;
    for(int i = currentIndex * col; i < col + currentIndex * col; i++)
    {
        RecBoxItem* item = new RecBoxItem();
        // 设置背景图片以及对应的文本信息
        QJsonObject obj = imageList[i].toObject();
        item->setRecText(obj.value("text").toString());
        item->setRecImage(obj.value("path").toString());

        if(row == 2 && index++ >= col / 2)
        {
            ui->recListDownHLayout->addWidget(item);
        }
        else
        {
            ui->recListUpHLayout->addWidget(item);
        }
    }
}

void RecBox::on_btUp_clicked()
{
    currentIndex--;
    if(currentIndex < 0)
    {
        currentIndex = count - 1;
    }
    createRecBoxItem();
}

void RecBox::on_btDown_clicked()
{
    currentIndex++;
    if(currentIndex >= count)
    {
        currentIndex = 0;
    }
    createRecBoxItem();
}
