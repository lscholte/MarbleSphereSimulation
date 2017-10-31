#include "Bowl.hpp"
#include "Shader.hpp"
#include "Asset.hpp"
#include <atlas/utils/GUI.hpp>
#include <stb/stb_image.h>
#include <iostream>

Bowl::Bowl(float radius) :
	mRadius(radius)
{
	atlas::utils::Mesh::fromFile(
		(generated::Asset::getAssetDirectory() + "/sphere.obj").c_str(),
		mMesh
	);


	for(int i = 0; i < mMesh.indices().size(); ++i)
	{
		mColors.push_back(glm::vec3(1.0, 1.0, 1.0));
	}

	for(auto &vertex : mMesh.vertices())
	{
		vertex *= mRadius;
	}

	glGenVertexArrays(1, &mVao);
	glGenBuffers(1, &mPositionBuffer);
	glGenBuffers(1, &mColorBuffer);
	glGenBuffers(1, &mIndexBuffer);
	
	glBindVertexArray(mVao);
	
	glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, mMesh.vertices().size()*sizeof(glm::vec3), mMesh.vertices().data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	
	glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, mColors.size()*sizeof(glm::vec3), mColors.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mMesh.indices().size()*sizeof(GLint), mMesh.indices().data(), GL_STATIC_DRAW);
	
	glBindVertexArray(0);

	std::vector<atlas::gl::ShaderUnit> shaderUnits
	{
		atlas::gl::ShaderUnit(generated::Shader::getShaderDirectory() + "/scene.vert", GL_VERTEX_SHADER),
		atlas::gl::ShaderUnit(generated::Shader::getShaderDirectory() + "/scene.frag", GL_FRAGMENT_SHADER)
	};
	
	mShaders.push_back(atlas::gl::Shader(shaderUnits));
	
	mShaders[0].compileShaders();
	mShaders[0].linkShaders();
}

Bowl::Bowl() :
	Bowl(5.0f)
{
}

Bowl::~Bowl()
{
}




void Bowl::renderGeometry(atlas::math::Matrix4 const &projection, atlas::math::Matrix4 const &view)
{	
    mShaders[0].enableShaders();
	
	glm::mat4 modelViewProjection = projection * view * mModel;

	const GLint MODEL_VIEW_PROJECTION_UNIFORM_LOCATION = glGetUniformLocation(mShaders[0].getShaderProgram(), "ModelViewProjection");
    const GLint MODEL_UNIFORM_LOCATION = glGetUniformLocation(mShaders[0].getShaderProgram(), "Model");
	
	glUniformMatrix4fv(MODEL_VIEW_PROJECTION_UNIFORM_LOCATION, 1, GL_FALSE, &modelViewProjection[0][0]);
	glUniformMatrix4fv(MODEL_UNIFORM_LOCATION, 1, GL_FALSE, &mModel[0][0]); 

	glBindVertexArray(mVao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Draw wireframe
	glDrawElements(GL_TRIANGLES, mMesh.indices().size(), GL_UNSIGNED_INT, (void *) 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	
	glBindVertexArray(0);


    mShaders[0].disableShaders();
}

float Bowl::getRadius() const
{
	return mRadius;
}

void Bowl::setModel(glm::mat4 const &model)
{
	mModel = model;
}
