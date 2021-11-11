QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    datadefine.cpp \
    main.cpp \
    mainwindow.cpp \
    qtosgwidget.cpp \
    shapenodegenerator.cpp

HEADERS += \
    datadefine.h \
    mainwindow.h \
    qtosgwidget.h \
    shapenodegenerator.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../OpenSceneGraph-3.4.1-VC2017-64-Debug/lib/ \
-lOpenThreads \
-losgAnimation \
-losg \
-losgDB \
-losgFX \
-losgGA \
-losgManipulator \
-losgParticle \
-losgPresentation \
-losgQt \
-losgShadow \
-losgSim \
-losgTerrain \
-losgText \
-losgUI \
-losgUtil \
-losgViewer \
-losgVolume \
-losgWidget \

else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../OpenSceneGraph-3.4.1-VC2017-64-Debug/lib/ \
-lOpenThreadsd \
-lOpenThreadsd \
-losgAnimationd \
-losgd \
-losgDBd \
-losgFXd \
-losgGAd \
-losgManipulatord \
-losgParticled \
-losgPresentationd \
-losgQtd \
-losgShadowd \
-losgSimd \
-losgTerraind \
-losgTextd \
-losgUId \
-losgUtild \
-losgViewerd \
-losgVolumed \
-losgWidgetd \

INCLUDEPATH += $$PWD/../../../OpenSceneGraph-3.4.1-VC2017-64-Debug/include
DEPENDPATH += $$PWD/../../../OpenSceneGraph-3.4.1-VC2017-64-Debug/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../x64/lib/release/ \
-lalgebra \
-lpowerfield \
-lterminaltext \
-lwealib \

else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../x64/lib/debug/ \
-lalgebra \
-lpowerfield \
-lterminaltext \
-lwealib \

INCLUDEPATH += $$PWD/../../x64/include
DEPENDPATH += $$PWD/../../x64/include
