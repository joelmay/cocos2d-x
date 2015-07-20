//
//  NestedSpriteFrame.h
//  cocos2d_libs
//
//  Created by Joel May on 7/19/15.
//
//

#ifndef __cocos2d_libs__NestedSpriteFrame__
#define __cocos2d_libs__NestedSpriteFrame__

//#include "rapidxml.hpp"
#include "cocos2d.h"
#include "SpriteFrameX.h"

namespace hv {
    
class NestedSpriteFrame : public SpriteFrameX {
    
protected:
    cocos2d::Vector<SpriteFrameX*> _spriteFrames;
    
    std::vector<std::string>   _subItemNames;
    std::vector<int>           _subItemIntIDs;
    std::vector<cocos2d::Mat4> _subItemMatrices;  // TODO: convert this to a 3x3 matrix
    std::vector<float>         _subItemAlphas;
    
public:
    static NestedSpriteFrame* createWithTexture(const std::string& name,
                                                int intID,
                                                cocos2d::Texture2D* pTexture
                                                );
    
protected:
//CC_CONSTRUCTOR_ACCESS:
    NestedSpriteFrame() {}
    
    void initWithTexture(const std::string& name,
                         int intID,
                         cocos2d::Texture2D* pTexture
                         );
public:
    
    //void addChild(SpriteFrameX* pChild, cocos2d::Mat4& transformMatrix, float alpha);
    void addChild(const std::string& name, int intID, cocos2d::Mat4& transformMatrix, float alpha)
    {
        _subItemNames   .push_back(name);
        _subItemIntIDs  .push_back(intID);
        _subItemMatrices.push_back(transformMatrix);
        _subItemAlphas  .push_back(alpha);
    }
    
};
    
}

#endif /* defined(__cocos2d_libs__NestedSpriteFrame__) */
