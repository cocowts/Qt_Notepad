#include "MainWindow.h"
#include <QApplication>
#include <QFileInfo>
#include <QTranslator>
#include <QDebug>
#include <QLibraryInfo>
#include <QProcess>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv); 

    // 方式 1 加载添加到工程中的资源文件
    QTranslator t;
    if( t.load(":/Res/qm/qt_zh_CN.qm") )
    {
        a.installTranslator(&t);
    }

    // 方式 2 记载 qt 库实例的信息
    // QTranslator t;
    // if( t.load("qt_zh_CN.qm", QLibraryInfo::location(QLibraryInfo::TranslationsPath)) )
    // {
    //    a.installTranslator(&t);
    // }

    MainWindow* w = MainWindow::NewInstance();
    int ret = -1;

    if( w != nullptr )
    {
      if( argc > 1 )
      {
            QFileInfo fi(argv[1]);

            if( fi.exists() )
            {
                w->openFile(argv[1]);
            }
      }

       w->show();

       ret = a.exec();

       delete w;
    }

    return ret;
}
