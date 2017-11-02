#ifndef MarbleBowl_hpp
#define MarbleBowl_hpp

#include "Marble.hpp"
#include "Bowl.hpp"
#include <atlas/utils/Geometry.hpp>

class MarbleBowl : public atlas::utils::Geometry
{
    public:
        MarbleBowl();
        ~MarbleBowl();

        void renderGeometry(atlas::math::Matrix4 const &projection, atlas::math::Matrix4 const &view) override;
        void updateGeometry(atlas::core::Time<> const &t) override;
        void drawGui() override;        

        void applyTransformations();
    
    private:
        Marble mMarble;
        Bowl mBowl;

        //mTheta1 = angular position about z axis
        //mTheta2 = angular position about y axis
        float mTheta1, mTheta2;

        //mTheta1Dot = angular velocity about z axis
        //mTheta2Dot = angular velocity about y axis
        float mTheta1Dot, mTheta2Dot;

        //mTheta1DotDot = angular acceleration about z axis
        //mTheta2DotDot = angular acceleration about y axis
        float mTheta1DotDot, mTheta2DotDot;

        //mPhi1 = marble rotation about z axis
        //mPhi2 = marble rotation about y axis
        float mPhi1, mPhi2;

        float mMarbleRotationAngle;
        glm::vec3 mMarbleRotationAxis;

        glm::mat4 mRotationMatrix;

};

#endif