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
class Ogive;


class ShapeNodeGenerator
{
private:
    ShapeNodeGenerator();
    static ShapeNodeGenerator* m_pSelf;
    /** 对三维坐标数组进行转换 **/
     void V3ArrayTransform(osg::Vec3Array* pArr,const osg::Matrixd& mat);
     /** 生成圆形基点 默认基准点为（0,0,0）法线(0,0,1)**/
     osg::ref_ptr<osg::Vec3Array> GenerateCirclePoints(float radius,int points,float arcFrom =0,float arc = 2*osg::PI);
     /** 根据半径生成顶点数 **/
     ulong ComputePointsByRadius(float r,float arc = 2*osg::PI);
     /** 将数组点首尾相连 **/
     void LoopPoints( osg::ref_ptr<osg::Vec3Array>);
     osg::ref_ptr<osg::Geometry> GenerateOgive(Ogive* data);
     /** 根据顶点和脚标构建几何图形表面 **/
     void GenerateSurface(osg::Geometry* geom,int* index1,int* index2,int col);

public:
    static ShapeNodeGenerator* GetInstance();
    /** 生成矩形网格 **/
    osg::ref_ptr<osg::MatrixTransform> GetRectangleGrid(const osg::Vec3& upleft,float width,float height,
                                              osg::Vec3& nTo,int wDim,int hDim);
    /** 生成圆形网格 **/
    osg::ref_ptr<osg::MatrixTransform> GetCircleGrid(const osg::Vec3& center,float radius,int rDim,int arcDim,
                                                  osg::Vec3& nTo);
    /** 生成圆柱体 **/
    osg::ref_ptr<osg::MatrixTransform> GetCylinder(const osg::Vec3& center,float radius,float height ,osg::Vec3 nTo,bool useText2d=false);
     /** 生成圆锥体 **/
    osg::ref_ptr<osg::MatrixTransform> GetCone(const osg::Vec3& center,float radius,float height ,osg::Vec3 nTo);
    /** 获取轴旋转图形 **/
    osg::ref_ptr<osg::MatrixTransform> GetRoatationOfAxes(Ogive*);

};

#endif // SHAPENODEGENERATOR_H
