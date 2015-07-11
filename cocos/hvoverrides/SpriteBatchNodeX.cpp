//
//  SpriteBatchNodeX.cpp
//  cocos2d_libs
//
//  Created by Joel May on 7/10/15.
//
//

#include "SpriteBatchNodeX.h"

/*
 * creation with File Image
 */

hv::SpriteBatchNodeX* hv::SpriteBatchNodeX::create(const std::string& fileImage, ssize_t capacity/* = DEFAULT_CAPACITY*/)
{
    hv::SpriteBatchNodeX *batchNode = new (std::nothrow) hv::SpriteBatchNodeX();
    batchNode->initWithFile(fileImage, capacity);
    batchNode->autorelease();
    
    return batchNode;
}

hv::SpriteBatchNodeX::SpriteBatchNodeX(void) :
    _insideBounds(false)
{
}

void hv::SpriteBatchNodeX::draw(
                cocos2d::Renderer*   renderer,
                const cocos2d::Mat4& transform,
                uint32_t             flags
)
{
    // Optimization: Fast Dispatch
    if( _textureAtlas->getTotalQuads() == 0 )
    {
        return;
    }
    
    for (const auto &child : _children)
    {
#if CC_USE_PHYSICS
        auto physicsBody = child->getPhysicsBody();
        if (physicsBody)
        {
            child->updateTransformFromPhysics(transform, flags);
        }
#endif
        child->updateTransform();
    }
    
    _customCommand.init(_globalZOrder, transform, flags);
    _customCommand.func = CC_CALLBACK_0(hv::SpriteBatchNodeX::executeRenderCommand, this, transform, flags);
    
    renderer->addCommand(&_customCommand);
}


void hv::SpriteBatchNodeX::executeRenderCommand(const cocos2d::Mat4& transform, bool transformUpdated)
{
    auto program = getGLProgram();
    program->use();
    program->setUniformsForBuiltins(transform);
    
    auto programState = cocos2d::GLProgramState::getOrCreateWithGLProgram(program);
    programState->apply(transform);
    
    
    auto textureID = _textureAtlas->getTexture()->getName();
    
    cocos2d::GL::bindTexture2D(textureID);
    cocos2d::GL::blendFunc(_blendFunc.src, _blendFunc.dst);
    
   
    // Draw
    _textureAtlas->drawQuads();
}


