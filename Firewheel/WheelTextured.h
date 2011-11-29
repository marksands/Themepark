//
//  GLUTKeyCodes.h
//  Firewheel
//
//  Created by Mark Sands on 11/9/11.
//  Copyright (c) 2011 Mark Sands. All rights reserved.
//
//////////////////////////////////////////////
// Implementation of the Ferris Wheel Class //
//////////////////////////////////////////////

#ifndef WHEEL_H
#define WHEEL_H

#include <GLTools.h>
#include <GLShaderManager.h>
#include <GLFrustum.h>
#include <GLBatch.h>
#include <GLFrame.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <cmath>
#include "CarTextured.h"

GLfloat WHITE_VECTOR[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LIGHT_LOCATION[] = { 2.0f, 8.0f, 5.0f, 1.0f };

const int NBR_CARS = 24;

const GLfloat RING_COLOR[]  = { 0.6f, 0.6f, 0.6f, 1.0f };
const GLfloat RING_MAJOR_RADIUS = 0.5f;
const GLfloat RING_MINOR_RADIUS = 0.005f;
const int RING_NBR_MAJOR_SLICES = 90;
const int RING_NBR_MINOR_SLICES = 20;

const GLfloat AXLE_COLOR[]  = { 0.3f, 0.3f, 0.9f, 1.0f };
const GLfloat AXLE_BASE_RADIUS = 0.07f;
const GLfloat AXLE_TOP_RADIUS  = 0.07f;
const GLfloat AXLE_LENGTH      = 0.1f;
const int AXLE_NUMBER_SLICES   = 20;
const int AXLE_NUMBER_STACKS   = 20;

const GLfloat CAP_COLOR[]   = { 0.3f, 0.3f, 0.9f, 1.0f };
const GLfloat CAP_BASE_RADIUS = 0.0f;
const GLfloat CAP_TOP_RADIUS  = 0.075f;
const GLfloat CAP_LENGTH      = 0.005f;
const int CAP_NUMBER_SLICES   = 30;
const int CAP_NUMBER_STACKS   = 30;

const GLfloat SPOKE_COLOR[] = { 0.6f, 0.6f, 0.6f, 1.0f };
const GLfloat SPOKE_BASE_RADIUS = 0.005f;
const GLfloat SPOKE_TOP_RADIUS  = 0.005f;
const GLfloat SPOKE_LENGTH      = 0.5f;
const int SPOKE_NUMBER_SLICES   = 10;
const int SPOKE_NUMBER_STACKS   = 10;

const GLfloat STAND_COLOR[] = { 0.6f, 0.6f, 0.0f, 1.0f };
const GLfloat STAND_BASE_RADIUS = 0.01f;
const GLfloat STAND_TOP_RADIUS  = 0.01f;
const GLfloat STAND_LENGTH      = 0.85f;
const int STAND_NUMBER_SLICES   = 20;
const int STAND_NUMBER_STACKS   = 20;

const GLfloat INITIAL_WHEEL_ROTATION = 0.0f;
const GLfloat INITIAL_WHEEL_ROTATION_INCREMENT = 2.0f;
const GLfloat WHEEL_WIDTH = 0.1f;
const GLfloat CAP_ELEVATION     = 0.511f * WHEEL_WIDTH;
const GLfloat RING_ELEVATION    = 0.51f * WHEEL_WIDTH;
const GLfloat SPOKE_ELEVATION   = 0.45f * WHEEL_WIDTH;
const GLfloat STAND_STANDARD_X_ROTATION = 45.0f;
const GLfloat STAND_STANDARD_Y_ROTATION = 135.0f;
const GLfloat STAND_STANDARD_Y_OFFSET   = -0.05f;
const GLfloat STAND_STANDARD_Z_OFFSET   = 0.04f;

class Wheel
{
	public:
		Wheel();
		void SetupRenderingContext();
		void Update();
		void Draw(GLMatrixStack &modelViewMatrix, GLShaderManager &shaderManager, GLGeometryTransform &transformPipeline, M3DVector4f &vLightEyePos,
						GLuint capTexture[], GLuint wheelTexture[], GLuint wallTexture[][4], GLuint carTexture[], int currentTextureIndex);
	private:
		GLTriangleBatch ringBatch[2];
		GLTriangleBatch axleBatch;
		GLTriangleBatch capBatch[2];
		GLTriangleBatch spokeBatch[2][NBR_CARS];
		GLTriangleBatch standBatch[2][2];
		Car wheelCar[NBR_CARS];
		float currentRotation;
		float rotationIncrement;
};

// Default Constructor
Wheel::Wheel()
{
}

// Set up all structures needed to render the Ferris wheel objects.
void Wheel::SetupRenderingContext()
{
	int i, j;
	for (i = 0; i < 2; i++)
		gltMakeTorus(ringBatch[i], RING_MAJOR_RADIUS, RING_MINOR_RADIUS, RING_NBR_MAJOR_SLICES, RING_NBR_MINOR_SLICES);
	gltMakeCylinder(axleBatch, AXLE_BASE_RADIUS, AXLE_TOP_RADIUS, AXLE_LENGTH, AXLE_NUMBER_SLICES, AXLE_NUMBER_STACKS);
	for (i = 0; i < 2; i++)
		gltMakeDisk(capBatch[i], CAP_BASE_RADIUS, CAP_TOP_RADIUS, CAP_NUMBER_SLICES, CAP_NUMBER_STACKS);
	for (i = 0; i < 2; i++)
		for (j = 0; j < NBR_CARS; j++)
			gltMakeCylinder(spokeBatch[i][j], SPOKE_BASE_RADIUS, SPOKE_TOP_RADIUS, SPOKE_LENGTH, SPOKE_NUMBER_SLICES, SPOKE_NUMBER_STACKS);
	for (i = 0; i < 2; i++)
		for (j = 0; j < 2; j++)
			gltMakeCylinder(standBatch[i][j], STAND_BASE_RADIUS, STAND_TOP_RADIUS, STAND_LENGTH, STAND_NUMBER_SLICES, STAND_NUMBER_STACKS);

	for (i = 0; i < NBR_CARS; i++)
		wheelCar[i].SetupRenderingContext();

	currentRotation = INITIAL_WHEEL_ROTATION;
	rotationIncrement = INITIAL_WHEEL_ROTATION_INCREMENT;
}

// Timer-driven function to update positions, orientations, 
// etc., of all changing parts of the Ferris wheel.
void Wheel::Update()
{
	currentRotation += rotationIncrement;
	if (currentRotation > 360.0f)
		currentRotation -= 360.0f;

	for (int i = 0; i < NBR_CARS; i++)
		wheelCar[i].Update();
}

// Render the components of the Ferris wheel.
void Wheel::Draw(GLMatrixStack &modelViewMatrix, GLShaderManager &shaderManager, GLGeometryTransform &transformPipeline, M3DVector4f &vLightEyePos, 
						GLuint capTexture[], GLuint wheelTexture[], GLuint wallTexture[][4], GLuint carTexture[], int currentTextureIndex)
{
	// Get the light position in eye space
	M3DVector4f	vLightTransformed;
	M3DMatrix44f mCamera;
	modelViewMatrix.GetMatrix(mCamera);
	m3dTransformVector4(vLightTransformed, LIGHT_LOCATION, mCamera);

	int i, j;
	GLfloat carRotation;

	// Draw the rotated wheel, including the cars, the caps, the axle, and the spokes.
	modelViewMatrix.PushMatrix();
		// Rotate everything in this Push/Pop section so that the wheel appears to spin.
		modelViewMatrix.Rotate(currentRotation, 0.0f, 0.0f, 1.0f);

		// Draw the wheel's axle first (since it's the only part of the wheel that's not paired up).
		modelViewMatrix.PushMatrix();
			modelViewMatrix.Translate(0.0f, 0.0f, -0.5f * AXLE_LENGTH);
			shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, transformPipeline.GetModelViewMatrix(), 
							transformPipeline.GetProjectionMatrix(), vLightEyePos, AXLE_COLOR);
			axleBatch.Draw();
		modelViewMatrix.PopMatrix();

		// The rest of the wheel is drawn in pairs of objects.
		for (i = 0; i < 2; i++)
		{
			modelViewMatrix.PushMatrix();
				// Move one ring of the wheel towards the initial viewer position, the other away.
				glBindTexture(GL_TEXTURE_2D, wheelTexture[1]);
				modelViewMatrix.PushMatrix();
					if (i == 0)
						modelViewMatrix.Translate(0.0f, 0.0f, -0.5f * WHEEL_WIDTH);
					else
						modelViewMatrix.Translate(0.0f, 0.0f, 0.5f * WHEEL_WIDTH);
					shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
													transformPipeline.GetProjectionMatrix(), vLightTransformed, WHITE_VECTOR, 0);
					ringBatch[i].Draw();
				modelViewMatrix.PopMatrix();

				// Draw the current ring cap (one slightly closer to the viewer's initial; position, the other slightly farther away.
				glBindTexture(GL_TEXTURE_2D, capTexture[currentTextureIndex]);
				modelViewMatrix.PushMatrix();
					if (i == 0)
						modelViewMatrix.Translate(0.0f, 0.0f, -CAP_ELEVATION);
					else
						modelViewMatrix.Translate(0.0f, 0.0f, CAP_ELEVATION);
					modelViewMatrix.Rotate(-currentRotation, 0.0f, 0.0f, 1.0f);
					shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
													transformPipeline.GetProjectionMatrix(), vLightTransformed, WHITE_VECTOR, 0);
					capBatch[i].Draw();
				modelViewMatrix.PopMatrix();

				// Draw the spokes for the current ring.
				glBindTexture(GL_TEXTURE_2D, wheelTexture[0]);
				modelViewMatrix.PushMatrix();
					if (i == 0)
						modelViewMatrix.Translate(0.0f, 0.0f, -SPOKE_ELEVATION);
					else
						modelViewMatrix.Translate(0.0f, 0.0f, SPOKE_ELEVATION);
					for (j = 0; j < NBR_CARS; j++)
					{
						modelViewMatrix.PushMatrix();
							modelViewMatrix.Rotate(360.0f * j / NBR_CARS, 0.0f, 0.0f, 1.0f);
							modelViewMatrix.Rotate(90.0f, 0.0f, 1.0f, 0.0f);
							shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
														transformPipeline.GetProjectionMatrix(), vLightTransformed, WHITE_VECTOR, 0);
							spokeBatch[i][j].Draw();
						modelViewMatrix.PopMatrix();
					}
				modelViewMatrix.PopMatrix();

			modelViewMatrix.PopMatrix();
		}

		// Draw the wheel's cars at regular intervals.
		for (i = 0; i < NBR_CARS; i++)
		{
			modelViewMatrix.PushMatrix();
				carRotation = i * 360.0f / NBR_CARS;
				modelViewMatrix.Rotate(carRotation, 0.0f, 0.0f, 1.0f);
				modelViewMatrix.Translate(SPOKE_LENGTH, 0.0f, 0.0f);
				wheelCar[i].Draw(modelViewMatrix, shaderManager, transformPipeline, vLightEyePos, 
									currentRotation + carRotation, wallTexture[currentTextureIndex][i%4], carTexture);
			modelViewMatrix.PopMatrix();
		}
	modelViewMatrix.PopMatrix();

	// Draw the unrotated stand for the wheel.
	for (i = 0; i < 2; i++)
		for (j = 0; j < 2; j++)
		{
			glBindTexture(GL_TEXTURE_2D, wheelTexture[2]);
			modelViewMatrix.PushMatrix();
				modelViewMatrix.Translate(0.0f, STAND_STANDARD_Y_OFFSET, STAND_STANDARD_Z_OFFSET * (2 * i - 1));                  // Translate z -0.01 for i=0, +0.01 for i=1
				modelViewMatrix.Rotate((STAND_STANDARD_Y_ROTATION - 90.0f * i) * (2 * j - 1), 0.0, 1.0, 0.0);  // Rotate -135 for (0,0), 135 for (0,1), -45 for (1,0), and 45 for (1,1)
				modelViewMatrix.Rotate(STAND_STANDARD_X_ROTATION, 1.0f, 0.0f, 0.0f);
				shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, modelViewMatrix.GetMatrix(),
												transformPipeline.GetProjectionMatrix(), vLightTransformed, WHITE_VECTOR, 0);
				standBatch[i][j].Draw();
			modelViewMatrix.PopMatrix();
		}
}

#endif