#include "LUtil.h"

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

void drawLine(float x_1, float y_1, float x_2, float y_2, bool lr) {
    float x, y, m;
    m = (y_2 - y_1) / (x_2 - x_1);
    float absm = abs(m);
    if (m > 0) {
        if (lr && m <= 1) {
            x = x_1;
            y = y_1;
            glVertex2f(x, y);
            while (x < x_2) {
                x++;
                y+=m;
                glVertex2f(x, y);
            }
        }

        else if (lr && m > 1) {
            x = x_1;
            y = y_1;
            glVertex2f(x, y);
            while (y < y_2) {
                x+= 1/m;
                y+=1;
                glVertex2f(x, y);
            }
        }

        else if (!lr && m <= 1) {
            x = x_2;
            y = y_2;
            glVertex2f(x, y);
            while (x > x_1) {
                x--;
                y-=m;
                glVertex2f(x, y);
            }
        }

        else if (!lr && m > 1) {
            x = x_2;
            y = y_2;
            glVertex2f(x, y);
            while (y > y_1) {
                y--;
                x-= 1/m;
                glVertex2f(x, y);
            }
        }
    }
    else {
        if (lr && absm <= 1) {
            x = x_1;
            y = y_1;
            glVertex2f(x, y);
            while (x < x_2) {
                x++;
                y-=absm;
                glVertex2f(x, y);
            }
        }

        else if (lr && absm > 1) {
            x = x_1;
            y = y_1;
            glVertex2f(x, y);
            while (y > y_2) {
                x+= 1/absm;
                y-=1;
                glVertex2f(x, y);
            }
        }

        else if (!lr && absm <= 1) {
            x = x_2;
            y = y_2;
            glVertex2f(x, y);
            while (x > x_1) {
                x--;
                y+=absm;
                glVertex2f(x, y);
            }
        }

        else if (!lr && absm > 1) {
            x = x_2;
            y = y_2;
            glVertex2f(x, y);
            while (y < y_1) {
                y++;
                x-= 1/absm;
                glVertex2f(x, y);
            }
        }
    }
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

    float xc = 0;
    float yc = 0;
    float r = 50;
    
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

    drawLine(-50, 50, 50, -50, true);
    drawLine(-50, -50, 50, 50, true);


    glEnd();

    //Update screen
    glutSwapBuffers();
}
