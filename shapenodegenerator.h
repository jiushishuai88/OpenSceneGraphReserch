/******************************************************************************
*
* Copyright (c) 20XX-20XX, XXXXXXXX有限公司
*
* All rights reserved.
*
* 文 件 名: shapenodegenerator.h
*
* 作    者: guchuan
*
* 创建日期: 2021.10
*
* 描    述: osg图形生成器，用于生成各种图形的原始数据,目前只有几个简单基础图元用于基础测试
*
******************************************************************************/
#pragma execution_character_set("utf-8")

#ifndef SHAPENODEGENERATOR_H
#define SHAPENODEGENERATOR_H
#include<osg/MatrixTransform>
#include<QtCore>
#include "datadefine.h"


class ShapeNodeGenerator
{
private:
    ShapeNodeGenerator();
    static ShapeNodeGenerator* m_pSelf;
public:
    static ShapeNodeGenerator* GetInstance();
    /**生成矩形网格**/
    osg::ref_ptr<osg::MatrixTransform> GetRectangleGrid(const osg::Vec3& upleft,float width,float height,
                                              osg::Vec3& nTo,int wDim,int hDim);
    /**生成圆形网格**/
    osg::ref_ptr<osg::MatrixTransform> GetCircleGrid(const osg::Vec3& center,float radius,int rDim,int arcDim,
                                                  osg::Vec3& nTo);
    /**生成圆柱体**/
    osg::ref_ptr<osg::MatrixTransform> GetCylinder(const osg::Vec3& center,float radius,float height ,osg::Vec3 nTo,bool useText2d=false);
     /**生成圆锥体**/
    osg::ref_ptr<osg::MatrixTransform> GetCone(const osg::Vec3& center,float radius,float height ,osg::Vec3 nTo);
    void GenerateSurface(osg::Geometry* geom,int* index1,int* index2,int col);
    osg::ref_ptr<osg::MatrixTransform> GetPipe(PipeData*);
    osg::ref_ptr<osg::Vec3Array> GenerateCirclePoints( float z,float radius,int points);
};

#endif // SHAPENODEGENERATOR_H
