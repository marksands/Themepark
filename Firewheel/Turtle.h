//
//  Turtle.h
//  Firewheel
//
//  Created by Mark Sands on 12/1/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef Firewheel_Turtle_h
#define Firewheel_Turtle_h


#include <GLTools.h>
#include <GLShaderManager.h>
#include <GLFrustum.h>
#include <GLBatch.h>
#include <GLFrame.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <cmath>
#include <ctime>

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

const GLfloat TURTLE_BODY_COLOR[] = { .375, 0.2, 0.067, 1.0 };
const GLfloat TURTLE_LIMB_COLOR[] = { 0.25, 0.4, 0.25, 1.0 };
const GLfloat TURTLE_SHELL_COLOR[] = { 0.4, 0.3, 0.1, 1.0 };


class Turtle
{
public:
  Turtle();
  
  void SetupRenderingContext();
  void Update();
  void Draw(GLMatrixStack &modelViewMatrix, GLShaderManager &shaderManager, GLGeometryTransform &transformPipeline, M3DVector4f &vLightEyePos);
  
private:
  GLTriangleBatch head;
  GLTriangleBatch shell[2];
  GLTriangleBatch body;
  GLTriangleBatch arm[2];
  GLTriangleBatch leg[2];
  GLTriangleBatch tail;
};

/* ------------------- */
/* Default Constructor */

Turtle::Turtle()
{
  
}

/* -------------------------------------------------- */
/* Set up all structures needed to render the Turtle. */

void Turtle::SetupRenderingContext()
{
  int i;

  gltMakeSphere(head, 0.025, 30, 30);
  gltMakeCylinder(body, 0.09, 0.09, 0.02, 30, 15);

  for ( i = 0; i < 2; i++ )
    gltMakeCylinder(shell[i], 0.1, 0.0, 0.02, 30, 15);

  for ( i = 0; i < 2; i++ )
    gltMakeCylinder(arm[i], 0.01, 0.0, 0.025, 30, 15);

  for ( i = 0; i < 2; i++ )
    gltMakeCylinder(leg[i], 0.01, 0.0, 0.025, 30, 15);

  gltMakeCylinder(tail, 0.01, 0.0, 0.035, 30, 15);
}

/* ---------------------------------------------- */
/* Timer-driven function to update what's changed */

void Turtle::Update()
{
  
}

/* ------------------------------------ */
/* Render the components of the Turtle. */

void Turtle::Draw(GLMatrixStack &modelViewMatrix, GLShaderManager &shaderManager, GLGeometryTransform &transformPipeline, M3DVector4f &vLightEyePos)
{
	// Get the light position in eye space
	M3DVector4f	vLightTransformed;
	M3DMatrix44f mCamera;
	modelViewMatrix.GetMatrix(mCamera);
	m3dTransformVector4(vLightTransformed, LIGHT_POSITION, mCamera);
  
	modelViewMatrix.PushMatrix();
  
    modelViewMatrix.Translate(0.0, -0.1, 0.0);
  
    /* ----------------- */
    /* -- Turtle Body -- */

    modelViewMatrix.PushMatrix();
      modelViewMatrix.Rotate(90, 1.0, 0.0, 0.0);
      shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(), 
                                   transformPipeline.GetProjectionMatrix(), vLightTransformed, TURTLE_BODY_COLOR, 0);
      body.Draw();
    modelViewMatrix.PopMatrix();
    
    /* ------------------- */
    /* -- Turtle Shell  -- */
    
    /* shell top */
    modelViewMatrix.PushMatrix();
      modelViewMatrix.Rotate(-90, 1.0, 0.0, 0.0);
      shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(), 
                                   transformPipeline.GetProjectionMatrix(), vLightTransformed, TURTLE_SHELL_COLOR, 0);
      shell[0].Draw();    
    modelViewMatrix.PopMatrix();

    /* shell bottom */
    modelViewMatrix.PushMatrix();
      modelViewMatrix.Rotate(90, 1.0, 0.0, 0.0);
      modelViewMatrix.Translate(0.0, 0.0, 0.02);
      shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(), 
                                   transformPipeline.GetProjectionMatrix(), vLightTransformed, TURTLE_SHELL_COLOR, 0);
      shell[1].Draw();    
    modelViewMatrix.PopMatrix();

    /* ------------------- */
    /* -- Turtle Head -- */
        
    modelViewMatrix.PushMatrix();
      modelViewMatrix.Translate(0.11, 0.0, 0.0);
      shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
                                   transformPipeline.GetProjectionMatrix(), vLightTransformed, TURTLE_LIMB_COLOR, 0);
      head.Draw();
    modelViewMatrix.PopMatrix();
    
    /* ----------------- */
    /* -- Turtle Arms -- */

    modelViewMatrix.PushMatrix();
      modelViewMatrix.Translate(0.04, -0.01, 0.08);
      shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
                                   transformPipeline.GetProjectionMatrix(), vLightTransformed, TURTLE_LIMB_COLOR, 0);
      arm[0].Draw();
    modelViewMatrix.PopMatrix();

    modelViewMatrix.PushMatrix();
      modelViewMatrix.Translate(0.04, -0.01, -0.08);
      modelViewMatrix.Rotate(180, 1.0, 0.0, 0.0);
      shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
                                   transformPipeline.GetProjectionMatrix(), vLightTransformed, TURTLE_LIMB_COLOR, 0);
      arm[1].Draw();
    modelViewMatrix.PopMatrix();

    /* ----------------- */
    /* -- Turtle Legs -- */
    
    modelViewMatrix.PushMatrix();
      modelViewMatrix.Translate(-0.04, -0.01, 0.08);
      shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
                                   transformPipeline.GetProjectionMatrix(), vLightTransformed, TURTLE_LIMB_COLOR, 0);
      leg[0].Draw();
    modelViewMatrix.PopMatrix();
  
    modelViewMatrix.PushMatrix();
      modelViewMatrix.Translate(-0.04, -0.01, -0.08);
      modelViewMatrix.Rotate(180, 1.0, 0.0, 0.0);
      shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
                                   transformPipeline.GetProjectionMatrix(), vLightTransformed, TURTLE_LIMB_COLOR, 0);
      leg[1].Draw();
    modelViewMatrix.PopMatrix();

    /* ----------------- */
    /* -- Turtle Tail -- */

    modelViewMatrix.PushMatrix();
      modelViewMatrix.Translate(-0.09, -0.01, 0.0);
      modelViewMatrix.Rotate(-90, 0.0, 1.0, 0.0);
      shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
                                   transformPipeline.GetProjectionMatrix(), vLightTransformed, TURTLE_LIMB_COLOR, 0);
      tail.Draw();
    modelViewMatrix.PopMatrix();
    
  modelViewMatrix.PopMatrix();
}

#endif
