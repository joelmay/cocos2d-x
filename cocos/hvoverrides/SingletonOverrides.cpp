//
//  SingletonOverrides.cpp
//  cocos2d_libs
//
//  Created by Joel May on 7/8/15.
//
//

#include "SingletonOverrides.h"
#include "HVDirector.h"
#include "cocos2d.h"

using namespace hv;

SingletonOverrides* SingletonOverrides::getInstance()
{
    static SingletonOverrides* _gInstance = nullptr;
    
    if (_gInstance == nullptr){
        _gInstance = new SingletonOverrides();
    }
    return _gInstance;
}

void SingletonOverrides::createAll(void)
{
    // Force hv::Director to be created
    hv::Director::getInstance();
}