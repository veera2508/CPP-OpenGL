#include "LUtil.h"

extern float x_1, x_2, y_1, y_2;
extern bool lr;

bool initGL()
{
    //Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D(0.0, SCREEN_WIDTH, 0.0, SCREEN_HEIGHT);

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

    //Move modelview to center
    //glTranslatef(SCREEN_WIDTH/2.f, SCREEN_HEIGHT/2.f, 0.f);

    //DDA Line
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_POINTS);

    float x, y, m, dy, dx, pk;
    dy = y_2 - y_1;
    dx = x_2 - x_1;
    pk = 2 * dy - dx;
    int k = 0;
    m = dy / dx;
    float absm = abs(m);
    x = x_1;
    y = y_1;
    glVertex2f(x, y);
    while (x < x_2) 
    {
        x++;
        if (pk < 0) {
            pk += 2*dy;
        }
        else {
            pk += 2*dy - 2*dx;
            y++;
        }
        glVertex2f(x, y);
    }
    glEnd();
    //Update screen
    glutSwapBuffers();
}
