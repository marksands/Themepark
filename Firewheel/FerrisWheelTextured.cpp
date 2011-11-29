//
//  GLUTKeyCodes.h
//  Firewheel
//
//  Created by Mark Sands on 11/9/11.
//  Copyright (c) 2011 Mark Sands. All rights reserved.
//
////////////////////////////////////////////////////////////
// Texture-mapped Ferris wheel with primitive reflection. //
////////////////////////////////////////////////////////////

#include <GLTools.h>
#include <GLShaderManager.h>
#include <GLFrustum.h>
#include <GLBatch.h>
#include <GLFrame.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <cmath>
#include <cstdarg>
#include "WheelTextured.h"
#include "GLUTKeyCodes.h"

//#include "LoadMesh.h"
//#include "Lion.h"


/* DEGRESS TO RADIANS */
const float PI = 3.14159265f;
const float PI_OVER_180 = PI/180.0f;

static inline float DEG2RAD(float degrees) {
  return degrees * PI_OVER_180;
}

#include "Carousel.h"
#include "Track.h"

#ifdef __APPLE__
  #include <glut/glut.h>
#else
  #define FREEGLUT_STATIC
  #include <GL/glut.h>
#endif

#include "SoundManager.h"

/* Disable console on WIN_32 */
#if defined(__WIN32__) || defined(_WIN32)
#pragma comment(linker, "/subsystem:\"windows\" \
/entry:\"mainCRTStartup\"")
#endif

const int   ORIG_WINDOW_SIZE[] = { 1000, 1000 };
const float CAMERA_LINEAR_STEP = 0.1f;
const float CAMERA_ANGULAR_STEP = float(m3dDegToRad(5.0f));
const float FRUSTUM_FIELD_OF_VIEW = 35.0f;
const float FRUSTUM_NEAR_PLANE = 0.1f;
const float FRUSTUM_FAR_PLANE = 100.0f;

const float FLOOR_GRID_WIDTH = 40.0f;
const float FLOOR_GRID_INCREMENT = 0.5f;
const float FLOOR_HEIGHT = -0.65f;
const float REFLECTING_ALPHA = 0.5f;
const float NONREFLECTING_ALPHA = 1.0f;

const float FERRIS_WHEEL_POSITION[] = { 0.0f, 0.0f, -2.5f };
const int   NBR_TEXTURE_SETS = 2;
const int   NBR_WHEEL_TEXTURES = 3;
const int   NBR_WALL_TEXTURES = 4;
const int   NBR_CAR_TEXTURES = 4;
const int   MAX_FILENAME_LENGTH = 20;
const char  GROUND_TEXTURE_FILENAME[MAX_FILENAME_LENGTH] = { "grass.bmp" }; // { "Marble.bmp" };
const char  CAP_TEXTURE_FILENAME[NBR_TEXTURE_SETS][MAX_FILENAME_LENGTH] = {
  "MickeyMouse.bmp", "BugsBunny.bmp"
};
const char  WHEEL_TEXTURE_FILENAME[NBR_WHEEL_TEXTURES][MAX_FILENAME_LENGTH] = {
  "BlueMetal.bmp", "Brass.bmp", "Brass.bmp"
};
const char  WALL_TEXTURE_FILENAME[NBR_TEXTURE_SETS][NBR_WALL_TEXTURES][MAX_FILENAME_LENGTH] = {
  { "Mickey.bmp", "Donald.bmp", "Goofy.bmp", "Pluto.bmp" },
  { "Bugs.bmp", "Porky.bmp", "Marvin.bmp", "Tweety.bmp" }
};
const char  CAR_TEXTURE_FILENAME[NBR_CAR_TEXTURES][MAX_FILENAME_LENGTH] = { 
  "BlueMetal.bmp", "StainedGlass.bmp", "BlueMetal.bmp", "CutStone.bmp"
};
const char  FIRE_TEXTURE_FILENAME[MAX_FILENAME_LENGTH] = "FireParticle.bmp";

GLShaderManager shaderManager; // Shader Manager
GLMatrixStack modelViewMatrix; // Modelview Matrix
GLMatrixStack projectionMatrix; // Projection Matrix
GLFrustum viewFrustum; // View Frustum
GLGeometryTransform	transformPipeline; // Geometry Transform Pipeline

Wheel   theWheel;
bool    fullscreen = false;
bool    reflecting = false;
int     currentTextureIndex = 0;
GLBatch groundBatch;
GLFrame cameraFrame;
GLuint  groundTexture;
GLuint  capTexture[NBR_TEXTURE_SETS];
GLuint  wheelTexture[NBR_WHEEL_TEXTURES];
GLuint  wallTexture[NBR_TEXTURE_SETS][NBR_WALL_TEXTURES];
GLuint  carTexture[NBR_CAR_TEXTURES];

void SetupRenderingContext();
void ShutdownRenderingContext();
bool LoadBMPTexture(const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode);
void ResizeWindow(int nWidth, int nHeight);
void Display();
void DrawGround();
void DrawScene();
void TimerFunction(int value);
void KeyboardPress(unsigned char pressedKey, int mouseXPosition, int mouseYPosition);
void NonASCIIKeyboardPress(int key, int mouseXPosition, int mouseYPosition);

// THE LION
//Lion simba;

// THE TRACK
Track track;

// THE CAROUSEL
Carousel carousel;

char* files[1] = { const_cast<char*>("sample.wav") };

MediaPlayer *SoundEngine = new Balto(files);

// Set up all callback functions and the display environment.
int main(int argc, char* argv[])
{
	gltSetWorkingDirectory(argv[0]);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(ORIG_WINDOW_SIZE[0], ORIG_WINDOW_SIZE[1]);
	glutCreateWindow("Textured Ferris Wheel");

	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
		return -1;
	}

  //SoundEngine->Play( 0, true );

	glutKeyboardFunc( KeyboardPress );
	glutSpecialFunc( NonASCIIKeyboardPress );
	glutReshapeFunc( ResizeWindow );
	glutDisplayFunc( Display );
	glutTimerFunc( 50, TimerFunction, 1 );

	SetupRenderingContext();
	glutMainLoop();
	ShutdownRenderingContext();

	return 0;
}

// Initialize the batches of objects that will be rendered, and create and load the texture objects.
void SetupRenderingContext()
{
	int i, j;

	// Initialze Shader Manager
	shaderManager.InitializeStockShaders();	
	glEnable(GL_DEPTH_TEST);
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClearColor(0.94f, 0.94f, 1.0f, 1.0f); // Blue Sky
  //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	theWheel.SetupRenderingContext();
  //simba.SetupRenderingContext();
  track.SetupRenderingContext();
  carousel.SetupRenderingContext();

	// Make the ground
	GLfloat texSize = 50.0f;
	groundBatch.Begin(GL_TRIANGLE_FAN, 4, 1);
		groundBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
		groundBatch.Vertex3f(-0.5f * FLOOR_GRID_WIDTH, FLOOR_HEIGHT, 0.5f * FLOOR_GRID_WIDTH);	
		groundBatch.MultiTexCoord2f(0, texSize, 0.0f);
		groundBatch.Vertex3f(0.5f * FLOOR_GRID_WIDTH, FLOOR_HEIGHT, 0.5f * FLOOR_GRID_WIDTH);	
		groundBatch.MultiTexCoord2f(0, texSize, texSize);
		groundBatch.Vertex3f(0.5f * FLOOR_GRID_WIDTH, FLOOR_HEIGHT, -0.5f * FLOOR_GRID_WIDTH);	
		groundBatch.MultiTexCoord2f(0, 0.0f, texSize);
		groundBatch.Vertex3f(-0.5f * FLOOR_GRID_WIDTH, FLOOR_HEIGHT, -0.5f * FLOOR_GRID_WIDTH);
	groundBatch.End();

	// Make texture object for ground.
	glGenTextures(1, &groundTexture);
	glBindTexture(GL_TEXTURE_2D, groundTexture);
	LoadBMPTexture(GROUND_TEXTURE_FILENAME, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT);
	
	// Make texture objects for cap.
	glGenTextures(NBR_TEXTURE_SETS, capTexture);
	for ( i = 0; i < NBR_TEXTURE_SETS; i++ )
	{
		glBindTexture(GL_TEXTURE_2D, capTexture[i]);
		LoadBMPTexture(CAP_TEXTURE_FILENAME[i], GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);
	}

	// Make texture objects for wheel components.
	glGenTextures(NBR_WHEEL_TEXTURES, wheelTexture);
	for ( i = 0; i < NBR_WHEEL_TEXTURES; i++ )
	{
		glBindTexture(GL_TEXTURE_2D, wheelTexture[i]);
		LoadBMPTexture(WHEEL_TEXTURE_FILENAME[i], GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);
	}

	// Make texture objects for Ferris wheel car walls.
	for ( i = 0; i < NBR_TEXTURE_SETS; i++ )
	{
		glGenTextures(NBR_WALL_TEXTURES, wallTexture[i]);
		for ( j = 0; j < NBR_WALL_TEXTURES; j++ )
		{
			glBindTexture(GL_TEXTURE_2D, wallTexture[i][j]);
			LoadBMPTexture(WALL_TEXTURE_FILENAME[i][j], GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);
		}
	}

	// Make texture objects for car components.
	glGenTextures(NBR_CAR_TEXTURES, carTexture);
	for ( i = 0; i < NBR_CAR_TEXTURES; i++ )
	{
		glBindTexture(GL_TEXTURE_2D, carTexture[i]);
		LoadBMPTexture(CAR_TEXTURE_FILENAME[i], GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);
	}
}

// Remove all remnants of the textures that were used.
void ShutdownRenderingContext()
{
	glDeleteTextures(1, &groundTexture);
	glDeleteTextures(NBR_TEXTURE_SETS, capTexture);
	glDeleteTextures(NBR_WHEEL_TEXTURES, wheelTexture);
	for (int i = 0; i < NBR_TEXTURE_SETS; i++)
		glDeleteTextures(NBR_WALL_TEXTURES, wallTexture[i]);
	glDeleteTextures(NBR_CAR_TEXTURES, carTexture);
}

// Load in a BMP file as a texture. Allows specification of the filters and the wrap mode
bool LoadBMPTexture(const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode)	
{
	GLbyte *pBits;
	GLint iWidth, iHeight;

	pBits = gltReadBMPBits(szFileName, &iWidth, &iHeight);
	if(pBits == NULL)
		return false;

    // Set Wrap modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, pBits);

  free(pBits);
  
  switch (minFilter) {
    case GL_LINEAR_MIPMAP_LINEAR:
    case GL_LINEAR_MIPMAP_NEAREST:
    case GL_NEAREST_MIPMAP_LINEAR:
    case GL_NEAREST_MIPMAP_NEAREST:
      glGenerateMipmap(GL_TEXTURE_2D);
  }

	return true;
}

// Callback for handling display screen reshaping.
void ResizeWindow(int nWidth, int nHeight)
{
	glViewport(0, 0, nWidth, nHeight);

	// Create the projection matrix, and load it on the projection matrix stack
	viewFrustum.SetPerspective(FRUSTUM_FIELD_OF_VIEW, float(nWidth)/float(nHeight), FRUSTUM_NEAR_PLANE, FRUSTUM_FAR_PLANE);
	projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());

	// Set the transformation pipeline to use the two matrix stacks 
	transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
}

// Called to draw the scene
void Display()
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Save the current modelview matrix (the identity matrix)
	modelViewMatrix.PushMatrix();	
		M3DMatrix44f mCamera;
		cameraFrame.GetCameraMatrix(mCamera);
		modelViewMatrix.MultMatrix(mCamera);

		if (reflecting)
		{
			// Draw the "reflection" of the scene upside down
			modelViewMatrix.PushMatrix();

				// Flip the y-axis last.
				modelViewMatrix.Scale(1.0f, -1.0f, 1.0f);

				// The scene is essentially in a pit, bo elevate it an equal distance from the
				// x-z plane to ensure that its reflection will appear to be below the ground.
				modelViewMatrix.Translate(0.0f, -2.0f * FLOOR_HEIGHT, 0.0f);

				// Reverse the orientation of all polygonsm in the scene so the orientation of
				// their reflections will produce the same lighting as the above-ground scene.
				glFrontFace(GL_CW);
				DrawScene();
				glFrontFace(GL_CCW);

			modelViewMatrix.PopMatrix();
		}

  DrawGround();
  DrawScene();

	modelViewMatrix.PopMatrix();

  /*
  modelViewMatrix.PushMatrix();

    const int indices[] = {
      1, 2, 3, 4, 5, 6, 7, 8, 9
    };
  
    glDrawElements(GL_TRIANGLE_STRIP, 14, GL_UNSIGNED_SHORT, indices);

  modelViewMatrix.PopMatrix();
   */

	// Do the buffer Swap
	glutSwapBuffers();

	// Tell GLUT to do it again
	glutPostRedisplay();
}

// Renders the texture-mapped ground.
void DrawGround()
{
	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, groundTexture);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	static GLfloat vFloorColor[] = { 1.0f, 1.0f, 1.0f, REFLECTING_ALPHA};
	if (reflecting)
		vFloorColor[3] = REFLECTING_ALPHA;
	else
		vFloorColor[3] = NONREFLECTING_ALPHA;
	shaderManager.UseStockShader(GLT_SHADER_TEXTURE_MODULATE, transformPipeline.GetModelViewProjectionMatrix(), vFloorColor, 0);	
	groundBatch.Draw();
	glDisable(GL_BLEND);
}

// Renders the Ferris wheel objects via Wheel's Draw routine.
void DrawScene()
{
	modelViewMatrix.PushMatrix();	
		M3DMatrix44f mCamera;
		cameraFrame.GetCameraMatrix(mCamera);

		// Transform the light position into eye coordinates
		M3DVector4f vLightPos = { LIGHT_POSITION[0], LIGHT_POSITION[1], LIGHT_POSITION[2], LIGHT_POSITION[3] };
		M3DVector4f vLightEyePos;
		m3dTransformVector4(vLightEyePos, vLightPos, mCamera);

		// Position the ferris wheel appropriately.
		modelViewMatrix.Translate(FERRIS_WHEEL_POSITION[0], FERRIS_WHEEL_POSITION[1], FERRIS_WHEEL_POSITION[2]);

		// Apply the Translation to this entire block of objects
		modelViewMatrix.PushMatrix();
      //theWheel.Draw(modelViewMatrix, shaderManager, transformPipeline, vLightEyePos, capTexture, wheelTexture, wallTexture, carTexture, currentTextureIndex);
		modelViewMatrix.PopMatrix();

    modelViewMatrix.PushMatrix();
      //simba.Draw(modelViewMatrix, shaderManager, transformPipeline, vLightEyePos, 0, wallTexture[0][1], carTexture);
      track.Draw(modelViewMatrix, shaderManager, transformPipeline, vLightEyePos);
    modelViewMatrix.PopMatrix();
  
    modelViewMatrix.PushMatrix();
      modelViewMatrix.Translate(3.0f, 0.0f, 0.0f);
      carousel.Draw(modelViewMatrix, shaderManager, transformPipeline, vLightEyePos);
    modelViewMatrix.PopMatrix();

	modelViewMatrix.PopMatrix();
}

// Update positions, orientations, etc., of all changing objects.
void TimerFunction(int value)
{
	theWheel.Update();

	glutPostRedisplay();
	glutTimerFunc(50, TimerFunction, value);
}

// Respond to user requests to toggle reflection or to display alternative textures.
void KeyboardPress(unsigned char pressedKey, int mouseXPosition, int mouseYPosition)
{
  switch (pressedKey) {
    case R_LOWER_KEY: case R_UPPER_KEY:
      reflecting = !reflecting;
      break;

    case T_LOWER_KEY: case T_UPPER_KEY:
      currentTextureIndex = (currentTextureIndex + 1) % NBR_TEXTURE_SETS;
      break;

    case ESCAPE_KEY:
      exit(0);
      break;

    default:
      break;
  }
}

// Respond to arrow keys by moving the camera frame of reference
void NonASCIIKeyboardPress(int key, int mouseXPosition, int mouseYPosition)
{	
  switch (key) {
    case UP_KEY:
      cameraFrame.MoveForward(CAMERA_LINEAR_STEP);
      break;

    case DOWN_KEY:
      cameraFrame.MoveForward(-CAMERA_LINEAR_STEP);
      break;

    case LEFT_KEY:
      cameraFrame.RotateWorld(CAMERA_ANGULAR_STEP, 0.0f, 1.0f, 0.0f);
      break;

    case RIGHT_KEY:
      cameraFrame.RotateWorld(-CAMERA_ANGULAR_STEP, 0.0f, 1.0f, 0.0f);
      break;

    case F1_KEY:
      fullscreen = !fullscreen;
      fullscreen ? glutFullScreen() : glutReshapeWindow(ORIG_WINDOW_SIZE[0], ORIG_WINDOW_SIZE[1]);
      break;

    default:
      break;
  }
}
