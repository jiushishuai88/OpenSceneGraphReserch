#include "materialmanager.h"

MaterialManager* MaterialManager::self = nullptr;
MaterialManager::MaterialManager()
{

}

MaterialManager* MaterialManager::GetInstance()
{
    if(self == nullptr)
    {
        self = new MaterialManager();
    }
    return self;
}

void MaterialManager::Init()
{

}
