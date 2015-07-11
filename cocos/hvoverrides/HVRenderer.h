//
//  HVRenderer.h
//  cocos2d_libs
//
//  Created by Joel May on 7/9/15.
//
//

#ifndef __cocos2d_libs__HVRenderer__
#define __cocos2d_libs__HVRenderer__

#include "cocos2d.h"

namespace hv {


class Renderer : public cocos2d::Renderer {
    
public:
    Renderer(void);
    
protected:
    virtual void visitRenderQueue(cocos2d::RenderQueue& queue) override;
    virtual void processRenderCommand(cocos2d::RenderCommand* command) override;
    
    virtual void fillVerticesAndIndices(const cocos2d::TrianglesCommand* cmd) override;
    
    virtual void drawBatchedTriangles() override;
};
}


#endif /* defined(__cocos2d_libs__HVRenderer__) */
