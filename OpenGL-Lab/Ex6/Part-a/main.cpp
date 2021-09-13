#include "LUtil.h"
#include "LUtil.h"

transform_choice globt;
vector<Point> shape;

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

	cout<<"Enter coordinates of quad:\n";
	for (int i = 0; i < 4; i++) 
	{
		cout<<"Enter x"<<i+1<<" y"<<i+1<<" :";
		Point p;
		cin>>p.x>>p.y;
		shape.push_back(p);
	}


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
