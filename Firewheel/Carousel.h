//
//  Carousel.h
//  Firewheel
//
//  Created by Mark Sands on 11/29/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef Firewheel_Carousel_h
#define Firewheel_Carousel_h

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

/* ROOF CAP */
const GLfloat ROOF_CAP_COLOR[] = { 0.8f, 0.8f, 0.2f, 1.0f };
const GLfloat ROOF_CAP_BASE_RADIUS = 1.0f;
const GLfloat ROOF_CAP_TOP_RADIUS = 0.0f;
const GLfloat ROOF_CAP_LENGTH = 0.25f;
const GLint ROOF_CAP_NBR_SLICES = 30;
const GLint ROOF_CAP_NBR_STACKS = 30;

/* CENTER PIECE */
const GLfloat CENTER_PIECE_COLOR[] = { 0.8f, 0.8f, 0.2f, 1.0f };
const GLfloat CENTER_BASE_RADIUS = 0.275f;
const GLfloat CENTER_TOP_RADIUS = 0.275f;
const GLfloat CENTER_LENGTH = 1.0f;
const GLint CENTER_NUMBER_SLICES = 10;
const GLint CENTER_NUMBER_STACKS = 10;

/* BOTTOM + CAP */
const GLfloat BOTTOM_COLOR[] = { 0.8f, 0.8f, 0.2f, 1.0f };

const GLfloat BOTTOM_CAP_INNER_RADIUS = 0.0f;
const GLfloat BOTTOM_CAP_OUTER_RADIUS = 1.0f;
const GLint BOTTOM_CAP_NBR_SLICES = 30;
const GLint BOTTOM_CAP_NBR_STACKS = 30;

const GLfloat BOTTOM_BASE_RADIUS = 1.0f;
const GLfloat BOTTOM_TOP_RADIUS = 1.0f;
const GLfloat BOTTOM_LENGTH = 0.1f;
const GLint BOTTOM_NBR_SLICES = 30;
const GLint BOTTOM_NBR_STACKS = 30;

/* RIDE POLES */
const GLint NBR_RIDE_POLES = 4;
const GLfloat RIDE_POLE_BASE_RADIUS = 0.02f;
const GLfloat RIDE_POLE_TOP_RADIUS = 0.02f;
const GLfloat RIDE_POLE_LENGTH = 0.9f;
const GLint RIDE_POLE_NBR_SLICES = 10;
const GLint RIDE_POLE_NBR_STACKS = 10;

/* DATA FILE */
// const char* COASTER_DATA_FILE = "roller_coaster_data.txt";

/* CAROUSEL */
class Carousel
{
public:
  Carousel();
  ~Carousel();

  void SetupRenderingContext();
  void Update();
  void Draw(GLMatrixStack &modelViewMatrix, GLShaderManager &shaderManager, GLGeometryTransform &transformPipeline, M3DVector4f &vLightEyePos);

private:
  GLTriangleBatch roofcap;
  GLTriangleBatch centerpiece;

  GLTriangleBatch bottom;
  GLTriangleBatch bottomcap;

  GLTriangleBatch ridePoles[NBR_RIDE_POLES];
  /* animals */
};

Carousel::Carousel()
{
  
}

Carousel::~Carousel()
{
  
}

void Carousel::SetupRenderingContext()
{
  /* carousel roof */
  gltMakeCylinder(roofcap, ROOF_CAP_BASE_RADIUS, ROOF_CAP_TOP_RADIUS, ROOF_CAP_LENGTH, ROOF_CAP_NBR_SLICES, ROOF_CAP_NBR_STACKS);
  
  /* carousel center piece */
  gltMakeCylinder(centerpiece, CENTER_BASE_RADIUS, CENTER_TOP_RADIUS, CENTER_LENGTH, CENTER_NUMBER_SLICES, CENTER_NUMBER_STACKS);
  
  /* carousel botom + cap */
  gltMakeDisk(bottomcap, BOTTOM_CAP_INNER_RADIUS, BOTTOM_CAP_OUTER_RADIUS, BOTTOM_CAP_NBR_SLICES, BOTTOM_CAP_NBR_STACKS);
  gltMakeCylinder(bottom, BOTTOM_BASE_RADIUS, BOTTOM_TOP_RADIUS, BOTTOM_LENGTH, BOTTOM_NBR_SLICES, BOTTOM_NBR_STACKS);

  /* carousel ride poles */
  for ( int i = 0; i < NBR_RIDE_POLES; i++ )
    gltMakeCylinder(ridePoles[i], RIDE_POLE_BASE_RADIUS, RIDE_POLE_TOP_RADIUS, RIDE_POLE_LENGTH, RIDE_POLE_NBR_SLICES, RIDE_POLE_NBR_STACKS);
}

void Carousel::Update()
{
  
}

void Carousel::Draw(GLMatrixStack &modelViewMatrix, GLShaderManager &shaderManager, GLGeometryTransform &transformPipeline, M3DVector4f &vLightEyePos)
{
  modelViewMatrix.PushMatrix();
  
    /* Everything is drawn on its side, this tilts the structure upright */
    modelViewMatrix.Rotate(-90.0f, 1.0f, 0.0f, 0.0f);

    /* Floor carousel bottom */
    modelViewMatrix.PushMatrix();
      modelViewMatrix.Translate(0.0f, 0.0f, -0.70f);
      shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, transformPipeline.GetModelViewMatrix(), 
                                   transformPipeline.GetProjectionMatrix(), vLightEyePos, BOTTOM_COLOR);
      bottom.Draw();
    modelViewMatrix.PopMatrix();

    /* Floor carousel cap */
    modelViewMatrix.PushMatrix();
      modelViewMatrix.Translate(0.0f, 0.0f, -0.60f);
      shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, transformPipeline.GetModelViewMatrix(), 
                                 transformPipeline.GetProjectionMatrix(), vLightEyePos, BOTTOM_COLOR);
      bottomcap.Draw();
    modelViewMatrix.PopMatrix();

    /* rotate support beam verticle, start in center and spread out at coords x=cos(rot), y=sin(rot) */
    modelViewMatrix.PushMatrix();
      modelViewMatrix.Translate(0.0f, 0.0f, -0.70f);
      shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, transformPipeline.GetModelViewMatrix(), 
                                   transformPipeline.GetProjectionMatrix(), vLightEyePos, CENTER_PIECE_COLOR);
      centerpiece.Draw();
    modelViewMatrix.PopMatrix();
    
    /* carousel ride poles */
    for ( int i = 0; i < NBR_RIDE_POLES; i++ )
    {
      int rot = i*360.0f/NBR_RIDE_POLES;

      modelViewMatrix.PushMatrix();
        modelViewMatrix.Translate(cos(DEG2RAD(rot))/1.2f, sin(DEG2RAD(rot))/1.2f, 0.0f);
        modelViewMatrix.Translate(0.0f, 0.0f, -0.70f);
        shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, transformPipeline.GetModelViewMatrix(), 
                                     transformPipeline.GetProjectionMatrix(), vLightEyePos, ROOF_CAP_COLOR);
        ridePoles[i].Draw();
      modelViewMatrix.PopMatrix();
    }
    
    /* rotate support beam verticle, start in center and spread out at coords x=cos(rot), y=sin(rot) */
    modelViewMatrix.PushMatrix();
      modelViewMatrix.Translate(0.0f, 0.0f, 0.2f);
      shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, transformPipeline.GetModelViewMatrix(), 
                                   transformPipeline.GetProjectionMatrix(), vLightEyePos, ROOF_CAP_COLOR);
      roofcap.Draw();
    modelViewMatrix.PopMatrix();

  modelViewMatrix.PopMatrix();
}

#endif
