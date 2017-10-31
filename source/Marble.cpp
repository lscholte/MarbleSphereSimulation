#include "Marble.hpp"
#include "Shader.hpp"
#include "Asset.hpp"
#include <atlas/utils/GUI.hpp>
#include <stb/stb_image.h>
#include <iostream>

Marble::Marble() :
	mRadius(0.5f)
{
	atlas::utils::Mesh::fromFile(
		(generated::Asset::getAssetDirectory() + "/sphere.obj").c_str(),
		mMesh
	);


	for(auto &vertex : mMesh.vertices())
	{
		vertex *= mRadius;
	}

	glGenVertexArrays(1, &mVao);
	glGenBuffers(1, &mPositionBuffer);
	glGenBuffers(1, &mTextureBuffer);
	glGenBuffers(1, &mIndexBuffer);
	
	glBindVertexArray(mVao);
	
	glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, mMesh.vertices().size()*sizeof(glm::vec3), mMesh.vertices().data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	
	glBindBuffer(GL_ARRAY_BUFFER, mTextureBuffer);
	glBufferData(GL_ARRAY_BUFFER, mMesh.texCoords().size()*sizeof(glm::vec2), mMesh.texCoords().data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mMesh.indices().size()*sizeof(GLint), mMesh.indices().data(), GL_STATIC_DRAW);
	
	glBindVertexArray(0);

	int imageWidth, imageHeight, imageComp;    
    stbi_set_flip_vertically_on_load(1);    
    stbi_uc* image = stbi_load((generated::Asset::getAssetDirectory() + "/pokeball.png").c_str(), &imageWidth, &imageHeight, &imageComp, 4);
    stbi_set_flip_vertically_on_load(0);
    
    glGenTextures(1, &mTextureId);
    glActiveTexture(GL_TEXTURE0);    
    glBindTexture(GL_TEXTURE_2D, mTextureId);
    if(imageComp == 3)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);        
    }
    else if(imageComp == 4)
    {   
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);        
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);        

    stbi_image_free(image);

	std::vector<atlas::gl::ShaderUnit> shaderUnits
	{
		atlas::gl::ShaderUnit(generated::Shader::getShaderDirectory() + "/scene.vert", GL_VERTEX_SHADER),
		atlas::gl::ShaderUnit(generated::Shader::getShaderDirectory() + "/scene.frag", GL_FRAGMENT_SHADER)
	};
	
	mShaders.push_back(atlas::gl::Shader(shaderUnits));
	
	mShaders[0].compileShaders();
	mShaders[0].linkShaders();
}

Marble::~Marble()
{
}




void Marble::renderGeometry(atlas::math::Matrix4 const &projection, atlas::math::Matrix4 const &view)
{	
    mShaders[0].enableShaders();
	
	glm::mat4 modelViewProjection = projection * view * mModel;

	const GLint MODEL_VIEW_PROJECTION_UNIFORM_LOCATION = glGetUniformLocation(mShaders[0].getShaderProgram(), "ModelViewProjection");
	const GLint MODEL_UNIFORM_LOCATION = glGetUniformLocation(mShaders[0].getShaderProgram(), "Model");
	const GLint USE_DIFFUSE_MAP_UNIFORM_LOCATION = glGetUniformLocation(mShaders[0].getShaderProgram(), "UseDiffuseMap");        
	const GLint DIFFUSE_MAP_UNIFORM_LOCATION = glGetUniformLocation(mShaders[0].getShaderProgram(), "DiffuseMap"); 
	
	glActiveTexture(GL_TEXTURE0);
    glUniform1i(DIFFUSE_MAP_UNIFORM_LOCATION, 0);
    glUniform1i(USE_DIFFUSE_MAP_UNIFORM_LOCATION, true);    
    glBindTexture(GL_TEXTURE_2D, mTextureId); 
	
	glUniformMatrix4fv(MODEL_VIEW_PROJECTION_UNIFORM_LOCATION, 1, GL_FALSE, &modelViewProjection[0][0]);
	glUniformMatrix4fv(MODEL_UNIFORM_LOCATION, 1, GL_FALSE, &mModel[0][0]); 

	glBindVertexArray(mVao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);	
	glDrawElements(GL_TRIANGLES, mMesh.indices().size(), GL_UNSIGNED_INT, (void *) 0);
	glBindVertexArray(0);


    mShaders[0].disableShaders();
}

float Marble::getRadius() const
{
	return mRadius;
}

void Marble::setModel(glm::mat4 const &model)
{
	mModel = model;
}
