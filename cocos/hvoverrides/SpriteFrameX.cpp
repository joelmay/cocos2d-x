//
//  SpriteFrameX.cpp
//  cocos2d_libs
//
//  Created by Joel May on 7/19/15.
//
//

#include "SpriteFrameX.h"

using namespace hv;
using namespace cocos2d;


SpriteFrameX* SpriteFrameX::createWithTexture(const std::string& name,
                                           int intID,
                                           cocos2d::Texture2D* pTexture,
                                           const cocos2d::Rect& rect,
                                           bool rotated,
                                           const cocos2d::Vec2& offset,
                                           const cocos2d::Size& originalSize)
{
    SpriteFrameX *spriteFrame = new (std::nothrow) SpriteFrameX();
    spriteFrame->initWithTexture(name, intID, pTexture, rect, rotated, offset, rect.size);
    spriteFrame->autorelease();
    
    return spriteFrame;
}

void SpriteFrameX::initWithTexture(const std::string& name,
                                   int intID,
                                   cocos2d::Texture2D* pTexture,
                                   const cocos2d::Rect& rect,
                                   bool rotated,
                                   const cocos2d::Vec2& offset,
                                   const cocos2d::Size& originalSize)
{

    SpriteFrame::initWithTexture(pTexture, rect, rotated, offset, originalSize);
    
    _name = name;
    _intID = intID;
}