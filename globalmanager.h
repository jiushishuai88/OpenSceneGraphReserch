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
* 描    述: 全局管理器
*
******************************************************************************/
#pragma execution_character_set("utf-8")
#ifndef GLOBALMANAGER_H
#define GLOBALMANAGER_H


class GlobalManager
{
public:
    static GlobalManager* GetInstance();
    void Init();
private:
    GlobalManager();
    static GlobalManager* self;
};

#endif // GLOBALMANAGER_H
