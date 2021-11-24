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
* 创建日期: 2021.11
*
* 描    述: 纹理管理器
*
******************************************************************************/
#pragma execution_character_set("utf-8")
#ifndef TEXTURE2DMANAGER_H
#define TEXTURE2DMANAGER_H
#include<osg/Texture2D>

class Texture2DManager
{
public:
    enum T2DID
    {
        T2DID_NONE = 0,
        T2DID_TEST
    };
public:
    static Texture2DManager* GetInstance();
    osg::ref_ptr<osg::Texture2D> GetT2DByID(T2DID);
private:
    Texture2DManager();
    static Texture2DManager* m_pSelf;
};

#endif // TEXTURE2DMANAGER_H
