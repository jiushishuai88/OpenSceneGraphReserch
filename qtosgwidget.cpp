/******************************************************************************
*
* Copyright (c) 20XX-20XX, XXXXXXXX有限公司
*
* All rights reserved.
*
* 文 件 名: qtosgwidget.cpp
*
* 作    者: guchuan
*
* 创建日期: 2021.10
*
* 描    述: 实现osg嵌入qt widget.
*
******************************************************************************/


/***** pragma *****/
#pragma execution_character_set("utf-8")

#include "qtosgwidget.h"
#include <QApplication>
#include <QDesktopWidget>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <QWheelEvent>
#include <osg/Matrix>
#include<osg/PolygonMode>
#include <osgGA/StateSetManipulator>
#include <osg/CopyOp>
#include <osg/LineWidth>
#include <osg/PolygonOffset>
#include <osgUtil/Tessellator>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include "shapenodegenerator.h"
#include "osgUtil/SmoothingVisitor"
#include "texture2dmanager.h"

QtOSGWidget::QtOSGWidget(QWidget* parent):
    QOpenGLWidget(parent),
    m_pGraphicsWindow(new osgViewer::GraphicsWindowEmbedded(this->x(),this->y(),this->width()/2,this->height()/2)),
    m_pViewer(new osgViewer::Viewer()),
    m_scale(QApplication::desktop()->devicePixelRatio()),
    m_pRoot(new osg::Group),
    m_pCamera(new osg::Camera),
    m_pManipulator(new osgGA::TrackballManipulator)
{
    setMouseTracking(true);
    SetCamera();
    SetManipulator();
    SetViewer();
}
QtOSGWidget::~QtOSGWidget()
{
    m_pGraphicsWindow = nullptr;
    m_pViewer = nullptr;
    m_pRoot.release();
    m_pRoot = nullptr;
    m_pCamera = nullptr;
    m_pManipulator = nullptr;
}
void QtOSGWidget::initializeGL()
{
    osg::ref_ptr<osg::Group> group = dynamic_cast<osg::Group*>( m_pViewer->getSceneData());
    osg::ref_ptr<osg::StateSet> stateSet = group->getOrCreateStateSet();
    osg::ref_ptr<osg::Material> material = new osg::Material();
    material->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
    stateSet->setAttributeAndModes( material, osg::StateAttribute::ON );
    stateSet->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );
}
 void QtOSGWidget::resizeGL(int width, int height)
{
    m_pGraphicsWindow->getEventQueue()->windowResize(this->x()*m_scale, this->y() * m_scale, width*m_scale, height*m_scale);
    m_pGraphicsWindow->resized(this->x()*m_scale, this->y() * m_scale, width*m_scale, height*m_scale);
    m_pViewer->getCamera()->setViewport(0, 0, this->width()*m_scale, this->height()* m_scale);
}
 void QtOSGWidget::paintGL()
 {
     m_pViewer->frame();
 }
 void QtOSGWidget::mouseMoveEvent(QMouseEvent* event)
 {
     m_pGraphicsWindow->getEventQueue()->mouseMotion(event->x()*m_scale, event->y()*m_scale);
 }
void QtOSGWidget::mousePressEvent(QMouseEvent* event)
{
    unsigned int button = 0;
          switch (event->button()){
          case Qt::LeftButton:
              button = 1;
              break;
          case Qt::MiddleButton:
              button = 2;
              break;
          case Qt::RightButton:
              button = 3;
              break;
          default:
              break;
          }
  m_pGraphicsWindow->getEventQueue()->mouseButtonPress(event->x()*m_scale, event->y()*m_scale, button);
}
void QtOSGWidget::mouseReleaseEvent(QMouseEvent* event)
{
    unsigned int button = 0;
         switch (event->button()){
         case Qt::LeftButton:
             button = 1;
             break;
         case Qt::MiddleButton:
             button = 2;
             break;
         case Qt::RightButton:
             button = 3;
             break;
         default:
             break;
         }
   m_pGraphicsWindow->getEventQueue()->mouseButtonRelease(event->x()*m_scale, event->y()*m_scale, button);
}
void QtOSGWidget::wheelEvent(QWheelEvent* event)
{
    int delta = event->delta();
    osgGA::GUIEventAdapter::ScrollingMotion motion = delta > 0 ?osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN;
    m_pGraphicsWindow->getEventQueue()->mouseScroll(motion);
}
bool QtOSGWidget::event(QEvent* event)
{
    bool handled = QOpenGLWidget::event(event);
    this->update();
    return handled;
}

void QtOSGWidget::DrawSphere(const osg::Vec3& center,float radius, const osg::Vec4& color)
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    osg::ref_ptr<osg::Sphere> sphere    = new osg::Sphere( center,radius);
    osg::ref_ptr<osg::ShapeDrawable> sd = new osg::ShapeDrawable(sphere);
    sd->setColor(color);
    geode->addDrawable(sd);
    m_pRoot->addChild(geode);
}

 void QtOSGWidget::DrawCylinder(const osg::Vec3& center,float radius,float height ,osg::Vec3 nTo)
 {
     osg::ref_ptr<osg::MatrixTransform> mt = ShapeNodeGenerator::GetInstance()->GetCylinder(center,radius,height,nTo);
     m_pRoot->addChild(mt);
 }

void QtOSGWidget::DrawRectangleGrid(const osg::Vec3& upleft,float width,float height,
                                    osg::Vec3& nTo,int wDim,int hDim)
{
   osg::ref_ptr<osg::MatrixTransform> mt =ShapeNodeGenerator::GetInstance()->GetRectangleGrid(upleft,width,height,nTo,wDim,hDim);
   m_pRoot->addChild(mt);
}

void QtOSGWidget::SetCamera()
{
    m_pCamera->setViewport( 0, 0, this->width(), this->height() );
    m_pCamera->setClearColor( osg::Vec4( 0.9f, 0.9f, .9f, 1.f ) );
    float aspectRatio = static_cast<float>( this->width()) / static_cast<float>( this->height() );
    m_pCamera->setProjectionMatrixAsPerspective( 50.f, aspectRatio, 1.f, 1000.f );
    m_pCamera->setGraphicsContext( m_pGraphicsWindow );
}

void QtOSGWidget::SetManipulator()
{
    m_pManipulator->setHomePosition(osg::Vec3d(0,-5,0),osg::Vec3d(0,0,0),osg::Vec3d(0,0,1));
  m_pManipulator->setAllowThrow( false );
}

void QtOSGWidget::SetViewer()
{
    m_pViewer->setCamera(m_pCamera);
    m_pViewer->addEventHandler(new osgGA::StateSetManipulator(m_pViewer->getCamera()->getOrCreateStateSet()));
    m_pViewer->setSceneData(m_pRoot);
    m_pViewer->setCameraManipulator(m_pManipulator);
    m_pViewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
    m_pViewer->realize();
}

void QtOSGWidget::DrawCircleGrid(const osg::Vec3& center,float radius,int rDim,int arcDim,osg::Vec3& nTo)
{
    osg::ref_ptr<osg::MatrixTransform> mt = ShapeNodeGenerator::GetInstance()->GetCircleGrid(center,radius,rDim,arcDim,nTo);
    m_pRoot->addChild(mt);
}

void QtOSGWidget::SetData(QVector<struct eggData> data)
{
    m_veggUnit = data;
    for(int i=0;i<m_veggUnit.size();++i)
    {
        Draw(m_veggUnit.at(i));
    }
}

void QtOSGWidget::AddData(eggData data)
{
    m_veggUnit.append(data);
    Draw(data);
}

 void QtOSGWidget::Draw(const eggData &data)
 {
     Ogive ogive = {data};
     m_pRoot->addChild(ogive.GetMt());
 }

 void QtOSGWidget::TestFuc()
 {
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
     Ogive ogive = {data};
     m_pRoot->addChild(ogive.GetMt());
 }
