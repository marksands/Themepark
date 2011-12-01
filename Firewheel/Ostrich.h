//
//  Ostrich.h
//  Firewheel
//
//  Created by Mark Sands on 11/11/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef Firewheel_Ostrich_h
#define Firewheel_Ostrich_h

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

const GLfloat OSTRICH_BODY_COLOR[] = { .375, 0.2, 0.067, 1.0 };
const GLfloat OSTRICH_SKIN_COLOR[] = { .97, .8, .79, 1.0 };
const GLfloat OSTRICH_BEAK_COLOR[] = { 0.8, 0.8, 0.0, 1.0 };

class Ostrich
{
public:
  Ostrich();

  void SetupRenderingContext();
  void Update();
  void Draw(GLMatrixStack &modelViewMatrix, GLShaderManager &shaderManager, GLGeometryTransform &transformPipeline, M3DVector4f &vLightEyePos);

private:
  GLTriangleBatch head;
  GLTriangleBatch beak;
  GLTriangleBatch kneck;
  GLTriangleBatch body;
  GLTriangleBatch tailfeather[3];
  GLTriangleBatch legs[2];
};

/* ------------------- */
/* Default Constructor */

Ostrich::Ostrich()
{
  
}

/* -------------------------------------------------- */
/* Set up all structures needed to render the Ostrich */

void Ostrich::SetupRenderingContext()
{
  gltMakeSphere(head, 0.04, 30, 15);
  gltMakeCylinder(beak, 0.02, 0.0, 0.03, 15, 15);
  gltMakeCylinder(kneck, 0.015, 0.015, 0.3, 15, 15);
  gltMakeSphere(body, .1, 30, 15);

  for (int i = 0; i < 3; i++)
    gltMakeCylinder(tailfeather[i], 0.09, 0.0, 0.02, 15, 15);

  for (int i = 0; i < 2; i++)
    gltMakeCylinder(legs[i], 0.01, 0.01, 0.25, 15, 15);
}

/* ---------------------------------------- */
/* Any code that updates the Ostrich figure */

void Ostrich::Update()
{
  
}

/* ----------------------------------------- */
/* Render the components of the Ostrich body */

void Ostrich::Draw(GLMatrixStack &modelViewMatrix, GLShaderManager &shaderManager, GLGeometryTransform &transformPipeline, M3DVector4f &vLightEyePos)
{
	// Get the light position in eye space
	M3DVector4f	vLightTransformed;
	M3DMatrix44f mCamera;
	modelViewMatrix.GetMatrix(mCamera);
	m3dTransformVector4(vLightTransformed, LIGHT_POSITION, mCamera);

	modelViewMatrix.PushMatrix();

    modelViewMatrix.Translate(0.0, -0.2, 0.0);
  
    /* body */
    modelViewMatrix.PushMatrix();
      shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
                                   transformPipeline.GetProjectionMatrix(), vLightTransformed, OSTRICH_BODY_COLOR, 0);
      body.Draw();
    modelViewMatrix.PopMatrix();

    /* Tailfeather */
    modelViewMatrix.PushMatrix();
      modelViewMatrix.Rotate(-90, 1.0, 0.0, 0.0);

      modelViewMatrix.PushMatrix();
        modelViewMatrix.Translate(-0.05, 0.0, 0.09);
        modelViewMatrix.Rotate(-12, 0.0, 1.0, 0.0);
        shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
                                     transformPipeline.GetProjectionMatrix(), vLightTransformed, OSTRICH_BODY_COLOR, 0);
        tailfeather[0].Draw();
      modelViewMatrix.PopMatrix();
    modelViewMatrix.PopMatrix();

    /* tailfeather front */
    modelViewMatrix.PushMatrix();
      modelViewMatrix.Rotate(-90, 0.0, 0.0, 1.0);
    
      modelViewMatrix.PushMatrix();
        modelViewMatrix.Translate(0.0, 0.0, 0.09);
        modelViewMatrix.Rotate(-12, 0.0, 1.0, 0.0);
        shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
                                     transformPipeline.GetProjectionMatrix(), vLightTransformed, OSTRICH_BODY_COLOR, 0);
        tailfeather[0].Draw();
      modelViewMatrix.PopMatrix();
    modelViewMatrix.PopMatrix();

    /* tailfeather back */
    modelViewMatrix.PushMatrix();
      modelViewMatrix.Rotate(-90, 0.0, 0.0, 1.0);
      modelViewMatrix.Rotate(-135, 0.0, 1.0, 0.0);

      modelViewMatrix.PushMatrix();
        modelViewMatrix.Translate(-0.05, 0.0, 0.09);
        modelViewMatrix.Rotate(-12, 0.0, 1.0, 0.0);
        shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
                                     transformPipeline.GetProjectionMatrix(), vLightTransformed, OSTRICH_BODY_COLOR, 0);
        tailfeather[0].Draw();
      modelViewMatrix.PopMatrix();
    modelViewMatrix.PopMatrix();


    /* kneck */
    modelViewMatrix.PushMatrix();
      modelViewMatrix.Rotate(90, 0.0, 1.0, 0.0);
      modelViewMatrix.Rotate(-65, 1.0, 0.0, 0.0);
      shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
                                   transformPipeline.GetProjectionMatrix(), vLightTransformed, OSTRICH_SKIN_COLOR, 0);
      kneck.Draw();
    modelViewMatrix.PopMatrix();

    /* head */
    modelViewMatrix.PushMatrix();
      modelViewMatrix.Translate(0.14, 0.3, 0.0);
      shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
                                 transformPipeline.GetProjectionMatrix(), vLightTransformed, OSTRICH_SKIN_COLOR, 0);
      head.Draw();
    modelViewMatrix.PopMatrix();

    /* beak */
    modelViewMatrix.PushMatrix();
      modelViewMatrix.Translate(0.175, 0.3, 0.0);
      modelViewMatrix.Rotate(90, 0.0, 1.0, 0.0);
      shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
                                   transformPipeline.GetProjectionMatrix(), vLightTransformed, OSTRICH_BEAK_COLOR, 0);
      beak.Draw();
    modelViewMatrix.PopMatrix();

    /* legs 1 */
    modelViewMatrix.PushMatrix();
      modelViewMatrix.Rotate(105, 1.0, 0.0, 0.0);
      shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
                                   transformPipeline.GetProjectionMatrix(), vLightTransformed, OSTRICH_SKIN_COLOR, 0);
      legs[0].Draw();
    modelViewMatrix.PopMatrix();
    
    /* legs 2 */
    modelViewMatrix.PushMatrix();
      modelViewMatrix.Rotate(75, 1.0, 0.0, 0.0);
      shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
                                   transformPipeline.GetProjectionMatrix(), vLightTransformed, OSTRICH_SKIN_COLOR, 0);
      legs[1].Draw();
    modelViewMatrix.PopMatrix();

  modelViewMatrix.PopMatrix();
}

#endif
