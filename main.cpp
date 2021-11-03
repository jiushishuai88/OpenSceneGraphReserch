#include "mainwindow.h"

#include <QApplication>
#include "qtosgwidget.h"
#include <QDebug>
extern "C"
{
#include "wealib.h"
}
int main(int argc, char *argv[])
{
    //QApplication::setAttribute(Qt::AA_DisableHighDpiScaling);

    QApplication a(argc, argv);
    MainWindow w;
    QtOSGWidget* osgWD = new QtOSGWidget(&w);
    //----------------------------------------------------------------------------------------------
    osgWD->TestFuc();
    //------------------------------------------------------------------------------------------------------
    w.setCentralWidget(osgWD);
    w.show();
    return a.exec();
}
