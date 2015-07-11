//
//  HVDirector.cpp
//  cocos2d_libs
//
//  Created by Joel May on 7/8/15.
//
//

#include "HVDirector.h"
#include "HVRenderer.h"

using namespace hv;

hv::Director::Director(void)
{
    //CCLOG("hv::Director");
    printf("hv::director");
}
    
hv::Director* Director::getInstance()
{
    static hv::Director* dir = nullptr;
    
    if (dir == nullptr){
        dir = new hv::Director();
        setInstance(dir);
        dir->init();
    }
    
    return dir;
}

//////////////////////////////////////////////////////////
// HVHack: Start:  Added
cocos2d::Renderer* Director::createRenderer(void)
{
    return new (std::nothrow) hv::Renderer();
}
// HVHack: End:
//////////////////////////////////////////////////////////