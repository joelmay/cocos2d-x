//
//  SpriteBatchNodeX.cpp
//  cocos2d_libs
//
//  Created by Joel May on 7/10/15.
//
//

#include "SpriteBatchNodeX.h"

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
    //_customCommand.func = CC_CALLBACK_0(executeRenderCommand, this, transform, transformUpdated);
    _customCommand.func = CC_CALLBACK_0(hv::SpriteBatchNodeX::executeRenderCommand, this, transform, flags);
    //_customCommand.func = CC_CALLBACK_0(hv::SpriteBatchNodeX::executeRenderCommand, this, transform, transformUpdated);
    
    renderer->addCommand(&_customCommand);
    
    
//    
////    SpriteBatchNode::draw(renderer, transform, flags);
////    return;
//
//    // Don't do calculate the culling if the transform was not updated
//    bool transformUpdated = flags & FLAGS_TRANSFORM_DIRTY;
//#if CC_USE_CULLING
//    bool bInsideBounds = transformUpdated ? renderer->checkVisibility(transform, _contentSize) : _insideBounds;
//    
//    if(bInsideBounds)
//#endif
//    {
//        _customCommand.init(_globalZOrder, transform, flags);
//        //_customCommand.func = CC_CALLBACK_0(executeRenderCommand, this, transform, transformUpdated);
//        _customCommand.func = CC_CALLBACK_0(hv::SpriteBatchNodeX::executeRenderCommand, this, transform, transformUpdated);
//        
//        renderer->addCommand(&_customCommand);
//    }
}


void hv::SpriteBatchNodeX::executeRenderCommand(const cocos2d::Mat4& transform, bool transformUpdated)
{
    auto program = getGLProgram();
    program->use();
    program->setUniformsForBuiltins(transform);
    //program->setUniformsForBuiltins(_mv);
    
    auto textureID = _textureAtlas->getTexture()->getName();
    
    cocos2d::GL::bindTexture2D(textureID);
    cocos2d::GL::blendFunc(_blendFunc.src, _blendFunc.dst);
    //cocos2d::GL::blendFunc(_blendType.src, _blendType.dst);
    
    // Draw
    _textureAtlas->drawQuads();
    
    
//    
//    
//    // Optimization: Fast Dispatch
//    if( _textureAtlas == NULL || (_batchNodes.size() == 1 && _textureAtlas->getTotalQuads() == 0) )
//    {
//        return;
//    }
//    
//    auto glprogram = getGLProgram();
//    glprogram->use();
//    cocos2d::GL::blendFunc( _blendFunc.src, _blendFunc.dst );
//    
//    glprogram->setUniformsForBuiltins(transform);
//    for(const auto &child: _children)
//    {
//        child->updateTransform();
//    }
//    
//    
//    for (const auto& batchNode:_batchNodes)
//    {
//        batchNode->getTextureAtlas()->drawQuads();
//    }
}


