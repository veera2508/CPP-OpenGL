#include "LUtil.h"

extern transform_choice globt;
extern vector<Point> shape;

vector<vector<double>> comp_transmat;
bool first = true;

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

vector<vector<double>> matMul3x3(vector<vector<double>> mat1, vector<vector<double>> mat2)
{
    int m1 = mat1.size(), n1 = mat1[0].size(), m2 = mat2.size(), n2 = mat2[0].size();

    vector<vector<double>> ans(m1, vector<double>(n2, 0));

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

vector<vector<double>> makeHomogenous(Point p) {
    vector<vector<double>> ans(3, vector<double>(1, 0));
    ans[0][0] = p.x;
    ans[1][0] = p.y;
    ans[2][0] = 1;
    return ans;
}

Point revHomogenous(vector<vector<double>> coord) {
    Point p;
    p.x = coord[0][0];
    p.y = coord[1][0];
    return p;
}

vector<vector<double>> translation(int dx = 0, int dy = 0) 
{
    if (dx == 0 && dy == 0) {
        cout<<"Enter dx, dy:";
        cin>>dx>>dy;
    }
    vector<vector<double>> transmat(3, vector<double>(3, 0));
    transmat[0][0] = transmat[1][1] = transmat[2][2] = 1;
    transmat[0][2] = dx;
    transmat[1][2] = dy;
    return transmat;
}

void transform(vector<vector<double>> transmat) 
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

double DegreesToRadians( double degrees )
{ 
    return degrees * M_PI / 180; 
} 

vector<vector<double>> Rotate_ORG(int theta = 0)
{
    if (theta == 0) {
        cout<<"Enter anticlockwise rotation angle: ";
        cin>>theta;
    }
    vector<vector<double>> transmat(3, vector<double>(3, 0));
    double cosx = cos(DegreesToRadians(theta));
    double sinx = sin(DegreesToRadians(theta));
    transmat[0][0] = transmat[1][1] = cosx;
    transmat[0][1] = -sinx;
    transmat[1][0] = sinx;
    transmat[2][2] = 1;
    return transmat;
}

vector<vector<double>> Rotate_Point()
{
    int xc, yc;
    cout<<"Enter the reference points x, y: ";
    cin>>xc>>yc;
    int theta;
    cout<<"Enter anticlockwise rotation angle: ";
    cin>>theta;
    vector<vector<double>> transmat(3, vector<double>(3, 0));
    transmat = matMul3x3(translation(-xc, -yc), Rotate_ORG(theta));
    transmat = matMul3x3(transmat, translation(xc, yc));
    return transmat;
}

vector<vector<double>> Uniform_Scale(double sf = 0) 
{
    if (sf == 0)
    {
        cout<<"Enter scale factor: ";
        cin>>sf;
    }
    vector<vector<double>> transmat(3, vector<double>(3, 0));
    transmat[0][0] = transmat[1][1] = sf;
    transmat[2][2] = 1;
    return transmat;
}

vector<vector<double>> Differential_Scale(double sfx = 0, double sfy =0)
{
    if (sfx == 0 && sfy == 0) 
    {
        cout<<"Enter scale factors sfx, sfy: ";
        cin>>sfx>>sfy;
    }
    vector<vector<double>> transmat(3, vector<double>(3, 0));
    transmat[0][0] = sfx;
    transmat[1][1] = sfy;
    transmat[2][2] = 1;
    return transmat;
}

vector<vector<double>> Uniform_Scale_Point() 
{
    int xc, yc;
    cout<<"Enter the reference points x, y: ";
    cin>>xc>>yc;
    double sf;
    cout<<"Enter scale factor: ";
    cin>>sf;
    vector<vector<double>> transmat(3, vector<double>(3, 0));
    transmat = matMul3x3(translation(-xc, -yc), Uniform_Scale(sf));
    transmat = matMul3x3(transmat, translation(xc, yc));
    return transmat;
}

vector<vector<double>> Differential_Scale_Point() 
{
    int xc, yc;
    cout<<"Enter the reference points x, y: ";
    cin>>xc>>yc;
    double sfx, sfy;
    cout<<"Enter scale factors sfx, sfy: ";
    cin>>sfx>>sfy;
    vector<vector<double>> transmat(3, vector<double>(3, 0));
    transmat = matMul3x3(translation(-xc, -yc), Differential_Scale(sfx, sfy));
    transmat = matMul3x3(transmat, translation(xc, yc));
    return transmat;
}

vector<vector<double>> Reflection(bool alongX)
{
    vector<vector<double>> transmat(3, vector<double>(3, 0));
    if (alongX) 
    {
        transmat[0][0] = 1;
        transmat[1][1] = -1;
        transmat[2][2] = 1;
    }
    else
    {
        transmat[0][0] = -1;
        transmat[1][1] = 1;
        transmat[2][2] = 1;
    }
    return transmat;
}

vector<vector<double>> Reflection_YX()
{
    vector<vector<double>> transmat(3, vector<double>(3, 0));
    transmat[0][1] = -1;
    transmat[1][0] = -1;
    transmat[2][2] = 1;
    return transmat;
}

vector<vector<double>> Reflection_XY()
{
    vector<vector<double>> transmat(3, vector<double>(3, 0));
    transmat[0][1] = 1;
    transmat[1][0] = 1;
    transmat[2][2] = 1;
    return transmat;
}

vector<vector<double>> Shear_X(double sf = 0) 
{
    if (sf == 0)
    {
        cout<<"Enter sheer factor: ";
        cin>>sf;
    }
    vector<vector<double>> transmat(3, vector<double>(3, 0));
    transmat[0][0] = transmat[1][1] = transmat[2][2] = 1;
    transmat[0][1] = sf;
    return transmat;
}

vector<vector<double>> Shear_Y(double sf = 0) 
{
    if (sf == 0)
    {
        cout<<"Enter sheer factor: ";
        cin>>sf;
    }
    vector<vector<double>> transmat(3, vector<double>(3, 0));
    transmat[0][0] = transmat[1][1] = transmat[2][2] = 1;
    transmat[1][0] = sf;
    return transmat;
}

vector<vector<double>> Shear_X_Point(double sf = 0) 
{
    if (sf == 0)
    {
        cout<<"Enter sheer factor: ";
        cin>>sf;
    }
    cout<<"Enter yref: ";
    int yref;
    cin>>yref;
    vector<vector<double>> transmat(3, vector<double>(3, 0));
    transmat[0][0] = transmat[1][1] = transmat[2][2] = 1;
    transmat[0][1] = sf;
    transmat[0][2] = -sf*yref;
    return transmat;
}

vector<vector<double>> Shear_Y_Point(double sf = 0) 
{
    if (sf == 0)
    {
        cout<<"Enter sheer factor: ";
        cin>>sf;
    }
    cout<<"Enter xref: ";
    int xref;
    cin>>xref;
    vector<vector<double>> transmat(3, vector<double>(3, 0));
    transmat[0][0] = transmat[1][1] = transmat[2][2] = 1;
    transmat[1][0] = sf;
    transmat[1][2] = -sf*xref;
    return transmat;
}


void update() 
{

}

void render()
{
    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );

    vector<vector<double>> transmat;

    //Reset modelview matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glColor3f(1.f, 1.f, 1.f);
    glBegin(GL_LINES);
    glVertex2f(0, -SCREEN_HEIGHT/2);
    glVertex2f(0, SCREEN_HEIGHT/2);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-SCREEN_WIDTH/2, 0);
    glVertex2f(SCREEN_WIDTH/2, 0);
    glEnd();

    cout<<"\nTransform:\n";
	cout<<"1.Translation\n2.Rotation about origin\n3.Rotation about (x, y)\n";
	cout<<"4.Uniform Scaling wrt Origin\n5.Differential Scaling wrt origin\n6.Uniform Scaling wrt (x, y)\n7.Differential Scaling wrt (x,y)\n";
	cout<<"8.Reflection about X Axis\n9.Reflection about Y Axis\n10.Reflection about (x=-y)\n11.Reflection about (x=y)\n";
	cout<<"12.X-Direction Shear\n13.Y-Direction Shear\n";
    cout<<"14.X-Direction Shear wrt (y)\n15.Y-Direction Shear wrt (x)\n";
	cout<<"Enter the choice: ";
	int choice;
	cin>>choice;
	globt = (transform_choice)choice;

    switch(globt) {
        case TRANSLATION:
        transmat = translation();
        if (first) comp_transmat = transmat;
        else comp_transmat = matMul3x3(comp_transmat, transmat);
        transform(comp_transmat);
        break;

        case ROTATION_AB_ORGIN:
        transmat = Rotate_ORG();
        if (first) comp_transmat = transmat;
        else comp_transmat = matMul3x3(comp_transmat, transmat);
        transform(comp_transmat);
        break;

        case ROTATION_AB_POINT:
        transmat = Rotate_Point();
        if (first) comp_transmat = transmat;
        else comp_transmat = matMul3x3(comp_transmat, transmat);
        transform(comp_transmat);
        break;

        case UNI_SCALE_ORG:
        transmat = Uniform_Scale();
        if (first) comp_transmat = transmat;
        else comp_transmat = matMul3x3(comp_transmat, transmat);
        transform(comp_transmat);
        break;

        case DIFF_SCALE_ORG:
        transmat = Differential_Scale();
        if (first) comp_transmat = transmat;
        else comp_transmat = matMul3x3(comp_transmat, transmat);
        transform(comp_transmat);
        break;

        case UNI_SCALE_POINT:
        transmat = Uniform_Scale_Point();
        if (first) comp_transmat = transmat;
        else comp_transmat = matMul3x3(comp_transmat, transmat);
        transform(comp_transmat);
        break;

        case DIFF_SCALE_POINT:
        transmat = Differential_Scale_Point();
        if (first) comp_transmat = transmat;
        else comp_transmat = matMul3x3(comp_transmat, transmat);
        transform(comp_transmat);
        break;

        case REF_X:
        transmat = Reflection(true);
        if (first) comp_transmat = transmat;
        else comp_transmat = matMul3x3(comp_transmat, transmat);
        transform(comp_transmat);
        break;

        case REF_Y:
        transmat = Reflection(false);
        if (first) comp_transmat = transmat;
        else comp_transmat = matMul3x3(comp_transmat, transmat);
        transform(comp_transmat);
        break;

        case REF_XY:
        transmat = Reflection_XY();
        if (first) comp_transmat = transmat;
        else comp_transmat = matMul3x3(comp_transmat, transmat);
        transform(comp_transmat);
        break;

        case REF_YX:
        transmat = Reflection_YX();
        if (first) comp_transmat = transmat;
        else comp_transmat = matMul3x3(comp_transmat, transmat);
        transform(comp_transmat);
        break;

        case SHEAR_X:
        transmat = Shear_X();
        if (first) comp_transmat = transmat;
        else comp_transmat = matMul3x3(comp_transmat, transmat);
        transform(comp_transmat);
        break;

        case SHEAR_Y:
        transmat = Shear_Y();
        if (first) comp_transmat = transmat;
        else comp_transmat = matMul3x3(comp_transmat, transmat);
        transform(comp_transmat);
        break;

        case SHEAR_X_POINT:
        transmat = Shear_X_Point();
        if (first) comp_transmat = transmat;
        else comp_transmat = matMul3x3(comp_transmat, transmat);
        transform(comp_transmat);
        break;

        case SHEAR_Y_POINT:
        transmat = Shear_Y_Point();
        if (first) comp_transmat = transmat;
        else comp_transmat = matMul3x3(comp_transmat, transmat);
        transform(comp_transmat);
        break;
    }

    //Update screen
    glutSwapBuffers();
}
