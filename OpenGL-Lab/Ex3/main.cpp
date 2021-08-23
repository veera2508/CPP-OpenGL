#include "LUtil.h"
#include <iostream>
#include <math.h>
#include "LUtil.h"


using namespace std;

float x_1, x_2, y_1, y_2;
bool lr;
int lr_inp;

void runMainLoop( int val );

int main( int argc, char* args[] )
{

	//Initialize FreeGLUT
	glutInit( &argc, args );

	//Create OpenGL 2.1 context
	glutInitContextVersion( 2, 1 );

	//Create Double Buffered Window
	glutInitDisplayMode( GLUT_DOUBLE );
	glutInitWindowSize( SCREEN_WIDTH, SCREEN_HEIGHT );
	glutCreateWindow( "OpenGL" );

	//Do post window/context creation initialization
	if( !initGL() )
	{
		printf( "Unable to initialize graphics library!\n" );
		return 1;
	}

	cout<<"Enter x_1, y_1, x2, y_2"<<endl;
	cin>>x_1;
	cin>>y_1;
	cin>>x_2;
	cin>>y_2;
	cout<<"Left to Right (1) or Right to Left (2)?"<<endl;
	cin>>lr_inp;
	lr = (lr_inp == 1) ? true : false;

	//Set rendering function
	glutDisplayFunc( render );

	//Set main loop
	glutTimerFunc( 1000 / SCREEN_FPS, runMainLoop, 0 );

	//Start GLUT main loop
	glutMainLoop();

	return 0;
}

void runMainLoop( int val )
{
    //Frame logic
    update();
    render();

    //Run frame one more time
    glutTimerFunc( 1000 / SCREEN_FPS, runMainLoop, val );
}
