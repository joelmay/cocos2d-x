//
//  SpriteBatchNodeX.h
//  cocos2d_libs
//
//  Created by Joel May on 7/10/15.
//
//

#ifndef __cocos2d_libs__SpriteBatchNodeX__
#define __cocos2d_libs__SpriteBatchNodeX__

#include "cocos2d.h"

namespace hv {

class SpriteBatchNodeX : public cocos2d::SpriteBatchNode {
    
private:
    bool _insideBounds;
    cocos2d::CustomCommand _customCommand;
    
    cocos2d::Mat4 _colorMatrix;
    
protected:
    void onDraw(const cocos2d::Mat4& transform, bool transformUpdated);
    
public:
    static SpriteBatchNodeX* create(const std::string& fileImage, ssize_t capacity/* = DEFAULT_CAPACITY*/);
    
    SpriteBatchNodeX(void);
    
    void executeRenderCommand(const cocos2d::Mat4& transform, bool transformUpdated);
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
};
    
}

#endif /* defined(__cocos2d_libs__SpriteBatchNodeX__) */
