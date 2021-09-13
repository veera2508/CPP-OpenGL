#include "LUtil.h"

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
    double wx_org = 0, wy_org = 0, wx_min = 0, wx_max = 300, wy_min = 0, wy_max = 200;
    double vx_org = 350, vy_org = 0, vx_min = 350, vy_min = 0, vx_max = 500, vy_max = 400;
    double sx = (vx_max - vx_min)/(wx_max - wx_min);
    double sy = (vy_max - vy_min)/(wy_max - wy_min);
    
    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );

    //Reset modelview matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //DDA Line
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f( 0.f, 0.f );
    glVertex2f( 300.f, 0.f );
    glVertex2f( 300.f, 200.f );
    glVertex2f( 0.f, 200.f );
    glEnd();

    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f( 350.f, 0.f );
    glVertex2f( 500.f, 0.f );
    glVertex2f( 500.f, 400.f );
    glVertex2f( 350.f, 400.f );
    glEnd();

    vector<Point> Shape(4);
    Shape[0].x = 10; Shape[0].y = 10;
    Shape[1].x = 120; Shape[1].y = 10;
    Shape[2].x = 180; Shape[2].y = 110;
    Shape[3].x = 10; Shape[3].y = 70;

    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    for (Point p: Shape)
        glVertex2f( p.x, p.y );
    glEnd();

    vector<Point> Mod_Shape;
    for (Point p: Shape)
    {
        Point newp;
        newp.x = vx_min + (p.x - wx_min) * sx;
        newp.y = vy_min + (p.y - wy_min) * sy;
        Mod_Shape.push_back(newp);
    }

    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    for (Point p: Mod_Shape)
        glVertex2f( p.x, p.y );
    glEnd();

    //Update screen
    glutSwapBuffers();
}
