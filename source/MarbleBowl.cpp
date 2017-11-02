#include "MarbleBowl.hpp"
#include <atlas/utils/GUI.hpp>
#include <iostream>

MarbleBowl::MarbleBowl() :
    mBowl(5.0f),
    mTheta1(0.0f),
    mTheta2(0.0f),
    mTheta1Dot(0.0f),
    mTheta2Dot(0.0f),
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
    mTheta2DotDot = 0.0;    

    //Compute the angular velocities of the marble around the bowl
    mTheta1Dot = mTheta1Dot + mTheta1DotDot*t.deltaTime;
    mTheta2Dot = mTheta2Dot + mTheta2DotDot*t.deltaTime;    

    //Compute the angular positions of the marble around the bowl
    float originalTheta1 = mTheta1;
    float originalTheta2 = mTheta2;
    mTheta1 = mTheta1 + mTheta1Dot*t.deltaTime;
    mTheta2 = std::fmod(mTheta2 + mTheta2Dot*t.deltaTime, 2.0*M_PI);    

    glm::vec3 originalPos = glm::vec3(
        cos(originalTheta2)*sin(originalTheta1),
        -cos(originalTheta1),
        sin(originalTheta2)*sin(originalTheta1)
    );

    glm::vec3 newPos = glm::vec3(
        cos(mTheta2)*sin(mTheta1),
        -cos(mTheta1),
        sin(mTheta2)*sin(mTheta1)
    );

    //Compute the angle which the marble must rotate to get to its new position
    float angleBetweenPositions = acos(glm::dot(originalPos, newPos));
    mMarbleRotationAngle = (mBowl.getRadius() / mMarble.getRadius()) * angleBetweenPositions;
    
    //Compute the axis around which the marble must rotate to get to its new position
    glm::vec3 normal = originalPos;
    glm::vec3 marbleDirection = glm::normalize(newPos - originalPos);
    mMarbleRotationAxis = -glm::cross(normal, marbleDirection);         
    if(std::isnan(mMarbleRotationAxis.x) || std::isnan(mMarbleRotationAxis.y) || std::isnan(mMarbleRotationAxis.z))
    {
        mMarbleRotationAxis = glm::vec3(0, 0, 0);                 
    }
    
	this->applyTransformations();
}

void MarbleBowl::drawGui()
{	
	ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiSetCond_FirstUseEver);
	
	ImGui::Begin("Marble Bowl Options");
	ImGui::SliderFloat("Theta 1", &mTheta1, -0.5*M_PI, 0.5*M_PI);
    ImGui::SliderFloat("Theta 2", &mTheta2, 0.0f, 2.0*M_PI);
    // ImGui::SliderFloat("Theta 1 (Speed)", &mTheta1Dot, -4.0*M_PI, 4.0*M_PI);
	ImGui::SliderFloat("Theta 2 (Speed)", &mTheta2Dot, -4.0*M_PI, 4.0*M_PI);
	ImGui::End();
}

void MarbleBowl::applyTransformations()
{
	//Resets the model matrices so that we can re-apply scaling and/or translations to them
	mMarble.setModel(glm::mat4(1.0f));
    

    //Rotate marble to its correct orientation
    if(mMarbleRotationAxis != glm::vec3(0.0, 0.0, 0.0))
    {
        mRotationMatrix = glm::rotate(glm::mat4(1.0f), mMarbleRotationAngle, mMarbleRotationAxis) * mRotationMatrix;
    }
    mMarble.transformGeometry(mRotationMatrix);

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