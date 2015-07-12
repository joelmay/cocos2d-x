//
//  SpriteX.cpp
//  cocos2d_libs
//
//  Created by Joel May on 7/12/15.
//
//

#include "SpriteX.h"

using namespace hv;

SpriteX* SpriteX::create()
{
    SpriteX *sprite = new (std::nothrow) SpriteX();
    if (sprite && sprite->init())
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}



SpriteX* SpriteX::create(const std::string& filename)
{
    SpriteX *sprite = new (std::nothrow) SpriteX();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}


void SpriteX::setContentSize(const cocos2d::Size & size)
{
    if (! size.equals(_contentSize))
    {
        _contentSize = size;
        
        _anchorPointInPoints.set(_contentSize.width * _anchorPoint.x, _contentSize.height * _anchorPoint.y);
        _transformUpdated = _transformDirty = _inverseDirty = _contentSizeDirty = true;
    }
}

///////////////////////////////////////////////////////////////
// FIXME: HACK: optimization
// Copied from Sprite.cpp
#define SET_DIRTY_RECURSIVELY() {                       \
                    if (! _recursiveDirty) {            \
                        _recursiveDirty = true;         \
                        setDirty(true);                 \
                        if (!_children.empty())         \
                            setDirtyRecursively(true);  \
                        }                               \
                    }
///////////////////////////////////////////////////////////////

void SpriteX::setAnchorPoint(const cocos2d::Vec2& anchor)
{
    if (this->_spriteFrame == nullptr){
        // Not part of an atlas
        cocos2d::Sprite::setAnchorPoint(anchor);
    }
    else{
        
        if (! anchor.equals(_anchorPoint))
        {
            auto contentSize = this->_spriteFrame->getRect().size;
            _anchorPoint = anchor;
            _anchorPointInPoints.set(contentSize.width * _anchorPoint.x, contentSize.height * _anchorPoint.y);
            _transformUpdated = _transformDirty = _inverseDirty = true;
            SET_DIRTY_RECURSIVELY();
        }
    }
}
