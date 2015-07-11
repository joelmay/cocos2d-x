//
//  HVTrianglesCommand.h
//  cocos2d_libs
//
//  Created by Joel May on 7/10/15.
//
//

#ifndef __cocos2d_libs__HVTrianglesCommand__
#define __cocos2d_libs__HVTrianglesCommand__


#include "renderer/CCRenderCommand.h"
#include "renderer/CCGLProgramState.h"

/**
 * @addtogroup renderer
 * @{
 */

namespace hv {
    
    /** @struct V2F_C4F_T2F
     * A Vec2 with a vertex point, a tex coord point and a color 4F.
     */
    struct V2F_T2F_ACI
    {
        /// vertices (2F)
        cocos2d::Vec2       vertices;
        /// tex coords (2F)
        cocos2d::Tex2F          texCoords;
        float alpha;  // 0.0 - 1.0f
        float colorIndex; // will act as an int in shader.
    };
    
/**
 Command used to render one or more Triangles, which is similar to QuadCommand.
 Every TrianglesCommand will have generate material ID by give textureID, glProgramState, Blend function
 if the material id is the same, these TrianglesCommands could be batched to save draw call.
 */
class TrianglesCommand : public cocos2d::RenderCommand
{
public:
    /**The structure of Triangles. */
    struct Triangles
    {
        /**Vertex data pointer.*/
        V2F_T2F_ACI* verts;
        /**Index data pointer.*/
        unsigned short* indices;
        /**The number of vertices.*/
        ssize_t vertCount;
        /**The number of indices.*/
        ssize_t indexCount;
    };
    /**Construtor.*/
    TrianglesCommand();
    /**Destructor.*/
    ~TrianglesCommand();
    
    /** Initializes the command.
     @param globalOrder GlobalZOrder of the command.
     @param textureID The openGL handle of the used texture.
     @param glProgramState The specified glProgram and its uniform.
     @param blendType Blend function for the command.
     @param triangles Rendered triangles for the command.
     @param mv ModelView matrix for the command.
     @param flags to indicate that the command is using 3D rendering or not.
     */
    void init(float globalOrder, GLuint textureID,
              cocos2d::GLProgramState* glProgramState,
              cocos2d::BlendFunc blendType,
              const Triangles& triangles,
              const cocos2d::Mat4& mv,
              uint32_t flags);
    
    /**Deprecated function, the params is similar as the upper init function, with flags equals 0.*/
//    CC_DEPRECATED_ATTRIBUTE void init(float globalOrder, GLuint textureID,
//                                      cocos2d::GLProgramState* glProgramState,
//                                      cocos2d::BlendFunc blendType,
//                                      const Triangles& triangles,
//                                      const cocos2d::Mat4& mv);
    /**Apply the texture, shaders, programs, blend functions to GPU pipeline.*/
    void useMaterial() const;
    /**Get the material id of command.*/
    inline uint32_t getMaterialID() const { return _materialID; }
    /**Get the openGL texture handle.*/
    inline GLuint getTextureID() const { return _textureID; }
    /**Get a const reference of triangles.*/
    inline const Triangles& getTriangles() const { return _triangles; }
    /**Get the vertex count in the triangles.*/
    inline ssize_t getVertexCount() const { return _triangles.vertCount; }
    /**Get the index count of the triangles.*/
    inline ssize_t getIndexCount() const { return _triangles.indexCount; }
    /**Get the vertex data pointer.*/
    inline const V2F_T2F_ACI* getVertices() const { return _triangles.verts; }
    /**Get the index data pointer.*/
    inline const unsigned short* getIndices() const { return _triangles.indices; }
    /**Get the glprogramstate.*/
    inline cocos2d::GLProgramState* getGLProgramState() const { return _glProgramState; }
    /**Get the blend function.*/
    inline cocos2d::BlendFunc getBlendType() const { return _blendType; }
    /**Get the model view matrix.*/
    inline const cocos2d::Mat4& getModelView() const { return _mv; }
    
protected:
    /**Generate the material ID by textureID, glProgramState, and blend function.*/
    void generateMaterialID();
    
    /**Generated material id.*/
    uint32_t _materialID;
    /**OpenGL handle for texture.*/
    GLuint _textureID;
    /**GLprogramstate for the commmand. encapsulate shaders and uniforms.*/
    cocos2d::GLProgramState* _glProgramState;
    /**Blend function when rendering the triangles.*/
    cocos2d::BlendFunc _blendType;
    /**Rendered triangles.*/
    Triangles _triangles;
    /**Model view matrix when rendering the triangles.*/
    cocos2d::Mat4 _mv;
};

}; // end namespace hv
/**
 end of support group
 @}
 */

#endif /* defined(__cocos2d_libs__HVTrianglesCommand__) */
