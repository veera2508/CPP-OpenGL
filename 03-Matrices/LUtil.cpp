#include "LUtil.h"

//The current color rendering mode
int gColorMode = COLOR_MODE_CYAN;

//The projection scale
GLfloat gProjectionScale = 1.f;

bool initGL()
{
    //Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho(0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0);

    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //Initialize clear color
    glClearColor( 0.f, 0.f, 0.f, 1.f );

    //Check for error
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
        return false;
    }

    return true;
}

void update()
{

}

void render()
{
    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );

    //Reset modelview matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //Move modelview to center
    glTranslatef(SCREEN_WIDTH/2.f, SCREEN_HEIGHT/2.f, 0.f);

    //Render quad
    if (gColorMode == COLOR_MODE_CYAN)
    {
        glBegin( GL_QUADS );
            glColor3f(0.f, 1.f, 1.f);
            glVertex2f( -50.f, -50.f );
            glVertex2f( 50.f, -50.f );
            glVertex2f( 50.f, 50.f );
            glVertex2f( -50.f, 50.f );
        glEnd();
    }
    else
    {
        //RYGM Mix
        glBegin(GL_QUADS);
            glColor3f(1.f, 0.f, 0.f); glVertex2f( -50.f, -50.f );
            glColor3f(1.f, 1.f, 0.f); glVertex2f( 50.f, -50.f );
            glColor3f(0.f, 1.f, 0.f); glVertex2f( 50.f, 50.f );
            glColor3f(0.f, 0.f, 1.f); glVertex2f( -50.f, 50.f );
        glEnd();
    }

    //Update screen
    glutSwapBuffers();
}

void handleKeys( unsigned char key, int x, int y )
{
    if (key == 'q')
    {
        if (gColorMode == COLOR_MODE_CYAN)
            gColorMode = COLOR_MODE_MULTI;
        else
            gColorMode = COLOR_MODE_CYAN;
    }
    else if (key == 'e')
    {
        if (gProjectionScale == 1.f)
            gProjectionScale = 2.f; //Zoom out
        else if (gProjectionScale == 2.f)
            gProjectionScale = 0.5f; //Zoom in;
        else if (gProjectionScale == 0.5f)
            gProjectionScale = 1.f; //Regular
        
        //Update the projection matrix
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0, SCREEN_WIDTH * gProjectionScale, SCREEN_HEIGHT * gProjectionScale, 0.0, 1.0, -1.0);
    }
}
