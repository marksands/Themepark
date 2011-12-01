// --------------------------------------------------------------
//
//  Unicorn.h
//
// I had intended to make this a "Lego Land" theme park,
// which is why I built the unicorn out of cubes, aka "legos".
// However, this got too difficult with other animals, and was 
// taking way too long. So I left this one built with cubes, the
// rest are built with standard GLTriangleBatch 3Dpolygons.
//
// --------------------------------------------------------------

#ifndef Firewheel_Unicorn_h
#define Firewheel_Unicorn_h

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

const GLfloat UNICORN_COLOR[] = { .375, 0.2, 0.067, 1.0 };

class Unicorn
{
  public:
    Unicorn();

    void SetupRenderingContext();
    void Update();
    void Draw(GLMatrixStack &modelViewMatrix, GLShaderManager &shaderManager, GLGeometryTransform &transformPipeline, M3DVector4f &vLightEyePos);

  private:
    GLBatch head[3];
    GLBatch kneck[2];
    GLBatch body[4];

    GLBatch hind_leg_right[8];
    GLBatch hind_leg_left[8];
    GLBatch front_leg_right[8];
    GLBatch front_leg_left[8];

    GLBatch tail;
    GLBatch horn;
};

/* ------------------- */
/* Default Constructor */

Unicorn::Unicorn()
{
  
}

/* --------------------------------------------------- */
/* Set up all structures needed to render the Unicorn. */

void Unicorn::SetupRenderingContext()
{
  int i;

  for ( i = 0; i < 2; i++ )  gltMakeCube(kneck[i], 0.05f);
  for ( i = 0; i < 3; i++ )  gltMakeCube(head[i], 0.045f);
  for ( i = 0; i < 4; i++ )  gltMakeCube(body[i], 0.1f);
  for ( i = 0; i < 8; i++ )  gltMakeCube(front_leg_left[i], 0.04);
  for ( i = 0; i < 8; i++ )  gltMakeCube(front_leg_right[i], 0.04);
  for ( i = 0; i < 8; i++ )  gltMakeCube(hind_leg_left[i], 0.04);
  for ( i = 0; i < 8; i++ )  gltMakeCube(hind_leg_right[i], 0.04);
}

/* ---------------------------------------------- */
/* Timer-driven function to update what's changed */

void Unicorn::Update()
{
  
}

/* ------------------------------------- */
/* Render the components of the Unicorn. */

void Unicorn::Draw(GLMatrixStack &modelViewMatrix, GLShaderManager &shaderManager, GLGeometryTransform &transformPipeline, M3DVector4f &vLightEyePos)
{
	// Get the light position in eye space
	M3DVector4f	vLightTransformed;
	M3DMatrix44f mCamera;
	modelViewMatrix.GetMatrix(mCamera);
	m3dTransformVector4(vLightTransformed, LIGHT_POSITION, mCamera);

  int i;

	modelViewMatrix.PushMatrix();

  /* ------------------- */
  /* -- Unicorn Kneck -- */

  for ( i = 0; i < 2; i++ ) {
    modelViewMatrix.PushMatrix();
      modelViewMatrix.Translate(0.2, -0.2, 0.0);
      modelViewMatrix.PushMatrix(); 
        modelViewMatrix.Rotate(45, 0.0, 0.0, 1.0);
        modelViewMatrix.Translate(0.4 + i * 0.05, 0.05, 0);
        shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(), 
                                     transformPipeline.GetProjectionMatrix(), vLightTransformed, UNICORN_COLOR, 0);
        kneck[i].Draw();
      modelViewMatrix.PopMatrix();    
    modelViewMatrix.PopMatrix();
  }

  /* ------------------- */
  /* -- Unicorn Head  -- */

  for ( i = 0; i < 3; i++ ) {
    modelViewMatrix.PushMatrix();
      modelViewMatrix.Translate(0.02, 0.25, 0.0);
      modelViewMatrix.PushMatrix(); 
        modelViewMatrix.Rotate(-22, 0.0, 0.0, 1.0);
        modelViewMatrix.Translate(0.5 + i*0.045, 0.1, 0);
        shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(), 
                                     transformPipeline.GetProjectionMatrix(), vLightTransformed, UNICORN_COLOR, 0);
        head[i].Draw();
      modelViewMatrix.PopMatrix();    
    modelViewMatrix.PopMatrix();
  }

  /* ------------------- */
  /* -- Unicorn Body -- */

  for ( i = 0; i < 4; i++ ) {

    modelViewMatrix.PushMatrix();
      modelViewMatrix.Translate( i * 0.1, 0.0, 0.0);
      shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
                                   transformPipeline.GetProjectionMatrix(), vLightTransformed, UNICORN_COLOR, 0);
      body[i].Draw();
    modelViewMatrix.PopMatrix();
  }

  /* ------------------------------- */
  /* -- Unicorn Front Leg - Right -- */

  for ( i = 0; i < 4; i++ ) {
    /* front leg right - top */
    modelViewMatrix.PushMatrix();
      modelViewMatrix.Translate(0.3, -0.2 + i*0.02, 0.05);
      shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
                                   transformPipeline.GetProjectionMatrix(), vLightTransformed, UNICORN_COLOR, 0);
      front_leg_right[i].Draw();
    modelViewMatrix.PopMatrix();
  }
  for ( i = 4; i < 8; i++ ) {
    /* front leg right - bottom */
    modelViewMatrix.PushMatrix();
    modelViewMatrix.Translate(0.15, 0.2, 0.0);
      modelViewMatrix.PushMatrix();
        modelViewMatrix.Rotate(-22, 0.0, 0.0, 1.0);
        modelViewMatrix.Translate(0.3, -0.5 + i*0.02, 0.05);
        shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
                                     transformPipeline.GetProjectionMatrix(), vLightTransformed, UNICORN_COLOR, 0);
        front_leg_right[i].Draw();
      modelViewMatrix.PopMatrix();
    modelViewMatrix.PopMatrix();
  }

  /* ------------------------------ */
  /* -- Unicorn Front Leg - Left -- */

  for ( i = 0; i < 4; i++ ) {
    /* front leg left - top */
    modelViewMatrix.PushMatrix();
      modelViewMatrix.Translate(0.05, -0.2, 0.0);
      modelViewMatrix.PushMatrix();
        modelViewMatrix.Rotate(22, 0.0, 0.0, 1.0);
        modelViewMatrix.Translate(0.3, -0.1 + i*0.02, -0.05);
        shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
                                     transformPipeline.GetProjectionMatrix(), vLightTransformed, UNICORN_COLOR, 0);
        front_leg_left[i].Draw();
      modelViewMatrix.PopMatrix();
    modelViewMatrix.PopMatrix();
  }
  for ( i = 4; i < 8; i++ ) {
    /* front leg right - bottom */
    modelViewMatrix.PushMatrix();
      modelViewMatrix.Translate(0.375, -0.375 + i*0.02, -0.05);
      shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
                                   transformPipeline.GetProjectionMatrix(), vLightTransformed, UNICORN_COLOR, 0);
      hind_leg_right[i].Draw();
    modelViewMatrix.PopMatrix();
  }

  /* ------------------------------- */
  /* -- Unicorn Hind Leg - Right -- */

  for ( i = 0; i < 4; i++ ) {
    /* hind leg right - top */
    modelViewMatrix.PushMatrix();
      modelViewMatrix.Translate(-0.3, -0.2, 0.0);
      modelViewMatrix.PushMatrix();
        modelViewMatrix.Rotate(22, 0.0, 0.0, 1.0);
        modelViewMatrix.Translate(0.3, -0.1 + i*0.02, 0.05);
        shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
                                     transformPipeline.GetProjectionMatrix(), vLightTransformed, UNICORN_COLOR, 0);
        hind_leg_right[i].Draw();
      modelViewMatrix.PopMatrix();
    modelViewMatrix.PopMatrix();
  }
  for ( i = 4; i < 8; i++ ) {
    /* hind leg right - bottom */
    modelViewMatrix.PushMatrix();
      modelViewMatrix.Translate(0.025, -0.375 + i*0.02, 0.05);
      shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
                                   transformPipeline.GetProjectionMatrix(), vLightTransformed, UNICORN_COLOR, 0);
      hind_leg_right[i].Draw();
    modelViewMatrix.PopMatrix();
  }

  /* ------------------------------- */
  /* -- Unicorn Hind Leg - Left -- */

  for ( i = 0; i < 4; i++ ) {
    /* hind leg left - top */
    modelViewMatrix.PushMatrix();
      modelViewMatrix.Translate(0.0, -0.2 + i*0.02, -0.05);
      shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
                                   transformPipeline.GetProjectionMatrix(), vLightTransformed, UNICORN_COLOR, 0);
      hind_leg_left[i].Draw();
    modelViewMatrix.PopMatrix();
  }
  for ( i = 4; i < 8; i++ ) {
    /* hind leg left - bottom */
    modelViewMatrix.PushMatrix();
      modelViewMatrix.Translate(-0.15, 0.2, 0.0);
      modelViewMatrix.PushMatrix();
        modelViewMatrix.Rotate(-22, 0.0, 0.0, 1.0);
        modelViewMatrix.Translate(0.3, -0.5 + i*0.02, -0.05);
        shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
                                     transformPipeline.GetProjectionMatrix(), vLightTransformed, UNICORN_COLOR, 0);
        hind_leg_left[i].Draw();
      modelViewMatrix.PopMatrix();
    modelViewMatrix.PopMatrix();
  }

  modelViewMatrix.PopMatrix();
}

#endif
