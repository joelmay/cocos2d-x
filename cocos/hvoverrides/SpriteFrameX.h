//
//  SpriteFrameX.h
//  cocos2d_libs
//
//  Created by Joel May on 7/19/15.
//
//

#ifndef __cocos2d_libs__SpriteFrameX__
#define __cocos2d_libs__SpriteFrameX__

#include "cocos2d.h"

namespace hv {
    
class SpriteFrameX : public cocos2d::SpriteFrame {
    
protected:
    std::string _name;
    int _intID;
    
public:
    
    static SpriteFrameX* createWithTexture(const std::string& name,
                                           int intID,
                                           cocos2d::Texture2D* pTexture,
                                           const cocos2d::Rect& rect,
                                           bool rotated,
                                           const cocos2d::Vec2& offset,
                                           const cocos2d::Size& originalSize);
    
    const std::string& getName (void) const { return _name ; }
    int                getIntID(void) const { return _intID; }
    
protected:
//CC_CONSTRUCTOR_ACCESS:
    SpriteFrameX() {}
    
    void initWithTexture(const std::string& name, int intID, cocos2d::Texture2D* pTexture)
    {
        _name = name;
        _intID = intID;
        setTexture(pTexture);
    }
    
    void initWithTexture(const std::string& name,
                         int intID,
                         cocos2d::Texture2D* pTexture,
                         const cocos2d::Rect& rect,
                         bool rotated,
                         const cocos2d::Vec2& offset,
                         const cocos2d::Size& originalSize);
    
};
    
}

#endif /* defined(__cocos2d_libs__SpriteFrameX__) */
