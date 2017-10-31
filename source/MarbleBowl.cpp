#include "MarbleBowl.hpp"

MarbleBowl::MarbleBowl() :
    mBowl(5.0f),
    mTheta1(3.1415926f/2.0f),
    mTheta2(0.0f),
    mTheta1Dot(0.0f),
    mTheta2Dot(3.1415926f/2.0f), //half a circumference per second
    mTheta1DotDot(0.0f),
    mTheta2DotDot(0.0f),
    mPhi1(0.0f),
    mPhi2(0.0f)
{
    this->applyTransformations();    
}

MarbleBowl::~MarbleBowl()
{
}

void MarbleBowl::renderGeometry(atlas::math::Matrix4 const &projection, atlas::math::Matrix4 const &view)
{
    mMarble.renderGeometry(projection, view);
    mBowl.renderGeometry(projection, view);
}

void MarbleBowl::updateGeometry(atlas::core::Time<> const &t)
{	

    //Compute the angular accelerations of the marble around the bowl
    mTheta1DotDot = -(9.81*sin(mTheta1)) / mBowl.getRadius();
    // mTheta1DotDot = 0.0;
    mTheta2DotDot = 0.0;    

    //Compute the angular velocities of the marble around the bowl
    mTheta1Dot = mTheta1Dot + mTheta1DotDot*t.deltaTime;
    mTheta2Dot = mTheta2Dot + mTheta2DotDot*t.deltaTime;    

    //Compute the angular positions of the marble around the bowl
    mTheta1 = mTheta1 + mTheta1Dot*t.deltaTime;
    mTheta2 = mTheta2 + mTheta2Dot*t.deltaTime;    

    //Using the contstraint equations,
    //compute the rotation of the marble
    mPhi1 = (mBowl.getRadius() / mMarble.getRadius()) * mTheta1;
    mPhi2 = (mBowl.getRadius() / mMarble.getRadius()) * mTheta2;

	this->applyTransformations();
}

void MarbleBowl::applyTransformations()
{
	//Resets the model matrices so that we can re-apply scaling and/or translations to them
	mMarble.setModel(glm::mat4(1.0f));
    

    //Rotate marble to its correct orientation
    mMarble.transformGeometry(
        glm::rotate(
            glm::mat4(1.0f),
            mPhi1,
            glm::vec3(0.0f, 0.0f, -1.0f)
        )
    );

    mMarble.transformGeometry(
        glm::rotate(
            glm::mat4(1.0f),
            mPhi2,
            glm::vec3(0.0f, 1.0f, 0.0f)
        )
    );

    //Translate marble to its correct position in the bowl
    mMarble.transformGeometry(
        glm::translate(
            glm::mat4(1.0f),
            //subtract radius of marble so that marble isn't sticking through the bowl
            (mBowl.getRadius()-mMarble.getRadius())*glm::vec3(
                cos(mTheta2)*sin(mTheta1),
                -cos(mTheta1),
                sin(mTheta2)*sin(mTheta1)
            )
        )
    );

}