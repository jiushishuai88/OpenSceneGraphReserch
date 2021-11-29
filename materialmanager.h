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
* 描    述: 材质管理器
*
******************************************************************************/
#pragma execution_character_set("utf-8")
#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H


class MaterialManager
{
public:
    enum MaterialID
    {

    };
public:
    static MaterialManager* GetInstance();
    void Init();
private:
    MaterialManager();
    static MaterialManager* self;
};

#endif // MATERIALMANAGER_H
