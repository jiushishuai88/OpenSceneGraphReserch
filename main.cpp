#include "mainwindow.h"

#include <QApplication>
#include "qtosgwidget.h"
#include <QDebug>
#include "globalmanager.h"
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
    GlobalManager::GetInstance()->Init();
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
    data.center = {-6,0,0};
    osgWD->AddData(data);


    data.length= 4;
    data.innerRadiusLeft = 1;
    data.innerRadiusRight = 1;
    data.arcInner = 0;
    data.extRadiusLeft = 2;
    data.extRadiusRight = 3;
    data.arcExt = -osg::PI_4;
    data.normal = {-1,0,0};
    data.textureID = Texture2DManager::T2DID_TEST;
    data.color = {1,1,1,1};
    data.center = {-2,0,0};
    osgWD->AddData(data);


    data.length= 10;
    data.center = {5,0,0};
    data.innerRadiusLeft = 1;
    data.innerRadiusRight = 1;
    data.arcInner = 0;
    data.extRadiusLeft = 3;
    data.extRadiusRight = 3;
    data.arcExt = 0;
    data.color = {0.5,0.5,0.5,1};
    data.textureID = Texture2DManager::T2DID_NONE;
    osgWD->AddData(data);
    //------------------------------------------------------------------------------------------------------
    w.setCentralWidget(osgWD);
    w.show();
    return a.exec();
}
