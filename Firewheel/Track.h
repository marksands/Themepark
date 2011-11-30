//
//  Track.h
//  Firewheel
//
//  Created by Mark Sands on 11/28/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef Firewheel_Track_h
#define Firewheel_Track_h

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
const GLfloat R_POLE_MARGIN = 0.02f;
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
const GLfloat FRAME_OUTER_RADIUS = 1.0f;
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
    }
  }
  dataFile.close();

  /* read runner data into runner_verts */
  std::ifstream runnerData(RUNNER_DATA_FILE);
  if (runnerData.is_open())
  {
    for (i = 0; !runnerData.eof(); i++) {
      runnerData >> runner_verts[i][0] >> runner_verts[i][1] >> runner_verts[i][2];
      runner_verts[i][0] -= 1.00;
      runner_verts[i][2] += 0.08;
    }
  }
  runnerData.close();
  
  /* Copy vertex data into custom batch object */
  lineLoop.Begin(GL_LINE_LOOP, 128);
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

  /* Bottom eliptical frame */
  modelViewMatrix.PushMatrix();
    modelViewMatrix.Rotate(-90, 1.0f, 0.0f, 0.0f);
    modelViewMatrix.Translate(0.0f, 0.0f, -0.65f);
    shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, transformPipeline.GetModelViewMatrix(), 
                                 transformPipeline.GetProjectionMatrix(), vLightEyePos, FRAME_COLOR);
    frame.Draw();
  modelViewMatrix.PopMatrix();

  /* Roller Coaster support beams (r_poles) */
  modelViewMatrix.PushMatrix();
    for ( i = 0; i < NUMBER_R_POLES; i++ )
    {
      GLfloat currentRotation = i * 360.0f/NUMBER_R_POLES;

      /* rotate support beam verticle, start in center and spread out at coords x=cos(rot), y=sin(rot) */
      modelViewMatrix.PushMatrix();
        modelViewMatrix.Rotate(-90, 1.0f, 0.0f, 0.0f);
        modelViewMatrix.Translate(0.0f, 0.0f, -0.7f);
        modelViewMatrix.Translate(cosf(DEG2RAD(currentRotation)), sinf(DEG2RAD(currentRotation)), 0.0f);
        //printf("%f %f %f\n", cosf(DEG2RAD(currentRotation)), sinf(DEG2RAD(currentRotation)), r_poleLength[i] - 0.7f );
        shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, transformPipeline.GetModelViewMatrix(), 
                                     transformPipeline.GetProjectionMatrix(), vLightEyePos, R_POLE_COLOR);
        r_poles[i].Draw();
      modelViewMatrix.PopMatrix();
    }
  modelViewMatrix.PopMatrix();

  //printf("----------\n-----------\n------------");

  /* Roller Coaster runner */
  modelViewMatrix.PushMatrix();
    for ( i = 0; i < NUMBER_RUNNER; i++ )
    {
      GLfloat currentRotation = i * 360.0f/NUMBER_RUNNER;

      /* rotate support beam verticle, start in center and spread out at coords x=cos(rot), y=sin(rot) */\
      modelViewMatrix.PushMatrix();
        modelViewMatrix.Rotate(-90, 1.0f, 0.0f, 0.0f);
        modelViewMatrix.Translate(0.0f, 0.0f, r_poleLength[i]-0.69f);
        modelViewMatrix.Translate(cosf(DEG2RAD(currentRotation)), sinf(DEG2RAD(currentRotation)), 0.0f);
        shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, transformPipeline.GetModelViewMatrix(), 
                                     transformPipeline.GetProjectionMatrix(), vLightEyePos, R_POLE_COLOR);
      //circuit[i].Draw();
      modelViewMatrix.PopMatrix();  
    }
  modelViewMatrix.PopMatrix();

  /* Roller Coaster runner track */  
  modelViewMatrix.PushMatrix();
    glLineWidth(16.0f);
    lineLoop.Draw();
  modelViewMatrix.PopMatrix();
}

#endif
