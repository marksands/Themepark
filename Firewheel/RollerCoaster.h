//
//  RollerCoaster.h
//  Firewheel
//
//  Created by Mark Sands on 11/28/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef Firewheel_RollerCoaster_h
#define Firewheel_RollerCoaster_h

#include <GLTools.h>
#include <GLShaderManager.h>
#include <GLFrustum.h>
#include <GLBatch.h>
#include <GLFrame.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <StopWatch.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <ctime>

#include "CarTextured.h"

/* R_POLES */
const GLfloat R_POLE_COLOR[] = { 0.67f, 0.67f, 0.67f, 1.0f };
const GLint NUMBER_R_POLES = 128;
const GLfloat R_POLE_MARGIN = 0.1f;
const GLfloat R_POLE_BASE_RADIUS = 0.01f;
const GLfloat R_POLE_TOP_RADIUS  = 0.01f;
const GLint R_POLE_NUMBER_SLICES = 10;
const GLint R_POLE_NUMBER_STACKS = 10;

/* RUNNER */
const GLint NUMBER_RUNNER = 128;
const GLfloat RUNNER_LENGTH = R_POLE_MARGIN;
const GLfloat RUNNER_BASE_RADIUS = 0.01;
const GLfloat RUNNER_TOP_RADIUS = 0.01;
const GLint RUNNER_NUMBER_SLICES = 10;
const GLint RUNNER_NUMBER_STACKS = 10;

/* FRAME */
const GLfloat FRAME_COLOR[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat FRAME_OUTER_RADIUS = 3.0f;
const GLfloat FRAME_INNER_RADIUS = 0.025f;
const GLint FRAME_OUTER_SLICES = 30;
const GLint FRAME_INNER_SLICES = 30;

const GLint NUMBER_CARS = 3;

/* DATA FILE */
const char* COASTER_DATA_FILE = "roller_coaster_data.txt";
const char* RUNNER_DATA_FILE = "runner_data.txt";

/* TRACK */
class Track
{
  public:
    Track();
    ~Track();

    void SetupRenderingContext();
    void Update();
    void Draw(GLMatrixStack &modelViewMatrix, GLShaderManager &shaderManager, GLGeometryTransform &transformPipeline, M3DVector4f &vLightEyePos);

  private:
    GLTriangleBatch frame;
    GLTriangleBatch r_poles[NUMBER_R_POLES];
    GLTriangleBatch circuit[NUMBER_RUNNER];
  
    GLBatch lineLoop;
  
    GLfloat r_poleLength[NUMBER_R_POLES];
    GLfloat runner_verts[NUMBER_RUNNER][3];

    Car cars[NUMBER_CARS];
};

Track::Track()
{
  
}

Track::~Track()
{
  
}

void Track::SetupRenderingContext()
{ 
  int i;
  std::string fileHeader;

  /* read pole data int r_poleLength */
  std::ifstream dataFile(COASTER_DATA_FILE);
  if (dataFile.is_open())
  {
    /* ignore first line in data file */
    getline(dataFile, fileHeader);

    for (i = 0; !dataFile.eof(); i++) {
      dataFile >> r_poleLength[i];
      r_poleLength[i] *= 3;
    }
  }
  dataFile.close();

  /* read runner data into runner_verts */
  std::ifstream runnerData(RUNNER_DATA_FILE);
  if (runnerData.is_open())
  {
    for (i = 0; !runnerData.eof(); i++) {
      runnerData >> runner_verts[i][0] >> runner_verts[i][1] >> runner_verts[i][2];
      runner_verts[i][0] -= 0.32;
      runner_verts[i][2] += 0.075;

      runner_verts[i][0] *= 3.0;
      runner_verts[i][1] *= 3.0;
      runner_verts[i][2] *= 3.0;
    }
  }
  runnerData.close();
  
  /* Copy vertex data into custom batch object */
  glLineWidth(14.0f);
  lineLoop.Color4f(0.6, 0.6, 0.6, 1.0);
  lineLoop.Begin(GL_LINE_LOOP, 128);
    lineLoop.CopyNormalDataf(runner_verts);
    lineLoop.CopyVertexData3f(runner_verts);
  lineLoop.End();

  /* coaster support poles */
  for ( i = 0; i < NUMBER_R_POLES; i++ )
    gltMakeCylinder(r_poles[i], R_POLE_BASE_RADIUS, R_POLE_TOP_RADIUS, r_poleLength[i], R_POLE_NUMBER_SLICES, R_POLE_NUMBER_STACKS);

  /* coaster track */
  for ( i = 0; i < NUMBER_RUNNER; i++ )
    gltMakeCylinder(circuit[i], RUNNER_BASE_RADIUS, RUNNER_TOP_RADIUS, RUNNER_LENGTH, RUNNER_NUMBER_SLICES, RUNNER_NUMBER_STACKS);

  /* bottom eliptical frame */
  gltMakeTorus(frame, FRAME_OUTER_RADIUS, FRAME_INNER_RADIUS, FRAME_OUTER_SLICES, FRAME_INNER_SLICES);
}

void Track::Update()
{
  
}

void Track::Draw(GLMatrixStack &modelViewMatrix, GLShaderManager &shaderManager, GLGeometryTransform &transformPipeline, M3DVector4f &vLightEyePos)
{
  int i = 0;

  modelViewMatrix.PushMatrix();
  
    modelViewMatrix.Translate(3.0, 0.0, 0.0);

    /* ---------------------- */
    /* Bottom eliptical frame */

    modelViewMatrix.PushMatrix();
      modelViewMatrix.Rotate(-90, 1.0f, 0.0f, 0.0f);
      modelViewMatrix.Translate(0.0f, 0.0f, -0.65f);
      shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, transformPipeline.GetModelViewMatrix(), 
                                   transformPipeline.GetProjectionMatrix(), vLightEyePos, FRAME_COLOR);
      frame.Draw();
    modelViewMatrix.PopMatrix();

    /* -------------------------------------- */
    /* Roller Coaster support beams (r_poles) */
    
    modelViewMatrix.PushMatrix();
      for ( i = 0; i < NUMBER_R_POLES; i++ )
      {
        GLfloat currentRotation = i * 360.0f/NUMBER_R_POLES;

        /* rotate support beam verticle, start in center and spread out at coords x=cos(rot), y=sin(rot) */
        modelViewMatrix.PushMatrix();
          modelViewMatrix.Rotate(-90, 1.0f, 0.0f, 0.0f);
          modelViewMatrix.Translate(0.0f, 0.0f, -0.7f);
          modelViewMatrix.Translate(3*cosf(m3dDegToRad(currentRotation)), 3*sinf(m3dDegToRad(currentRotation)), 0.0f);

      M3DVector3f vout = { 0.0f, 0.0f, 0.0f };
      M3DVector3f vin  = { 0.0f, 0.0f, 0.0f };
      m3dTransformVector3(vout, vin, modelViewMatrix.GetMatrix());
      
      //printf("%f %f %f\n", vout[0], vout[1], vout[2]);
          shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, transformPipeline.GetModelViewMatrix(), 
                                       transformPipeline.GetProjectionMatrix(), vLightEyePos, R_POLE_COLOR);
          r_poles[i].Draw();
        modelViewMatrix.PopMatrix();
      }
    modelViewMatrix.PopMatrix();

  //printf("----------\n-----------\n------------");

    /* --------------------- */
    /* Roller Coaster runner */

    modelViewMatrix.PushMatrix();
      for ( i = 0; i < NUMBER_RUNNER; i++ )
      {
        GLfloat currentRotation = i * 360.0f/NUMBER_RUNNER;

        /* rotate support beam verticle, start in center and spread out at coords x=cos(rot), y=sin(rot) */\
        modelViewMatrix.PushMatrix();
          modelViewMatrix.Rotate(-90, 1.0f, 0.0f, 0.0f);
          modelViewMatrix.Translate(0.0f, 0.0f, r_poleLength[i]-0.69f);
          modelViewMatrix.Translate(cosf(m3dDegToRad(currentRotation)), sinf(m3dDegToRad(currentRotation)), 0.0f);
          shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, transformPipeline.GetModelViewMatrix(), 
                                       transformPipeline.GetProjectionMatrix(), vLightEyePos, R_POLE_COLOR);
        //circuit[i].Draw();
        modelViewMatrix.PopMatrix();  
      }
    modelViewMatrix.PopMatrix();

  modelViewMatrix.PopMatrix();

  /* --------------------------- */
  /* Roller Coaster runner track */

  modelViewMatrix.PushMatrix();

    // Get the light position in eye space
    M3DVector4f	vLightTransformed;
    M3DMatrix44f mCamera;
    modelViewMatrix.GetMatrix(mCamera);
    m3dTransformVector4(vLightTransformed, LIGHT_POSITION, mCamera);
    
    // Gave up making the shader work :(
    // shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
    //                              transformPipeline.GetProjectionMatrix(), vLightTransformed, R_POLE_COLOR, 0);

    lineLoop.Draw();
  modelViewMatrix.PopMatrix();

  /* ---- */
  /* Cars */

  modelViewMatrix.PushMatrix();
    
  modelViewMatrix.PopMatrix();
}

#endif
