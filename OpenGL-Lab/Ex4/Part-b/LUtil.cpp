#include "LUtil.h"

extern float xc, yc, r;

bool initGL()
{
    //Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D(-SCREEN_WIDTH/2, SCREEN_WIDTH/2, -SCREEN_HEIGHT/2, SCREEN_HEIGHT/2);

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

void update() {

}

void render()
{
    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );

    //Reset modelview matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //DDA Line
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_POINTS);

    float x, y, pk;
    pk = 1-r;
    x = 0;
    y = r;
    glVertex2f(x, y);
    while (x <= y) 
    {
        x++;
        if (pk < 0) {
            pk += 2*x + 1;
        }
        else {
            y--;
            pk += 2*x - 2*y + 1;
        }
        glVertex2f(xc + x, yc + y);
        glVertex2f(xc + y, yc + x);
        glVertex2f(xc + x, yc - y);
        glVertex2f(xc + y, yc - x);
        glVertex2f(xc - x, yc - y);
        glVertex2f(xc - y, yc - x);
        glVertex2f(xc - x, yc + y);
        glVertex2f(xc - y, yc + x);
    }
    glEnd();
    //Update screen
    glutSwapBuffers();
}
