//
//  NestedSpriteFrame.cpp
//  cocos2d_libs
//
//  Created by Joel May on 7/19/15.
//
//

#include "NestedSpriteFrame.h"

using namespace hv;
using namespace cocos2d;

NestedSpriteFrame* NestedSpriteFrame::createWithTexture(const std::string& name,
                                                        int intID,
                                                        cocos2d::Texture2D* pTexture
                                                        )
{
    NestedSpriteFrame *spriteFrame = new (std::nothrow) NestedSpriteFrame();
    spriteFrame->initWithTexture(name, intID, pTexture);
    spriteFrame->autorelease();
    
    return spriteFrame;
}

// TODO: Maybe remove this.
void NestedSpriteFrame::initWithTexture(const std::string& name,
                                        int intID,
                                        cocos2d::Texture2D* pTexture
                                        )
{
    SpriteFrameX::initWithTexture(name, intID, pTexture);
}
