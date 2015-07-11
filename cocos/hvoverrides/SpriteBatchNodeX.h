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
    
protected:
    void onDraw(const cocos2d::Mat4& transform, bool transformUpdated);
    
public:
    SpriteBatchNodeX(void) : _insideBounds(false) {}
    
    void executeRenderCommand(const cocos2d::Mat4& transform, bool transformUpdated);
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
};
    
}

#endif /* defined(__cocos2d_libs__SpriteBatchNodeX__) */
