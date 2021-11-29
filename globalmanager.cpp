#include "globalmanager.h"
#include "texture2dmanager.h"
#include "materialmanager.h"

GlobalManager* GlobalManager::self = nullptr;
GlobalManager::GlobalManager()
{

}

GlobalManager* GlobalManager::GetInstance()
{
    if(self == nullptr)
    {
        self = new GlobalManager();
    }
    return self;
}

void GlobalManager::Init()
{
    Texture2DManager::GetInstance()->Init();
    MaterialManager::GetInstance()->Init();
}
