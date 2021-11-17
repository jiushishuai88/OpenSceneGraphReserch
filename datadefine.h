/******************************************************************************
*
* Copyright (c) 20XX-20XX, XXXXXXXX有限公司
*
* All rights reserved.
*
* 文 件 名: datadefine.h
*
* 作    者: guchuan
*
* 创建日期: 2021.10
*
* 描    述: 用于定义自定义数据类型
*
******************************************************************************/
#pragma execution_character_set("utf-8")

#ifndef DATADEFINE_H
#define DATADEFINE_H

#define CYLINDER "cylinder"
#define BALL "ball"
#define CONE "cone"


#include<QtCore>
#include<QColor>
#include <osg/ValueObject>
#include <osg/Vec4>
#include <osg/Texture2D>



struct eggUnit{
    float x1, y1; //center and left pofloat
    QString shape;//the shape of the unit, 1 cylinder, 2 cone, 3 oval, 4 antioval, 5 ball
    QString name;//the unit name
    float length;//the length of the unit
    float r1;//external diameter: the radius of the left section,
    float r2;//external diameter: the radius of the right section, if shape is ball then don't use r2.
    float coreRadius;//the radius of core
    float arcRadius;//use while the shape is oval or antioval, the arcRadius is the radius of curved surface

    float leftShellThick;//left: total thick of layer 1-5
    float rightShellThick;//right: total thick of layer 1-5

    float leftLayerThick1;//left: layer 1 thick
    float leftLayerThick2;//left: layer 2 thick
    float leftLayerThick3;//left: layer 3 thick
    float leftLayerThick4;//left: layer 4 thick
    float leftLayerThick5;//left: layer 5 thick

    float rightLayerThick1;//right: layer 1 thick
    float rightLayerThick2;//right: layer 2 thick
    float rightLayerThick3;//right: layer 3 thick
    float rightLayerThick4;//right: layer 4 thick
    float rightLayerThick5;//right: layer 5 thick

    QColor layerColor1; //the layer1 color
    QColor layerColor2; //the layer2 color
    QColor layerColor3; //the layer3 color
    bool useTest2d;
};


class BasePrimtiveData
{
public:
    enum PriMtiveType
    {
        PriMtiveType_PIPE,
        PriMtiveType_OGIVE_PIPE
    };
public:
    BasePrimtiveData(PriMtiveType);
    virtual ~BasePrimtiveData();
    const osg::Vec3 baseNormal;
    osg::Vec3 center;
    osg::Vec3 normal;
    osg::Vec4 color;
    PriMtiveType type;
    osg::ref_ptr<osg::Texture2D> pT2D = nullptr;
};


class PipeData:public BasePrimtiveData
{
public:
    PipeData(PriMtiveType);
    virtual ~PipeData();
    float length;
    float innerRadius;
    float extRadius;
};

class OgivePipeData:public PipeData
{
public:
    OgivePipeData(PriMtiveType);
    virtual ~OgivePipeData();
    float arc;//0<arc<2π
};

#endif // DATADEFINE_H
