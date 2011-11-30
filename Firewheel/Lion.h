//
//  Lion.h
//  Firewheel
//
//  Created by Mark Sands on 11/11/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef Firewheel_Lion_h
#define Firewheel_Lion_h

#include <GLTools.h>
#include <GLShaderManager.h>
#include <GLFrustum.h>
#include <GLBatch.h>
#include <GLFrame.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <StopWatch.h>
#include <cmath>
#include <ctime>

class Lion
{
public:
  Lion();

  void SetupRenderingContext();
  void Update();
  void Draw(GLMatrixStack &modelViewMatrix, GLShaderManager &shaderManager, GLGeometryTransform &transformPipeline,
			      M3DVector4f &vLightEyePos, GLfloat totalCarRot, GLuint wallTexture, GLuint carTexture[]);
private:
  GLTriangleBatch head;
  GLTriangleBatch body;
};

// Default Constructor
Lion::Lion()
{
  
}

// Set up all structures needed to render the Ferris wheel car objects.
void Lion::SetupRenderingContext()
{
  gltMakeSphere(head, 10, 30, 15);
  gltMakeCylinder(body, 10, 10, 10, 30, 30);
}

// Timer-driven function to update positions, orientations, 
// etc., of all changing parts of the Ferris wheel car.
void Lion::Update()
{
}

// Render the components of the Ferris wheel car.
void Lion::Draw(GLMatrixStack &modelViewMatrix, GLShaderManager &shaderManager, GLGeometryTransform &transformPipeline,
               M3DVector4f &vLightEyePos, GLfloat totalCarRot, GLuint wallTexture, GLuint carTexture[])
{
	// Get the light position in eye space
	M3DVector4f	vLightTransformed;
	M3DMatrix44f mCamera;
	modelViewMatrix.GetMatrix(mCamera);
	m3dTransformVector4(vLightTransformed, LIGHT_POSITION, mCamera);

	modelViewMatrix.PushMatrix();

    modelViewMatrix.PushMatrix();
      glBindTexture(GL_TEXTURE_2D, carTexture[0]);
      modelViewMatrix.Translate(0.0f, 0.0f, -0.5f * BAR_LENGTH);
      shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(), 
                                   transformPipeline.GetProjectionMatrix(), vLightTransformed, WHITE_COLOR, 0);
      //barBatch.Draw();
      head.Draw();
    modelViewMatrix.PopMatrix();


    modelViewMatrix.Rotate(-totalCarRot, 0.0f, 0.0f, 1.0f);
    modelViewMatrix.Rotate(90.0f, 1.0f, 0.0f, 0.0f);


    modelViewMatrix.PushMatrix();
      glBindTexture(GL_TEXTURE_2D, carTexture[1]);
      shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
                                 transformPipeline.GetProjectionMatrix(), vLightTransformed, WHITE_COLOR, 0);
      //roofBatch.Draw();
      body.Draw();
    modelViewMatrix.PopMatrix();


    modelViewMatrix.PushMatrix();
      //glBindTexture(GL_TEXTURE_2D, carTexture[2]);
      shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
                                 transformPipeline.GetProjectionMatrix(), vLightTransformed, WHITE_COLOR, 0);
      //poleBatch.Draw();
    modelViewMatrix.PopMatrix();

  
    modelViewMatrix.PushMatrix();
      //glBindTexture(GL_TEXTURE_2D, carTexture[4]);
      modelViewMatrix.Translate(0.0f, 0.0f, POLE_LENGTH);
      modelViewMatrix.Scale(FLOOR_SCALE[0], FLOOR_SCALE[1], FLOOR_SCALE[2]);
      shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
                                   transformPipeline.GetProjectionMatrix(), vLightTransformed, WHITE_COLOR, 0);
      //floorBatch.Draw();
    modelViewMatrix.PopMatrix();

    
    modelViewMatrix.PushMatrix();
      //glBindTexture(GL_TEXTURE_2D, wallTexture);
      modelViewMatrix.Translate(0.0f, 0.0f, POLE_LENGTH);// POLE_LENGTH - WALL_LENGTH);
      modelViewMatrix.Rotate(90.0f, 0.0f, 0.0f, 1.0f);
      modelViewMatrix.Rotate(180.0f, 0.0f, 1.0f, 0.0f);
      shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
                                   transformPipeline.GetProjectionMatrix(), vLightTransformed, WHITE_COLOR, 0);
      //wallBatch.Draw();
    modelViewMatrix.PopMatrix();

  modelViewMatrix.PopMatrix();
}

#endif
