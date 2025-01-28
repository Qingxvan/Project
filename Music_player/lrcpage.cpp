#include "lrcpage.h"
#include "ui_lrcpage.h"
#include "qqmusic.h"
#include <QFile>

LrcPage::LrcPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LrcPage)
{
    ui->setupUi(this);
    // 设置窗口无标题栏
    setWindowFlag(Qt::FramelessWindowHint);

    // 初始化歌词上移动画对象
    animal = new QPropertyAnimation(this, "geometry", this);
    animal->setDuration(200);
    animal->setStartValue(QRect(10, 10, width(), height()));
    animal->setEndValue(QRect(10, 10 + height(), width(), height()));

    // 关联点击下拉按钮触发窗口回收
    connect(ui->hideBtn, &QPushButton::clicked, this, [=]{
        animal->start();
    });

    connect(animal, &QPropertyAnimation::finished, this, [=]{
        // 下拉回收时，将其隐藏
        hide();
    });
    ui->hideBtn->setIcon(QIcon(":/images/pullDown.png"));

    showLrcWordLine(-1);
}

LrcPage::~LrcPage()
{
    delete ui;
}

bool LrcPage::parseLrcFile(const QString &lrcFilePath)
{
    QFile file(lrcFilePath);

    // 打开文件之前清空之前数据
    lrcWordlines.clear();

    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "lrc File open error";
        return false;
    }

    while(!file.atEnd())
    {
        // 获取一行
        QString lrcLineWord = file.readLine(1024);

        // 分割时间和文本
        int start = 0, end = 0;
        end = lrcLineWord.indexOf(']', start);
        QString lrcTime = lrcLineWord.mid(start, end - start + 1);
        QString lrcWord = lrcLineWord.mid(end + 1, lrcLineWord.size() - end - 1 - 1);
        // 解析时间
        // 分
        qint64 lineTime = 0;
        start = 1;
        end = lrcTime.indexOf(':', start);
        // 转换为毫秒
        lineTime += lrcTime.mid(start, end - start).toInt() * 60 * 1000;

        // 秒
        start = end + 1;
        end = lrcTime.indexOf('.', start);
        // 转换为毫秒
        lineTime += lrcTime.mid(start, end - start).toInt() * 1000;

        // 毫秒
        start = end + 1;
        end = lrcTime.indexOf('.', start);
        // 转换为毫秒
        lineTime += lrcTime.mid(start, end - start).toInt();

        // 保存该行歌词
        lrcWordlines.push_back(LrcWordLine(lineTime, lrcWord));
    }

    return true;
}

void LrcPage::showLrcWordLine(qint64 time)
{
    // 获取当前music的时间在vector中的索引
    int index = getLineWordLineIndex(time);
    // 更新当前播放歌曲七行歌词
    if(time == -1)
    {
        ui->line1->setText("");
        ui->line2->setText("");
        ui->line3->setText("");
        ui->lineCenter->setText("当前无歌词");
        ui->line4->setText("");
        ui->line5->setText("");
        ui->line6->setText("");
    }
    else
    {
//        ui->musicName->setText(extractTitle(lrcWordlines));
//        ui->musicSinger->setText(extractArtist(lrcWordlines));
        ui->line1->setText(getLrcWordByindex(index - 3));
        ui->line2->setText(getLrcWordByindex(index - 2));
        ui->line3->setText(getLrcWordByindex(index - 1));
        ui->lineCenter->setText(getLrcWordByindex(index));
        ui->line4->setText(getLrcWordByindex(index + 1));
        ui->line5->setText(getLrcWordByindex(index + 2));
        ui->line6->setText(getLrcWordByindex(index + 3));
    }
}

int LrcPage::getLineWordLineIndex(qint64 time)
{
    // 对比time和vector中保存的time结果进行处理
    if(lrcWordlines.isEmpty())
    {
        return -1;
    }

    if(time <= lrcWordlines[0].lrcTime)
    {
        return 0;
    }

    for(int i = 1; i < lrcWordlines.size(); i++)
    {
        if(time >= lrcWordlines[i - 1].lrcTime && time < lrcWordlines[i].lrcTime)
        {
            return i - 1;
        }
    }
    // 歌曲结束时始终显示最后一行歌词
    return lrcWordlines.size() - 1;
}

QString LrcPage::getLrcWordByindex(int index)
{
    if(index < 0 || index >= lrcWordlines.size())
    {
        return "";
    }

    return lrcWordlines[index].lrcText;
}

//QString LrcPage::extractTitle(QVector<LrcWordLine>& lrcLines)
//{
//    QRegularExpression re("\\[ti:(.*?)\\]");
//    for (const auto& line : lrcLines) {
//        QRegularExpressionMatch match = re.match(line.lrcText);
//        if (match.hasMatch()) {
//            return match.captured(1);
//        }
//    }
//    return QString(); // 如果没有找到标题，返回空字符串
//}

//QString LrcPage::extractArtist(QVector<LrcWordLine>& lrcLines)
//{
//    QRegularExpression re("\\[ar:(.*?)\\]");
//    for (const auto& line : lrcLines) {
//        QRegularExpressionMatch match = re.match(line.lrcText);
//        if (match.hasMatch()) {
//            return match.captured(1);
//        }
//    }
//    return QString(); // 如果没有找到歌手名，返回空字符串
//}
