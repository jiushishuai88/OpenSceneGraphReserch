/******************************************************************************
*
* Copyright (c) 20XX-20XX, XXXXXXXX有限公司
*
* All rights reserved.
*
* 文 件 名: qtoswidget
*
* 作    者: guchuan
*
* 创建日期: 2021.10
*
* 描    述: osg widget
*
******************************************************************************/
#pragma execution_character_set("utf-8")

#ifndef QTOSGWIDGET_H
#define QTOSGWIDGET_H
#include <QOpenGLWidget>
#include <osgGA/EventQueue>
#include <osgViewer/GraphicsWindow>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include <osg/MatrixTransform>
#include "datadefine.h"
#include "ogive.h"

/** osg widget类，3d显示接口 **/
class QtOSGWidget:public QOpenGLWidget
{
public:
    QtOSGWidget(QWidget* parent=nullptr);
    virtual ~QtOSGWidget();
    /** 绘制矩形网格 **/
    void DrawRectangleGrid(const osg::Vec3& center,float width,float hight,
                                              osg::Vec3& nTo,int wDim,int hDim);
    /** 绘制圆形网格* */
    void DrawCircleGrid(const osg::Vec3& center,float radius,
                                           int rDim,int arcDim,osg::Vec3& nTo);
    /** 绘制球形 **/
    void DrawSphere(const osg::Vec3& center,float radius, const osg::Vec4& color);
    /** 绘制圆柱 **/
    void DrawCylinder(const osg::Vec3& center,float radius, float height,osg::Vec3 nTo);
    /** 设置场景显示标准数据 **/
    void SetData(QVector<struct eggData> data);
    /** 添加场景物体标准数据 **/
    void AddData(eggData data);
    void TestFuc();
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual bool event(QEvent* event) override;
private:
    void SetCamera();
    void SetManipulator();
    void SetViewer();
    void Draw(const eggData& data);
    osg::ref_ptr<osgViewer::GraphicsWindow> m_pGraphicsWindow;
    osg::ref_ptr<osgViewer::Viewer> m_pViewer;
    qreal m_scale;
    osg::ref_ptr<osg::Group> m_pRoot;
    osg::ref_ptr<osg::Camera> m_pCamera;
    osg::ref_ptr<osgGA::TrackballManipulator> m_pManipulator;
    QVector<struct eggData> m_veggUnit;
};

#endif // QTOSGWIDGET_H
