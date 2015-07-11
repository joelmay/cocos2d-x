//
//  HVRenderer.cpp
//  cocos2d_libs
//
//  Created by Joel May on 7/9/15.
//
//

#include "HVRenderer.h"


using namespace hv;
using namespace cocos2d;
    
hv::Renderer::Renderer(void)
{
    printf("hv renderer");
}


void hv::Renderer::visitRenderQueue(RenderQueue& queue)
{
    queue.saveRenderState();
    
    //
    //Process Global-Z < 0 Objects
    //
//    const auto& zNegQueue = queue.getSubQueue(RenderQueue::QUEUE_GROUP::GLOBALZ_NEG);
//    if (zNegQueue.size() > 0)
//    {
//        if(_isDepthTestFor2D)
//        {
//            glEnable(GL_DEPTH_TEST);
//            glDepthMask(true);
//            RenderState::StateBlock::_defaultState->setDepthTest(true);
//            RenderState::StateBlock::_defaultState->setDepthWrite(true);
//        }
//        else
//        {
//            glDisable(GL_DEPTH_TEST);
//            glDepthMask(false);
//            RenderState::StateBlock::_defaultState->setDepthTest(false);
//            RenderState::StateBlock::_defaultState->setDepthWrite(false);
//        }
//        for (auto it = zNegQueue.cbegin(); it != zNegQueue.cend(); ++it)
//        {
//            processRenderCommand(*it);
//        }
//        flush();
//    }
//    
    //
    //Process Opaque Object
    //
//    const auto& opaqueQueue = queue.getSubQueue(RenderQueue::QUEUE_GROUP::OPAQUE_3D);
//    if (opaqueQueue.size() > 0)
//    {
//        //Clear depth to achieve layered rendering
//        glEnable(GL_DEPTH_TEST);
//        glDepthMask(true);
//        RenderState::StateBlock::_defaultState->setDepthTest(true);
//        RenderState::StateBlock::_defaultState->setDepthWrite(true);
//        
//        
//        for (auto it = opaqueQueue.cbegin(); it != opaqueQueue.cend(); ++it)
//        {
//            processRenderCommand(*it);
//        }
//        flush();
//    }
//    
    //
    //Process 3D Transparent object
    //
//    const auto& transQueue = queue.getSubQueue(RenderQueue::QUEUE_GROUP::TRANSPARENT_3D);
//    if (transQueue.size() > 0)
//    {
//        glEnable(GL_DEPTH_TEST);
//        glDepthMask(false);
//        
//        RenderState::StateBlock::_defaultState->setDepthTest(true);
//        RenderState::StateBlock::_defaultState->setDepthWrite(false);
//        
//        
//        for (auto it = transQueue.cbegin(); it != transQueue.cend(); ++it)
//        {
//            processRenderCommand(*it);
//        }
//        flush();
//    }
//    
    //
    //Process Global-Z = 0 Queue
    //
    const auto& zZeroQueue = queue.getSubQueue(RenderQueue::QUEUE_GROUP::GLOBALZ_ZERO);
    if (zZeroQueue.size() > 0)
    {
        if(_isDepthTestFor2D)
        {
            glEnable(GL_DEPTH_TEST);
            glDepthMask(true);
            
            RenderState::StateBlock::_defaultState->setDepthTest(true);
            RenderState::StateBlock::_defaultState->setDepthWrite(true);
            
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
            glDepthMask(false);
            
            RenderState::StateBlock::_defaultState->setDepthTest(false);
            RenderState::StateBlock::_defaultState->setDepthWrite(false);
            
        }
        for (auto it = zZeroQueue.cbegin(); it != zZeroQueue.cend(); ++it)
        {
            processRenderCommand(*it);
        }
        flush();
    }
    
    //
    //Process Global-Z > 0 Queue
    //
    const auto& zPosQueue = queue.getSubQueue(RenderQueue::QUEUE_GROUP::GLOBALZ_POS);
    if (zPosQueue.size() > 0)
    {
        for (auto it = zPosQueue.cbegin(); it != zPosQueue.cend(); ++it)
        {
            processRenderCommand(*it);
        }
        flush();
    }
    
    queue.restoreRenderState();
}

void hv::Renderer::processRenderCommand(RenderCommand* command)
{
    auto commandType = command->getType();
    if( RenderCommand::Type::TRIANGLES_COMMAND == commandType)
    {
        //Draw if we have batched other commands which are not triangle command
        flush3D();
        flushQuads();
        
        //Process triangle command
        auto cmd = static_cast<TrianglesCommand*>(command);
        
        //Draw batched Triangles if necessary
        if(cmd->isSkipBatching() || _filledVertex + cmd->getVertexCount() > VBO_SIZE || _filledIndex + cmd->getIndexCount() > INDEX_VBO_SIZE)
        {
            CCASSERT(cmd->getVertexCount()>= 0 && cmd->getVertexCount() < VBO_SIZE, "VBO for vertex is not big enough, please break the data down or use customized render command");
            CCASSERT(cmd->getIndexCount()>= 0 && cmd->getIndexCount() < INDEX_VBO_SIZE, "VBO for index is not big enough, please break the data down or use customized render command");
            //Draw batched Triangles if VBO is full
            drawBatchedTriangles();
        }
        
        //Batch Triangles
        _batchedCommands.push_back(cmd);
        
        fillVerticesAndIndices(cmd);
        
        if(cmd->isSkipBatching())
        {
            drawBatchedTriangles();
        }
        
    }
    else if ( RenderCommand::Type::QUAD_COMMAND == commandType )
    {
        //Draw if we have batched other commands which are not quad command
        flush3D();
        flushTriangles();
        
        //Process quad command
        auto cmd = static_cast<QuadCommand*>(command);
        
        //Draw batched quads if necessary
        if(cmd->isSkipBatching()|| (_numberQuads + cmd->getQuadCount()) * 4 > VBO_SIZE )
        {
            CCASSERT(cmd->getQuadCount()>= 0 && cmd->getQuadCount() * 4 < VBO_SIZE, "VBO for vertex is not big enough, please break the data down or use customized render command");
            //Draw batched quads if VBO is full
            drawBatchedQuads();
        }
        
        //Batch Quads
        _batchQuadCommands.push_back(cmd);
        
        fillQuads(cmd);
        
        if(cmd->isSkipBatching())
        {
            drawBatchedQuads();
        }
    }
    else if (RenderCommand::Type::MESH_COMMAND == commandType)
    {
        flush2D();
        auto cmd = static_cast<MeshCommand*>(command);
        
        if (cmd->isSkipBatching() || _lastBatchedMeshCommand == nullptr || _lastBatchedMeshCommand->getMaterialID() != cmd->getMaterialID())
        {
            flush3D();
            
            if(cmd->isSkipBatching())
            {
                // XXX: execute() will call bind() and unbind()
                // but unbind() shouldn't be call if the next command is a MESH_COMMAND with Material.
                // Once most of cocos2d-x moves to Pass/StateBlock, only bind() should be used.
                cmd->execute();
            }
            else
            {
                cmd->preBatchDraw();
                cmd->batchDraw();
                _lastBatchedMeshCommand = cmd;
            }
        }
        else
        {
            cmd->batchDraw();
        }
    }
    else if(RenderCommand::Type::GROUP_COMMAND == commandType)
    {
        flush();
        int renderQueueID = ((GroupCommand*) command)->getRenderQueueID();
        visitRenderQueue(_renderGroups[renderQueueID]);
    }
    else if(RenderCommand::Type::CUSTOM_COMMAND == commandType)
    {
        flush();
        auto cmd = static_cast<CustomCommand*>(command);
        cmd->execute();
    }
    else if(RenderCommand::Type::BATCH_COMMAND == commandType)
    {
        flush();
        auto cmd = static_cast<BatchCommand*>(command);
        cmd->execute();
    }
    else if(RenderCommand::Type::PRIMITIVE_COMMAND == commandType)
    {
        flush();
        auto cmd = static_cast<PrimitiveCommand*>(command);
        cmd->execute();
    }
    else
    {
        CCLOGERROR("Unknown commands in renderQueue");
    }
}

void hv::Renderer::fillVerticesAndIndices(const TrianglesCommand* cmd)
{
    memcpy(_verts + _filledVertex, cmd->getVertices(), sizeof(V3F_C4B_T2F) * cmd->getVertexCount());
    const Mat4& modelView = cmd->getModelView();
    
    for(ssize_t i=0; i< cmd->getVertexCount(); ++i)
    {
        V3F_C4B_T2F *q = &_verts[i + _filledVertex];
        Vec3 *vec1 = (Vec3*)&q->vertices;
        modelView.transformPoint(vec1);
    }
    
    const unsigned short* indices = cmd->getIndices();
    //fill index
    for(ssize_t i=0; i< cmd->getIndexCount(); ++i)
    {
        _indices[_filledIndex + i] = _filledVertex + indices[i];
    }
    
    _filledVertex += cmd->getVertexCount();
    _filledIndex += cmd->getIndexCount();
}

void hv::Renderer::drawBatchedTriangles()
{
    //TODO: we can improve the draw performance by insert material switching command before hand.
    
    int indexToDraw = 0;
    int startIndex = 0;
    
    //Upload buffer to VBO
    if(_filledVertex <= 0 || _filledIndex <= 0 || _batchedCommands.empty())
    {
        return;
    }
    
    if (Configuration::getInstance()->supportsShareableVAO())
    {
        //Bind VAO
        GL::bindVAO(_buffersVAO);
        //Set VBO data
        glBindBuffer(GL_ARRAY_BUFFER, _buffersVBO[0]);
        
        // option 1: subdata
        //        glBufferSubData(GL_ARRAY_BUFFER, sizeof(_quads[0])*start, sizeof(_quads[0]) * n , &_quads[start] );
        
        // option 2: data
        //        glBufferData(GL_ARRAY_BUFFER, sizeof(quads_[0]) * (n-start), &quads_[start], GL_DYNAMIC_DRAW);
        
        // option 3: orphaning + glMapBuffer
        glBufferData(GL_ARRAY_BUFFER, sizeof(_verts[0]) * _filledVertex, nullptr, GL_DYNAMIC_DRAW);
        void *buf = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        memcpy(buf, _verts, sizeof(_verts[0])* _filledVertex);
        glUnmapBuffer(GL_ARRAY_BUFFER);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffersVBO[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices[0]) * _filledIndex, _indices, GL_STATIC_DRAW);
    }
    else
    {
#define kQuadSize sizeof(_verts[0])
        glBindBuffer(GL_ARRAY_BUFFER, _buffersVBO[0]);
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(_verts[0]) * _filledVertex , _verts, GL_DYNAMIC_DRAW);
        
        GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
        
        // vertices
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*) offsetof(V3F_C4B_T2F, vertices));
        
        // colors
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (GLvoid*) offsetof(V3F_C4B_T2F, colors));
        
        // tex coords
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*) offsetof(V3F_C4B_T2F, texCoords));
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffersVBO[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices[0]) * _filledIndex, _indices, GL_STATIC_DRAW);
    }
    
    //Start drawing verties in batch
    for(const auto& cmd : _batchedCommands)
    {
        auto newMaterialID = cmd->getMaterialID();
        if(_lastMaterialID != newMaterialID || newMaterialID == MATERIAL_ID_DO_NOT_BATCH)
        {
            //Draw quads
            if(indexToDraw > 0)
            {
                glDrawElements(GL_TRIANGLES, (GLsizei) indexToDraw, GL_UNSIGNED_SHORT, (GLvoid*) (startIndex*sizeof(_indices[0])) );
                _drawnBatches++;
                _drawnVertices += indexToDraw;
                
                startIndex += indexToDraw;
                indexToDraw = 0;
            }
            
            //Use new material
            cmd->useMaterial();
            _lastMaterialID = newMaterialID;
        }
        
        indexToDraw += cmd->getIndexCount();
    }
    
    //Draw any remaining triangles
    if(indexToDraw > 0)
    {
        glDrawElements(GL_TRIANGLES, (GLsizei) indexToDraw, GL_UNSIGNED_SHORT, (GLvoid*) (startIndex*sizeof(_indices[0])) );
        _drawnBatches++;
        _drawnVertices += indexToDraw;
    }
    
    if (Configuration::getInstance()->supportsShareableVAO())
    {
        //Unbind VAO
        GL::bindVAO(0);
    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    
    _batchedCommands.clear();
    _filledVertex = 0;
    _filledIndex = 0;
}

