#include <iostream>
#include <math.h>
#include <vector>
using namespace std;

#ifndef LUTIL_H
#define LUTIL_H

#include "LOpenGL.h"

class Point {
public:
	int x, y;
};

enum transform_choice {
	TRANSLATION = 1,
	ROTATION_AB_ORGIN,
	ROTATION_AB_POINT,
	UNI_SCALE_ORG,
	DIFF_SCALE_ORG,
	UNI_SCALE_POINT,
	DIFF_SCALE_POINT,
	REF_X,
	REF_Y,
	REF_YX,
	REF_XY,
	SHEAR_X,
	SHEAR_Y,
	SHEAR_X_POINT,
	SHEAR_Y_POINT
};

//Screen Constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 60;

//Color modes
const int COLOR_MODE_CYAN = 0;
const int COLOR_MODE_MULTI = 1;

bool initGL();
/*
Pre Condition:
 -A valid OpenGL context
Post Condition:
 -Initializes matrices and clear color
 -Reports to console if there was an OpenGL error
 -Returns false if there was an error in initialization
Side Effects:
 -Projection matrix is set to identity matrix
 -Modelview matrix is set to identity matrix
 -Matrix mode is set to modelview
 -Clear color is set to black
*/

void update();
/*
Pre Condition:
 -None
Post Condition:
 -Does per frame logic
Side Effects:
 -None
*/

void render();
/*
Pre Condition:
 -A valid OpenGL context
 -Active modelview matrix
Post Condition:
 -Renders the scene
Side Effects:
 -Clears the color buffer
 -Swaps the front/back buffer
*/


#endif
