#include "qqmusic.h"

#include <QApplication>
#include <QSharedMemory>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSharedMemory sharedMemory("QQMusic");
    // 第一次运行开辟共享内存，第二次运行时直接让程序退出
    if(sharedMemory.attach())
    {
        QMessageBox::information(nullptr, "QQMusic提示", "QQMusic已经在运行了... ");
        return 0;
    }
    sharedMemory.create(1);

    QQMusic w;
    w.show();
    return a.exec();
}
