#ifndef LRCPAGE_H
#define LRCPAGE_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QRegularExpression>

struct LrcWordLine
{
    LrcWordLine(qint64 lrcTime, QString lrcText)
    {
        this->lrcTime = lrcTime;
        this->lrcText = lrcText;
    }
    qint64 lrcTime;
    QString lrcText;
};

namespace Ui {
class LrcPage;
}

class LrcPage : public QWidget
{
    Q_OBJECT

public:
    explicit LrcPage(QWidget *parent = nullptr);
    ~LrcPage();

    // 歌词解析
    bool parseLrcFile(const QString& lrcFilePath);
    // 歌词同步
    void showLrcWordLine(qint64 time);
    // 获取当前行索引
    int getLineWordLineIndex(qint64 time);
    // 获取当前索引歌词
    QString getLrcWordByindex(int index);
    QString extractTitle(QVector<LrcWordLine>& lrcLines);
    QString extractArtist(QVector<LrcWordLine>& lrcLines);
private:
    Ui::LrcPage *ui;

    QPropertyAnimation* animal;

    QVector<LrcWordLine> lrcWordlines;
};

#endif // LRCPAGE_H
