    #include "volumetool.h"
#include "ui_volumetool.h"

#include <QGraphicsDropShadowEffect>
#include <QPainter>

VolumeTool::VolumeTool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VolumeTool),
    isMuted(false),
    volumeRatio(20)
{
    ui->setupUi(this);
    // 弹出音量条件界面
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    // 将Widget多余边界隐藏
    setAttribute(Qt::WA_TranslucentBackground);
    // 窗口自定义阴影效果
    QGraphicsDropShadowEffect * shadowsEffect = new QGraphicsDropShadowEffect(this);
    shadowsEffect->setColor("#646464");
    shadowsEffect->setBlurRadius(14);
    shadowsEffect->setOffset(0, 0);
    this->setGraphicsEffect(shadowsEffect);

    // 设置音量静音按钮图标
    ui->silenceBtn->setIcon(QIcon(":/images/volume.png"));

    // 设置默认音量百分比
    ui->volumeRatio->setText("20%");
    // 设置out_line尺寸
    QRect rect = ui->outLine->geometry();
    // 重新坐标
    ui->outLine->setGeometry(rect.x(), 180 - 36 + 25, rect.width(), 36);
    // 定位按钮位置
    ui->silderBtn->move(ui->silderBtn->x(), ui->outLine->y() - ui->silderBtn->y() / 2);
    // 静音信号事件
    connect(ui->silenceBtn, &QPushButton::clicked, this, &VolumeTool::onSilenceBtnClicked);
    // 事件过滤器
    ui->volumeBox->installEventFilter(this);
}

VolumeTool::~VolumeTool()
{
    delete ui;
}

void VolumeTool::paintEvent(QPaintEvent *event)
{
    (void)event;
    // 绘制volumerTool界面下的倒三角
    QPainter painter(this);
    // 设置画笔
    painter.setPen(Qt::NoPen);
    // 设置画刷
    painter.setBrush(::QBrush(Qt::white));
    // 绘制三角形 定位坐标系
    QPolygon polygon;
    QPoint a(10, 300 + 5);
    QPoint b(10 + 60, 300 + 5);
    QPoint c(10 + 30, 300 + 20);
    polygon.append(a);
    polygon.append(b);
    polygon.append(c);

    painter.drawPolygon(polygon);

}

void VolumeTool::onSilenceBtnClicked()
{
    isMuted = !isMuted;
    if(isMuted)
    {
        ui->silenceBtn->setIcon(QIcon(":/images/silent.png"));
    }
    else
    {
        ui->silenceBtn->setIcon(QIcon(":/images/volume.png"));
    }
    // 向上层QQMusic发射静音信号
    emit setMusicMuted(isMuted);
}

bool VolumeTool::eventFilter(QObject *watched, QEvent *event)
{
    // VolumeBox控件内发生信号
    if(ui->volumeBox == watched)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            // 鼠标按下
            calcVolume();
        }
        else if(event->type() == QEvent::MouseButtonRelease)
        {
            // 鼠标释放
            emit setMusicVolume(volumeRatio);
        }
        else if(event->type() == QEvent::MouseMove)
        {
            // 鼠标移动
            calcVolume();
            emit setMusicVolume(volumeRatio);
        }
        return true;
    }
    return QObject::eventFilter(watched, event);
}

void VolumeTool::calcVolume()
{
    // 获取鼠标点击时Y的坐标
    int height = ui->volumeBox->mapFromGlobal(QCursor().pos()).y();
    // height坐标合法性
    height = height < 25 ? 25 : height;
    height = height > 205 ? 205 : height;
    // 更新outLine动画
    ui->outLine->setGeometry(ui->outLine->x(), height, ui->outLine->width(), 205 - height);
    // 更新sliderBtn按钮位置
    ui->silderBtn->move(ui->silderBtn->x(), ui->outLine->y() - ui->silderBtn->height() / 2);
    // 计算音量大小
    volumeRatio = (int)(ui->outLine->height() / (float)180 * 100);
    // 更新百分比
    ui->volumeRatio->setText(QString::number(volumeRatio) + "%");
}























