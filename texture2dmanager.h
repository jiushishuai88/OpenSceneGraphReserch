#ifndef TEXTURE2DMANAGER_H
#define TEXTURE2DMANAGER_H
#include<osg/Texture2D>

class Texture2DManager
{
public:
    enum T2DID
    {
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
