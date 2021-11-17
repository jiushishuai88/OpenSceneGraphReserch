#include "texture2dmanager.h"
#include<osgDB/ReadFile>

Texture2DManager* Texture2DManager::m_pSelf = nullptr;
Texture2DManager::Texture2DManager()
{

}

 Texture2DManager* Texture2DManager::GetInstance()
 {
     if(m_pSelf == nullptr)
     {
         m_pSelf = new Texture2DManager();
     }
     return m_pSelf;
 }

 osg::ref_ptr<osg::Texture2D> Texture2DManager::GetT2DByID(T2DID id)
 {
     switch (id) {
     case T2DID_TEST:
        osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
         texture->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::WrapMode::CLAMP_TO_BORDER);//贴图填充的方式
         texture->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::WrapMode::CLAMP_TO_BORDER);//贴图填充的方式
         texture->setDataVariance(osg::Object::DYNAMIC); // protect from being optimized away as static state.
         texture->setImage(osgDB::readRefImageFile("D:/WorkSpace/624/OsgOpenGL/OsgOpenGl/texture2d/wenli.png"));
         return texture;
         break;
     }
     return nullptr;
 }
