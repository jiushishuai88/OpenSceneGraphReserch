/******************************************************************************
*
* Copyright (c) 20XX-20XX, XXXXXXXX有限公司
*
* All rights reserved.
*
* 文 件 名: ogive.h
*
* 作    者: guchuan
*
* 创建日期: 2021.11
*
* 描    述: 生成拱形
*
******************************************************************************/
#pragma execution_character_set("utf-8")

#ifndef OGIVE_H
#define OGIVE_H
#include<QtCore>
#include<osg/MatrixTransform>
#include"datadefine.h"

class Ogive
{
public:
    Ogive();
    Ogive(eggData);
    /** 获取初始法向量 **/
    osg::Vec3 GetBaseNormal()const;
    /** 参数data **/
    eggData GetEggData() const;
    /** 轴旋转体圆形粒度**/
    ulong GetCirclePoints() const;
    /** 外轮廓线 **/
    osg::ref_ptr<osg::Vec3Array> GetExtArcArray() const;
    /** 内轮廓线**/
    osg::ref_ptr<osg::Vec3Array> GetInnerArcArray() const;
    /** 获取三维几何**/
    osg::ref_ptr<osg::Geometry> GetGeometry();
    /** 设置原始数据 **/
    void SetData(const eggData&);
private:
    ulong m_arcPoints;
    ulong m_circlePoints;
    /** 外圆弧半径 **/
    float m_extArcR;
    /** 内圆弧半径 **/
    float m_innerArcR;
    const osg::Vec3 m_baseNormal;
    osg::Vec3 m_extPointUp;
    osg::Vec3 m_extPointDown;
    osg::Vec3 m_innerPointUp;
    osg::Vec3 m_innerPointDown;
    osg::Vec3 m_extCenter;
    osg::Vec3 m_innerCenter;
    osg::ref_ptr<osg::Vec3Array> m_pExtArray;
    osg::ref_ptr<osg::Vec3Array> m_pInnerArray;
    osg::ref_ptr<osg::Geometry> m_pGeom;
    eggData m_eggData;
    void Update();
    /** 根据两点和弧度计算圆心（逆时针方向）**/
    osg::Vec3 ComputeCircleCenter(const osg::Vec3&,const osg::Vec3&,const float&) const;
    /** 生成弧线 **/
    osg::ref_ptr<osg::Vec3Array> GenerateArcPoints(const osg::Vec3 &center,const float &radius,const int &points,const float &arcFrom ,const float &arcTo);
    /** 生成弧线 **/
    osg::ref_ptr<osg::Vec3Array> GenerateArcPoints(const osg::Vec3 &center,const osg::Vec3 &from,const osg::Vec3 &to,const int &points);
    /** 计算粒度 **/
    ulong ComputePointsByRadius(float r,float arc);
    /** 连接成面 **/
    void GenerateSurface(osg::Geometry* geom,int* index1,int* index2,int col);
    /** 首尾连接成环形数据 **/
    void LoopPoints( osg::ref_ptr<osg::Vec3Array> pArr);
     /** 几何变换 **/
    void V3ArrayTransform(osg::Vec3Array* pArr,const osg::Matrixd& mat);
    /** 生成圆形 **/
    osg::ref_ptr<osg::Vec3Array> GenerateCircle(float radius,int points);
    /** 生成几何体 **/
    void GenerateGeom();
};

#endif // OGIVE_H
