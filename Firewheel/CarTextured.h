//
//  CarTextured.h
//  Firewheel
//
//  Created by Mark Sands on 11/9/11.
//  Copyright (c) 2011 Mark Sands. All rights reserved.
//
////////////////////////////////////////////////////
// Implementation of the Ferris Wheel's Car Class //
///////////////////////////////////////////////////

#ifndef CAR_H
#define CAR_H

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

const GLfloat BAR_BASE_RADIUS = 0.003f;
const GLfloat BAR_TOP_RADIUS  = 0.003f;
const GLfloat BAR_LENGTH      = 0.1f;
const int BAR_NUMBER_SLICES   = 10;
const int BAR_NUMBER_STACKS   = 10;

const GLfloat ROOF_BASE_RADIUS = 0.0f;
const GLfloat ROOF_TOP_RADIUS  = 0.045f;
const GLfloat ROOF_LENGTH      = 0.02f;
const int ROOF_NUMBER_SLICES   = 10;
const int ROOF_NUMBER_STACKS   = 10;

const GLfloat POLE_BASE_RADIUS = 0.005f;
const GLfloat POLE_TOP_RADIUS  = 0.005f;
const GLfloat POLE_LENGTH      = 0.1f;
const int POLE_NUMBER_SLICES   = 10;
const int POLE_NUMBER_STACKS   = 10;

const GLfloat FLOOR_SCALE[] = { 1.0f, 1.0f, 0.4f };
const GLfloat FLOOR_RADIUS    = 0.0375f;
const int FLOOR_NUMBER_SLICES = 10;
const int FLOOR_NUMBER_STACKS = 10;

const GLfloat WALL_BASE_RADIUS = 0.04f;
const GLfloat WALL_TOP_RADIUS  = 0.04f;
const GLfloat WALL_LENGTH      = 0.04f;
const int WALL_NUMBER_SLICES   = 10;
const int WALL_NUMBER_STACKS   = 10;

const GLfloat WHITE_COLOR[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat LIGHT_POSITION[] = { 2.0f, 8.0f, 5.0f, 1.0f };

// Utility Function: Generate a random integer value between the two parameterized values.
int GenerateRandomNumber(int lowerBound, int upperBound);
int GenerateRandomNumber(int lowerBound, int upperBound)
{
	static bool firstTime = true;
	static time_t randomNumberSeed;

	if (firstTime)
	{
		time(&randomNumberSeed);
		firstTime = false;
		srand((unsigned int)randomNumberSeed);
	}

	return ( lowerBound + rand() % (upperBound - lowerBound + 1) );
}

class Car
{
	public:
		Car();
		void SetupRenderingContext();
		void Update();
		void Draw(GLMatrixStack &modelViewMatrix, GLShaderManager &shaderManager, GLGeometryTransform &transformPipeline,
			      M3DVector4f &vLightEyePos, GLfloat totalCarRot, GLuint wallTexture, GLuint carTexture[]);
	private:
		GLTriangleBatch barBatch;
		GLTriangleBatch roofBatch;
		GLTriangleBatch poleBatch;
		GLTriangleBatch floorBatch;
		GLTriangleBatch wallBatch;
};

// Default Constructor
Car::Car()
{
}

// Set up all structures needed to render the Ferris wheel car objects.
void Car::SetupRenderingContext()
{
	gltMakeCylinder(barBatch, BAR_BASE_RADIUS, BAR_TOP_RADIUS, BAR_LENGTH, BAR_NUMBER_SLICES, BAR_NUMBER_STACKS);
	gltMakeCylinder(roofBatch, ROOF_BASE_RADIUS, ROOF_TOP_RADIUS, ROOF_LENGTH, ROOF_NUMBER_SLICES, ROOF_NUMBER_STACKS);
	gltMakeCylinder(poleBatch, POLE_BASE_RADIUS, POLE_TOP_RADIUS, POLE_LENGTH, POLE_NUMBER_SLICES, POLE_NUMBER_STACKS);
	gltMakeSphere(floorBatch, FLOOR_RADIUS, FLOOR_NUMBER_SLICES, FLOOR_NUMBER_STACKS);
	gltMakeCylinder(wallBatch, WALL_BASE_RADIUS, WALL_TOP_RADIUS, WALL_LENGTH, WALL_NUMBER_SLICES, WALL_NUMBER_STACKS);
}

// Timer-driven function to update positions, orientations, 
// etc., of all changing parts of the Ferris wheel car.
void Car::Update()
{
}

// Render the components of the Ferris wheel car.
void Car::Draw(GLMatrixStack &modelViewMatrix, GLShaderManager &shaderManager, GLGeometryTransform &transformPipeline,
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
			barBatch.Draw();
		modelViewMatrix.PopMatrix();

		modelViewMatrix.Rotate(-totalCarRot, 0.0f, 0.0f, 1.0f);
		modelViewMatrix.Rotate(90.0f, 1.0f, 0.0f, 0.0f);
		modelViewMatrix.PushMatrix();
		glBindTexture(GL_TEXTURE_2D, carTexture[1]);
		shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
											transformPipeline.GetProjectionMatrix(), vLightTransformed, WHITE_COLOR, 0);
		roofBatch.Draw();
		modelViewMatrix.PopMatrix();
		
		modelViewMatrix.PushMatrix();
			glBindTexture(GL_TEXTURE_2D, carTexture[2]);
			shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
												transformPipeline.GetProjectionMatrix(), vLightTransformed, WHITE_COLOR, 0);
			poleBatch.Draw();
		modelViewMatrix.PopMatrix();

		modelViewMatrix.PushMatrix();
			glBindTexture(GL_TEXTURE_2D, carTexture[4]);
			modelViewMatrix.Translate(0.0f, 0.0f, POLE_LENGTH);
			modelViewMatrix.Scale(FLOOR_SCALE[0], FLOOR_SCALE[1], FLOOR_SCALE[2]);
			shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
											transformPipeline.GetProjectionMatrix(), vLightTransformed, WHITE_COLOR, 0);
			floorBatch.Draw();
		modelViewMatrix.PopMatrix();

		modelViewMatrix.PushMatrix();
			glBindTexture(GL_TEXTURE_2D, wallTexture);
			modelViewMatrix.Translate(0.0f, 0.0f, POLE_LENGTH);// POLE_LENGTH - WALL_LENGTH);
			modelViewMatrix.Rotate(90.0f, 0.0f, 0.0f, 1.0f);
			modelViewMatrix.Rotate(180.0f, 0.0f, 1.0f, 0.0f);
			shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
											transformPipeline.GetProjectionMatrix(), vLightTransformed, WHITE_COLOR, 0);
			wallBatch.Draw();
		modelViewMatrix.PopMatrix();
	modelViewMatrix.PopMatrix();
}

#endif