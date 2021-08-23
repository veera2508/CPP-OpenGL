#include "LUtil.h"

extern transform_choice globt;
extern vector<Point> shape;

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

vector<vector<int>> matMul3x3(vector<vector<int>> mat1, vector<vector<int>> mat2)
{
    int m1 = mat1.size(), n1 = mat1[0].size(), m2 = mat2.size(), n2 = mat2[0].size();

    vector<vector<int>> ans(m1, vector<int>(n2, 0));

    if (n1 != m2)
    {
        cout<<"Matrix dimensions don't match!\n";
        return ans;
    }

    for (int i = 0; i < m1; i++) 
    {
        for (int j = 0; j < n2; j++)
        {
            for (int k = 0; k < m2; k++) 
            {
                ans[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }

    return ans;
}

vector<vector<int>> makeHomogenous(Point p) {
    vector<vector<int>> ans(3, vector<int>(1, 0));
    ans[0][0] = p.x;
    ans[1][0] = p.y;
    ans[2][0] = 1;
    return ans;
}

Point revHomogenous(vector<vector<int>> coord) {
    Point p;
    p.x = coord[0][0];
    p.y = coord[1][0];
    return p;
}

vector<vector<int>> translation() 
{
    int dx, dy;
    cout<<"Enter dx, dy:";
    cin>>dx>>dy;
    vector<vector<int>> transmat(3, vector<int>(3, 0));
    transmat[0][0] = transmat[1][1] = transmat[2][2] = 1;
    transmat[0][2] = dx;
    transmat[1][2] = dy;
    return transmat;
}

void transform(vector<vector<int>> transmat) 
{
    vector<Point> newshape;

    glBegin( GL_QUADS );
    glColor3f(0.f, 1.f, 0.f);
    for (int i = 0; i < shape.size(); i++) 
    {
        Point oldp = shape[i];
        glVertex2f(oldp.x, oldp.y);
        newshape.push_back(revHomogenous(matMul3x3(transmat, makeHomogenous(oldp))));  
    }
    glEnd();

    glBegin( GL_QUADS );
    glColor3f(1.f, 0.f, 0.f);
    for (int i = 0; i < newshape.size(); i++)
        glVertex2f(newshape[i].x, newshape[i].y);
    glEnd();
}

vector<vector<int>> Rotate_ORG()
{
    int theta;
    cout<<"Enter anticlockwise rotation angle: ";
    cin>>theta;
    vector<vector<int>> transmat(3, vector<int>(3, 0));



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

    glColor3f(1.f, 1.f, 1.f);
    glBegin(GL_LINE);
    glVertex2f(0, -SCREEN_HEIGHT/2);
    glVertex2f(0, SCREEN_HEIGHT/2);
    glEnd();

    glBegin(GL_LINE);
    glVertex2f(-SCREEN_WIDTH/2, 0);
    glVertex2f(SCREEN_WIDTH/2, 0);
    glEnd();

    switch(globt) {
        case TRANSLATION:
        transform(translation());
        break;
    }

    //Update screen
    glutSwapBuffers();
}
