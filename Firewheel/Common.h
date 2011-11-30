//
//  Common.h
//  Firewheel
//
//  Created by Mark Sands on 11/29/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef Firewheel_Common_h
#define Firewheel_Common_h

#include <ctime>

/* Utility Function: Generate a random integer value between the two parameterized values. */
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

/* DEGRESS TO RADIANS */
const float PI = 3.14159265f;
const float PI_OVER_180 = PI/180.0f;

static inline float DEG2RAD(float degrees) {
  return degrees * PI_OVER_180;
}

#endif
