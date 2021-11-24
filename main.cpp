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
    eggData data;
    data.length= 4;
    data.innerRadiusLeft = 0;
    data.innerRadiusRight = 1;
    data.arcInner = osg::PI_2;
    data.extRadiusLeft = 0;
    data.extRadiusRight = 2;
    data.arcExt = osg::PI_2;
    data.normal = {-1,0,0};
    data.textureID = Texture2DManager::T2DID_TEST;
    data.color = {1,1,1,1};
    osgWD->AddData(data);
    //------------------------------------------------------------------------------------------------------
    w.setCentralWidget(osgWD);
    w.show();
    return a.exec();
}
