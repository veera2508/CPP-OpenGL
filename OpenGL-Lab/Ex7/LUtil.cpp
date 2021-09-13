#include "LUtil.h"


vector<vector<double>> MM(vector<vector<double>> mat1, vector<vector<double>> mat2)
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

double DToR( double degrees )
{ 
    return degrees * M_PI / 180; 
} 

class Point 
{
public:
	int x, y, z;

	Point() 
	{
		x = y = z = 0;
	}

	Point(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {} 

	void render()
	{
		glVertex3f(x, y, z);
	}

	vector<vector<double>> makeHomogenous() 
	{
		vector<vector<double>> ans(4, vector<double>(1, 0));
		ans[0][0] = x;
		ans[1][0] = y;
		ans[2][0] = z;
		ans[3][0] = 1;
		return ans;
	}
};

Point revHomogenous(vector<vector<double>> coord) 
{
    Point q;
    q.x = coord[0][0];
    q.y = coord[1][0];
    q.z = coord[2][0];
    return q;
}

class Square 
{
public:
	vector<Point> vert;
	vector<double> color;

    Square() {}

	Square(Point a, Point b, Point c, Point d, vector<double> color)
	{
		vert.push_back(a);
		vert.push_back(b);
		vert.push_back(c);
		vert.push_back(d);
		this->color = color;
	}

	void render(string mode = "filled")
	{
		glColor3f(color[0], color[1], color[2]);
		if (mode == "filled")
			glBegin(GL_QUADS);
		else
			glBegin(GL_LINE_LOOP);
		
		for (Point p: vert) p.render();
		glEnd();
	}

	Square transform(vector<vector<double>> transmat) 
	{
        vector<Point> newvert;
        Square newsqr;
		for (auto ver: vert)
        {
			newvert.push_back(revHomogenous(MM(transmat, ver.makeHomogenous())));
        }
        newsqr.vert = vector<Point>(newvert.begin(), newvert.end());
        newsqr.color = vector<double>(color.begin(), color.end());
        return newsqr;
	}

};

class Cube
{
public:
	//Front->Back->Top->Bottom->Left->Right
	vector<Square> sides;
	void render()
	{
		for (auto side: sides) side.render("loop");
	}

	Cube transform(vector<vector<double>> transmat)
	{
        Cube c2;
		for (auto side: sides) c2.sides.push_back(side.transform(transmat));
        return c2;
	}
};


Cube c1;

bool initGL()
{
    glEnable(GL_DEPTH_TEST);
    
    //Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(60, SCREEN_WIDTH / SCREEN_HEIGHT, 5.0, 100.0);

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

vector<vector<double>> Rotate_X(int theta = 0)
{
    if (theta == 0) {
        cout<<"Enter anticlockwise rotation angle: ";
        cin>>theta;
    }
    vector<vector<double>> transmat(4, vector<double>(4, 0));
    double cosx = cos(DToR(theta));
    double sinx = sin(DToR(theta));
    cout<<cosx<<sinx<<endl;
    transmat[0][0] = transmat[3][3] = 1;
    transmat[1][1] = transmat[2][2] = cosx;
    transmat[1][2] = -sinx;
    transmat[2][1] = sinx;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
            cout<<transmat[i][j]<<" ";
        cout<<endl;
    }
    return transmat;
}

vector<vector<double>> translation(int dx = 0, int dy = 0, int dz = 0) 
{
    if (dx == 0 && dy == 0 && dz == 0) {
        cout<<"Enter dx, dy, dz:";
        cin>>dx>>dy>>dz;
    }
    vector<vector<double>> transmat(4, vector<double>(4, 0));
    transmat[0][0] = transmat[1][1] = transmat[2][2] = transmat[3][3] = 1;
    transmat[0][3] = dx;
    transmat[1][3] = dy;
    transmat[2][3] = dz;
    return transmat;
}

void update() {
}

void render()
{
    
    //Clear color buffer
    glClearColor( 0.f, 0.f, 0.f, 1.f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    double side = 10;

    vector<double> red = {1.0f, 0.f, 0.f};
    vector<double> blue = {0.0f, 1.f, 0.f};
    vector<double> green = {0.0f, 0.f, 1.f};
    vector<double> yellow = {1.0f, 0.f, 1.f};
    vector<double> purple = {1.0f, 1.f, 0.f};
    vector<double> cyan = {0.0f, 1.f, 1.f};

    //Front->Back->Top->Bottom->Right->Left
    c1.sides.push_back(Square(Point(-side/2, -side/2, side/2), Point(side/2, -side/2, side/2), Point(side/2, side/2, side/2), Point(-side/2, side/2, side/2), red));
    c1.sides.push_back(Square(Point(-side/2, -side/2, -side/2), Point(side/2, -side/2, -side/2), Point(side/2, side/2, -side/2), Point(-side/2, side/2, -side/2), blue));
    c1.sides.push_back(Square(Point(-side/2, side/2, -side/2), Point(side/2, side/2, -side/2), Point(side/2, side/2, side/2), Point(-side/2, side/2, side/2), green));
    c1.sides.push_back(Square(Point(-side/2, -side/2, -side/2), Point(side/2, -side/2, -side/2), Point(side/2, -side/2, side/2), Point(-side/2, -side/2, side/2), yellow));
    c1.sides.push_back(Square(Point(side/2, -side/2, -side/2), Point(side/2, -side/2, side/2), Point(side/2, side/2, side/2), Point(side/2, side/2, -side/2), purple));
    c1.sides.push_back(Square(Point(-side/2, -side/2, -side/2), Point(-side/2, -side/2, side/2), Point(-side/2, side/2, side/2), Point(-side/2, side/2, -side/2), cyan));

    glTranslatef(0, 0, -30);

    Cube c2;
    int ch;
    vector<vector<double>> transmat;
    cout<<"3D Transform:\n";
    cout<<"1.Translation\n2.Rotation about X axis\n3.Rotation about Y axis\n4.Rotation about Z axis\n";
    cout<<"Enter the choice: ";
    cin>>ch;
    switch(ch)
    {
        case 1:
            transmat = translation();
            break;
        case 2:
            transmat = Rotate_X();
            break;

    }
    c2 = c1.transform(transmat);
    c2.render();

    glutSwapBuffers();
}
