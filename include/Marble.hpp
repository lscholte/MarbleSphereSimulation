#ifndef Marble_hpp
#define Marble_hpp

#include <atlas/utils/Geometry.hpp>
#include <atlas/utils/Mesh.hpp>
#include <vector>

class Marble : public atlas::utils::Geometry
{
    public:
        Marble();
        ~Marble();

        void renderGeometry(atlas::math::Matrix4 const &projection, atlas::math::Matrix4 const &view) override;	    
        
        float getRadius() const;
        void setModel(glm::mat4 const &model);        

    private:

        GLuint mVao;
        GLuint mPositionBuffer, mTextureBuffer, mIndexBuffer;
        GLuint mTextureId;

        atlas::utils::Mesh mMesh;

        float mRadius;
};

#endif