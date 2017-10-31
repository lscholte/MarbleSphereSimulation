#ifndef Bowl_hpp
#define Bowl_hpp

#include <atlas/utils/Geometry.hpp>
#include <atlas/utils/Mesh.hpp>
#include <vector>

class Bowl : public atlas::utils::Geometry
{
    public:
        Bowl();
        Bowl(float radius);        
        ~Bowl();

        void renderGeometry(atlas::math::Matrix4 const &projection, atlas::math::Matrix4 const &view) override;	    
        
        float getRadius() const;
        void setModel(glm::mat4 const &model);        

    private:

        GLuint mVao;
        GLuint mPositionBuffer, mColorBuffer, mIndexBuffer;
        GLuint mTextureId;

        atlas::utils::Mesh mMesh;
        std::vector<glm::vec3> mColors;

        float mRadius;
};

#endif