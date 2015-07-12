//
//  SpriteX.h
//  cocos2d_libs
//
//  Created by Joel May on 7/12/15.
//
//

#ifndef __cocos2d_libs__SpriteX__
#define __cocos2d_libs__SpriteX__

#include "cocos2d.h"

namespace hv {
    
class SpriteX : public cocos2d::Sprite {
    
    
public:
    static SpriteX* create();
    static SpriteX* create(const std::string& filename);
    
    virtual void setAnchorPoint(const cocos2d::Vec2& anchor) override;
    
    virtual void setContentSize(const cocos2d::Size & size) override;
};
    
}

#endif /* defined(__cocos2d_libs__SpriteX__) */
