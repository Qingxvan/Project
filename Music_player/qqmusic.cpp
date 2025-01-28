#include "qqmusic.h"
#include "ui_qqmusic.h"
#include <QDebug>
#include <QPushButton>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#include <QJsonArray>
#include <QJsonObject>
#include <QFileDialog>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QMenu>

QQMusic::QQMusic(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QQMusic)
    ,currentIndex(-1)
{
    ui->setupUi(this);

    initui();
    initSqlite();
    initMusicList();
    initPlayer();
    connectSignalAndSlots();
}

QQMusic::~QQMusic()
{
    delete ui;
}

void QQMusic::initui()
{
    this->setWindowFlag(Qt::FramelessWindowHint);
    // 设置窗口背景透明度
    setAttribute(Qt::WA_TranslucentBackground);
    // 设置音乐程序图标
    setWindowIcon(QIcon(":/images/tubiao.png"));
    // 添加程序托盘
    QSystemTrayIcon* trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/images/tubiao.png"));
    trayIcon->show();
    // 添加托盘信息
    QMenu* trayMenu = new QMenu();
    trayMenu->addAction("显示", this, &QWidget::showNormal);
    trayMenu->addAction("退出", this, &QQMusic::onQQMusicQuit);
    trayIcon->setContextMenu(trayMenu);

    // 给窗口边缘增加阴影效果
    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setOffset(0,0);
    shadowEffect->setColor("#000000");
    shadowEffect->setBlurRadius(10);
    this->setGraphicsEffect(shadowEffect);

    // 设置btForm图标 & 文本信息
    ui->rec->setIconAndText(":/images/rec.png", "推荐", 0);
    ui->radio->setIconAndText(":/images/radio.png", "电台", 1);
    ui->music->setIconAndText(":/images/music.png", "乐馆", 2);
    ui->like->setIconAndText(":/images/like.png", "喜欢", 3);
    ui->local->setIconAndText(":/images/local.png", "本地和下载", 4);
    ui->recent->setIconAndText(":/images/recent.png", "最近播放", 5);


    // 将Page_local设置为默认窗口
    ui->stackedWidget->setCurrentIndex(4);
    currentPage = ui->page_local;

    // 在本地下载local中显示动画波动
    // ui->local->showAnimal(true);

    //randomPiction();

    srand(time(NULL));

    ui->rectMusicBox->initRecBoxUi(randomPiction(), 1);
    ui->supplayMusicBox->initRecBoxUi(randomPiction(), 2);

    // 添加CommonPage文本 图片信息
    ui->page_like->setCommonPageUi("喜欢", ":/images/rec/013.png");
    ui->page_local->setCommonPageUi("本地和下载", ":/images/rec/035.png");
    ui->page_recent->setCommonPageUi("最近播放", ":/images/rec/020.png");

    // 播放/暂停图标设置
    ui->play->setIcon(QIcon(":/images/play.png"));
    // 播放模式图片设置
    ui->playMode->setIcon(QIcon(":/images/playMode.png"));

    // 给音量按键关联上volumetool
    volumeTool = new VolumeTool(this);
    // 实例化Lrc歌词对象
    lrcPage = new LrcPage(this);
    lrcPage->setGeometry(10, 10, lrcPage->width(), lrcPage->height());
    lrcPage->hide();

    // 初始化歌词上移动画对象
    lrcPageAnimal = new QPropertyAnimation(lrcPage, "geometry", this);
    lrcPageAnimal->setDuration(200);
    lrcPageAnimal->setStartValue(QRect(10, 10 + lrcPage->height(), lrcPage->width(), lrcPage->height()));
    lrcPageAnimal->setEndValue(QRect(10, 10, lrcPage->width(), lrcPage->height()));

}

void QQMusic::initSqlite()
{
    // 数据库创建
    QSqlDatabase sqlite = QSqlDatabase::addDatabase("QSQLITE");

    // 设置数据库名称
    sqlite.setDatabaseName("QQMuisc.db");
    // 打开数据库
    if(!sqlite.open())
    {
        QMessageBox::critical(this, "QQMuisc", "sqlite open error !!!");
        return;
    }
    qDebug() << "QQMusic sqlite connect success ";
    // 创建表结构
    QString sql = "create table if not exists MusicInFo(\
                    id integer primary key autoincrement,\
                    musicId varchar(50) unique,\
                    musicName varchar(50),\
                    musicSinger varchar(50),\
                    albumName varchar(50),\
                    musicUrl varchar(256),\
                    duration bigint,\
                    isLike integer,\
                    isHistory integer)";

    QSqlQuery query;
    if(!query.exec(sql))
    {
        QMessageBox::critical(this, "QQMusic", "create table error !!!");
        return;
    }
    qDebug() << "create table success ";
}

void QQMusic::initMusicList()
{
    // 读取数据库内容
    musiclist.readFromDB();
    // 将数据库保存的信息更新到界面中
    ui->page_like->setMusicListType(PageType::LIKE_PAGE);
    ui->page_like->reFresh(musiclist);
    ui->page_local->setMusicListType(PageType::LOCAL_PAGE);
    ui->page_local->reFresh(musiclist);
    ui->page_recent->setMusicListType(PageType::HISTORY_PAGE);
    ui->page_recent->reFresh(musiclist);
}

// 初始化媒体播放控制
void QQMusic::initPlayer()
{
    // 初始化媒体播放相关类对象
    player = new QMediaPlayer(this);
    // 初始化播放类型
    playList = new QMediaPlaylist(this);
    // 设置默认播放模式
    playList->setPlaybackMode(QMediaPlaylist::Loop); // 循环播放
    // 将播放列表添加到播放媒体对象中
    player->setPlaylist(playList);
    // 设置默认音量大小
    player->setVolume(20);

    // 关联QMediaPlayer切换播放/暂停事件
    connect(player, &QMediaPlayer::stateChanged, this, &QQMusic::onPlayerStateChanged);
    // 关联歌曲切换时，歌曲时长的改变
    connect(player, &QMediaPlayer::durationChanged, this, &QQMusic::onDurationChanged);
    // 关联媒体时长发生变化信号
    connect(player, &QMediaPlayer::positionChanged, this, &QQMusic::onPositionChangde);
    // 播放列表模式发生改变信号关联
    connect(playList, &QMediaPlaylist::playbackModeChanged, this, &QQMusic::onPlaybackModeChanged);
    // playList中正在播放源发生改变时触发信号
    connect(playList, &QMediaPlaylist::currentIndexChanged, this, &QQMusic::onCurrentIndexChanged);
    // 关联媒体源发生改变是触发信号
    connect(player, &QMediaPlayer::metaDataAvailableChanged, this, &QQMusic::onMetaDataAvailableChanged);
}

// 推荐文本 + 推荐图片路径
QJsonArray QQMusic::randomPiction()
{
    QVector<QString> vecImageName;
    vecImageName << "001.png" << "002.png" << "003.png" << "004.png" << "005.png" << "006.png"
                 << "007.png" << "008.png" << "009.png" << "010.png" << "011.png" << "012.png"
                 <<	"013.png" << "014.png" << "015.png" << "016.png" << "017.png" << "018.png"
                 << "019.png" << "020.png" << "021.png" << "022.png" << "023.png" << "024.png"
                 << "025.png" << "026.png" << "027.png" << "028.png" << "029.png" << "030.png"
                 << "030.png" << "031.png" << "032.png" << "033.png" << "034.png" << "035.png"
                 <<	"036.png" << "037.png" << "038.png" << "039.png";
    std::random_shuffle(vecImageName.begin(), vecImageName.end());

    QJsonArray objArray;
    for(int i = 0; i < vecImageName.size(); i++)
    {
        QJsonObject obj;
        obj.insert("path", ":/images/rec/" + vecImageName[i]);

        QString strText = QString("推荐-%1").arg(i + 1, 3, 10, QChar('0'));
        obj.insert("text", strText);

        objArray.append(obj);
    }

    return objArray;
}

// 关联LeftBox中BtForm的信号和槽函数
void QQMusic::connectSignalAndSlots()
{
    connect(ui->rec, &BtForm::btClick, this, &QQMusic::onBtFormClick);
    connect(ui->radio, &BtForm::btClick, this, &QQMusic::onBtFormClick);
    connect(ui->music, &BtForm::btClick, this, &QQMusic::onBtFormClick);
    connect(ui->like, &BtForm::btClick, this, &QQMusic::onBtFormClick);
    connect(ui->local, &BtForm::btClick, this, &QQMusic::onBtFormClick);
    connect(ui->recent, &BtForm::btClick, this, &QQMusic::onBtFormClick);
    // 处理page_like,page_local,page_recent中添加或不添加音乐的处理
    connect(ui->page_like, &CommonPage::updataLikeMusic, this, &QQMusic::updataLikeMusicAndPage);
    connect(ui->page_local, &CommonPage::updataLikeMusic, this, &QQMusic::updataLikeMusicAndPage);
    connect(ui->page_recent, &CommonPage::updataLikeMusic, this, &QQMusic::updataLikeMusicAndPage);

    // 播放控制区信号槽函数关联事件 连接播放/暂停信号关联
    connect(ui->play, &QPushButton::clicked, this, &QQMusic::onPlayMusic);
    // 播放上一首事件信号关联
    connect(ui->playUp, &QPushButton::clicked, this, &QQMusic::onPlayUpClicked);
    // 播放下一首事件信号关联
    connect(ui->playDown, &QPushButton::clicked, this, &QQMusic::onPlayDownClicked);
    // 播放模式信号关联
    connect(ui->playMode, &QPushButton::clicked, this, &QQMusic::onPlaybackModeClicked);
    // 播放全部按钮信号关联 Page_like Page_local Page_recent
    connect(ui->page_like, &CommonPage::playAll, this, &QQMusic::onPlayAll);
    connect(ui->page_local, &CommonPage::playAll, this, &QQMusic::onPlayAll);
    connect(ui->page_recent, &CommonPage::playAll, this, &QQMusic::onPlayAll);
    // 处理双击Page_like、Page_local、Page_recent页面中listBox歌曲播放
    connect(ui->page_like, &CommonPage::playMusicByIndex, this, &QQMusic::playMusicByIndex);
    connect(ui->page_local, &CommonPage::playMusicByIndex, this, &QQMusic::playMusicByIndex);
    connect(ui->page_recent, &CommonPage::playMusicByIndex, this, &QQMusic::playMusicByIndex);
    // 处理静音事件
    connect(volumeTool, &VolumeTool::setMusicMuted, this, &QQMusic::setPlayerMuted);
    // 处理音量大小事件
    connect(volumeTool, &VolumeTool::setMusicVolume, this, &QQMusic::setPlayerVolume);
    // musicSlider拖动进度条当前歌曲进度改变事件
    connect(ui->progressBar, &MusicSlider::setMusicSliderPositon, this, &QQMusic::onMusicSliderChanged);

    // 歌词弹出窗口
    connect(ui->song, &QPushButton::clicked, this, &QQMusic::onLrcWordClicked);
}

void QQMusic::updateBtformAnimal()
{
    // 获取当前currentPage在stackedWidget中的索引
    int index = ui->stackedWidget->indexOf(currentPage);
    if(index == -1)
    {
        qDebug() << "该页面不存在 ";
        return;
    }
    // 获取QQMusic界面上所有的BTForm
    QList<BtForm*> btForms = this->findChildren<BtForm*>();
    for(auto btForm : btForms)
    {
        if(btForm->getPageId() == index)
        {
            btForm->showAnimal(true);
        }
        else
        {
            btForm->showAnimal(false);
        }
    }
}

void QQMusic::onQQMusicQuit()
{
    // 关闭之前将信息写入到数据库
    musiclist.writeToQQMusicDB();
    // 关闭之前将数据库链接断开
    sqlite.close();
    // 关闭程序
    close();
}


void QQMusic::on_quit_clicked()
{
    hide();
}

void QQMusic::onBtFormClick(int pageId)
{
    isDrag = false;

    QList<BtForm*> btFormList = this->findChildren<BtForm*>();
    for(auto btForm : btFormList)
    {
        if(btForm->getPageId() != pageId)
        {
            btForm->clearBackground();
        }
    }

    ui->stackedWidget->setCurrentIndex(pageId);
}

void QQMusic::mouseMoveEvent(QMouseEvent *event)
{
    if(Qt::LeftButton == event->buttons() && isDrag)
    {
        move(event->globalPos() - dragPosition);
    }
    QWidget::mouseMoveEvent(event);
}

void QQMusic::mousePressEvent(QMouseEvent *event)
{
    if(Qt::LeftButton == event->button())
    {
        isDrag = true;
        dragPosition = event->globalPos() - geometry().topLeft();
        return;
    }
    QWidget::mousePressEvent(event);
}

void QQMusic::updataLikeMusicAndPage(bool isLike, const QString &musicId)
{
    // 通过ID找到该音乐,修改状态
    auto it = musiclist.findMusicByMusicId(musicId);
    if(it != musiclist.end())
    {
        it->setIsLike(isLike);
    }
    // 更新页面歌曲列表
    ui->page_like->reFresh(musiclist);
    ui->page_local->reFresh(musiclist);
    ui->page_recent->reFresh(musiclist);
}

void QQMusic::on_volume_clicked()
{
    // 获取volume控件的left-top坐标，并转换为屏幕的全局坐标
    QPoint point = ui->volume->mapToGlobal(QPoint(0, 0));
    // 计算volumeTool偏移的位置
    QPoint volumeLeftTop = point - QPoint(volumeTool->width() / 2 - 20, volumeTool->height() - 20);
    // 移动volumeTool
    volumeTool->move(volumeLeftTop);
    // 显示调用
    volumeTool->show();
}

void QQMusic::on_addLocal_clicked()
{
    QFileDialog fileDialog(this);
    // 设置对话框标题
    fileDialog.setWindowTitle("添加本地下载音乐");
    // 设置对话框默认打开路径
    QDir dir(QDir::currentPath());
    dir.cdUp();
    QString projectPaht = dir.path();
    projectPaht += "/Music_player/images/music";
    // qDebug() << projectPaht;
    fileDialog.setDirectory(projectPaht);

    // 设置可以一次打开多个文件
    fileDialog.setFileMode(QFileDialog::ExistingFiles);
    // 添加过滤器 通过文件MIME类型过滤
    QStringList mimeTypeFilters;
    mimeTypeFilters << "application/octer-stream";
    fileDialog.setMimeTypeFilters(mimeTypeFilters);

    if(QDialog::Accepted == fileDialog.exec())
    {
        // 获取选中的文件
        QList<QUrl> fileUrls = fileDialog.selectedUrls();
        // 把文件信息加载到本地下载

        // 将所有歌曲添加到音乐列表中
        musiclist.addMusicsByUrl(fileUrls);
        // 将歌曲信息添加到CommonPage中的ListWidget里面
        // 将CommonPage页面切换到本地下载
        ui->stackedWidget->setCurrentIndex(4);

        ui->page_local->reFresh(musiclist);


        // 将page_local的歌曲列表添加到媒体播放列表
        ui->page_local->addMusicToPlaylist(musiclist, playList);
    }
}

// 播放/暂停 控制
void QQMusic::onPlayMusic()
{
    if(QMediaPlayer::PlayingState == player->state())
    {
        // 正在播放点击触发暂停事件
        player->pause();
        //ui->play->setIcon(QIcon(":/images/play.png"));
    }
    else if(QMediaPlayer::PausedState == player->state())
    {
        // 暂停点击触发继续播放事件
        //ui->play->setIcon(QIcon(":/images/play_on.png"));
        player->play();
    }
    else if(QMediaPlayer::StoppedState == player->state())
    {
        // 启动时点击触发开始播放事件
        player->play();
        //ui->play->setIcon(QIcon(":/images/play_on.png"));
    }
    else
    {
        qDebug() << player->error();
    }
}

void QQMusic::onPlayerStateChanged()
{
    if(player->state() == QMediaPlayer::PlayingState)
    {
        // 处于播放状态时
        ui->play->setIcon(QIcon(":/images/play_on.png"));
    }
    else
    {
        ui->play->setIcon(QIcon(":/images/play.png"));
    }
}

void QQMusic::onPlayUpClicked()
{
    playList->previous();
}

void QQMusic::onPlayDownClicked()
{
    playList->next();
}

// 获取当前播放模式状态切换到下一个模式状态
void QQMusic::onPlaybackModeClicked()
{
    // 列表循环->随机播放->单曲循环
    if(playList->playbackMode() == QMediaPlaylist::Loop)
    {
        // 列表循环->随机播放
        playList->setPlaybackMode(QMediaPlaylist::Random);
        ui->playMode->setToolTip("随机播放");
    }
    else if(playList->playbackMode() == QMediaPlaylist::Random)
    {
        playList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        ui->playMode->setToolTip("单曲循环");
    }
    else if(playList->playbackMode() == QMediaPlaylist::CurrentItemInLoop)
    {
        playList->setPlaybackMode(QMediaPlaylist::Loop);
        ui->playMode->setToolTip("列表循环");
    }
    else
    {
        qDebug() << "暂未支持";
    }
}

void QQMusic::onPlaybackModeChanged(QMediaPlaylist::PlaybackMode playbackMode)
{
    if(playbackMode == QMediaPlaylist::Loop)
    {
        ui->playMode->setIcon(QIcon(":/images/playMode.png"));
    }
    else if(playbackMode == QMediaPlaylist::Random)
    {
        ui->playMode->setIcon(QIcon(":/images/shuffle.png"));
    }
    else if(playbackMode == QMediaPlaylist::CurrentItemInLoop)
    {
        ui->playMode->setIcon(QIcon(":/images/single_play.png"));
    }
    else
    {
        qDebug() << "暂未支持该模式";
    }
}

void QQMusic::onPlayAll(PageType pageType)
{
    // 播放当前对应页面中的所有歌曲
    CommonPage* page = ui->page_local;

    switch(pageType)
    {
    case PageType::LIKE_PAGE:
        page = ui->page_like;
        break;
    case PageType::LOCAL_PAGE:
        page = ui->page_local;
        break;
    case PageType::HISTORY_PAGE:
        page = ui->page_recent;
        break;
    default:
        qDebug() << "无该页面";
    }
    // 播放当前page页面
    playAllMusicOfCommonPage(page, 0);
}

void QQMusic::playAllMusicOfCommonPage(CommonPage *page, int index)
{
    (void)index;

    currentPage = page;
    updateBtformAnimal();

    // 清空之前加载过的playlist
    playList->clear();
    // 添加当前播放页面歌曲
    page->addMusicToPlaylist(musiclist, playList);
    // 从第一首开始播放
    playList->setCurrentIndex(index);
    player->play();
}

void QQMusic::playMusicByIndex(CommonPage *page, int index)
{
    playAllMusicOfCommonPage(page, index);
}

void QQMusic::onCurrentIndexChanged(int index)
{
    currentIndex = index;
    // 获取当前页面正在播放的音乐
    QString musicId = currentPage->getMusicIdByIndex(index);
    // 通过ID拿到对象，修改isHistory属性
    auto it = musiclist.findMusicByMusicId(musicId);
    if(it != musiclist.end())
    {
        it->setIsHistory(true);
    }
    // 将歌曲添加到Page_recent中
    ui->page_recent->reFresh(musiclist);
}

void QQMusic::setPlayerMuted(bool isMuted)
{
    player->setMuted(isMuted);
}

void QQMusic::setPlayerVolume(int volume)
{
    player->setVolume(volume);
}

void QQMusic::onDurationChanged(qint64 duration)
{
    totalTime = duration;
    // 总时长单位为毫秒
    // 时长/宽度/进制/补位
    ui->totalTime->setText(QString("%1:%2").arg(duration / 1000 / 60, 2, 10, QChar('0'))
                             .arg(duration / 1000 % 60, 2, 10, QChar('0')));
}

void QQMusic::onPositionChangde(qint64 position)
{
    // 更新当前播放时间的Text
    ui->currentTime->setText(QString("%1:%2").arg(position / 1000 / 60, 2, 10, QChar('0'))
                             .arg(position / 1000 % 60, 2, 10, QChar('0')));
    // 实时更新进度条
    ui->progressBar->setStep(position / (float)totalTime);
    // 同步歌词界面显示
    if(currentIndex >= 0)
    {
        lrcPage->showLrcWordLine(position);
    }
}

void QQMusic::onMusicSliderChanged(float ration)
{
    qint64 duration = totalTime * ration;

    ui->currentTime->setText(QString("%1:%2").arg(duration / 1000 / 60, 2, 10, QChar('0'))
                                             .arg(duration / 1000 % 60, 2, 10, QChar('0')));
    // 媒体播放时间修改
    player->setPosition(duration);
}

void QQMusic::onMetaDataAvailableChanged(bool available)
{
    (void) available;
    // 获取当前正在播放的媒体源索引
    QString musicId = currentPage->getMusicIdByIndex(currentIndex);
    auto it = musiclist.findMusicByMusicId(musicId);
    QString musicName("未知歌曲");
    QString musicSinger("未知歌手");
    if(it != musiclist.end())
    {
        musicName = it->getMusicName();
        musicSinger = it->getMusicSinger();
    }
    ui->musicName->setText(musicName);
    ui->musicSinger->setText(musicSinger);
    // 获取封面图
    QVariant coverimage = player->metaData("ThumbnailImage");

    if(coverimage.canConvert<QImage>())
    {
        QImage image = coverimage.value<QImage>();
        ui->musicCover->setPixmap(QPixmap::fromImage(image));
        currentPage->setMusicImage(QPixmap::fromImage(image));
    }
    else
    {
        qDebug() << "该歌曲没有封面图";
        QString path = ":/images/rec/038.png";
        ui->musicCover->setPixmap(path);
        // currentPage->setMusicImage(path);
    }

    ui->musicCover->setScaledContents(true);

    // 解析歌曲Lrc文件
    if(it != musiclist.end())
    {
        // 获取路径
        QString lrcPath = it->getLrcFilePath();
        // 解析
        lrcPage->parseLrcFile(lrcPath);
    }

}

void QQMusic::onLrcWordClicked()
{
    // 调用显示
    lrcPage->show();

    lrcPageAnimal->start();
}
// 换肤
void QQMusic::on_skin_clicked()
{
    QMessageBox::information(this, "提示", "换肤功能暂未开发...");
}

void QQMusic::on_min_clicked()
{
    showMinimized();
}
