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
#include "texture2dmanager.h"


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

struct eggData
{
    float length = {1};
    float innerRadiusLeft = {1};
    float innerRadiusRight = {1};
    float arcInner = 0.01;

    float extRadiusLeft = {0.5};
    float extRadiusRight = {0.5};
    float arcExt = 0.01;
    osg::Vec3 center = {0,0,0};
    osg::Vec3 normal = {0,0,1};
    osg::Vec4 color = {1,1,1,1};
    Texture2DManager::T2DID textureID = Texture2DManager::T2DID_NONE;
};

#endif // DATADEFINE_H
