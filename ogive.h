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
    /** 生成的三维模型**/
    osg::ref_ptr<osg::MatrixTransform> GetMt();
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
    eggData m_eggData;
    void Update();
    osg::Vec3 ComputeCircleCenter(const osg::Vec3&,const osg::Vec3&,const float&) const;
    osg::ref_ptr<osg::Vec3Array> GenerateCirclePoints(const osg::Vec3 &center,const float &radius,const int &points,const float &arcFrom ,const float &arcTo);
    osg::ref_ptr<osg::Vec3Array> GenerateCirclePoints(const osg::Vec3 &center,const osg::Vec3 &from,const osg::Vec3 &to,const int &points);
    ulong ComputePointsByRadius(float r,float arc);
    void SetData(const eggData&);
};

#endif // OGIVE_H
