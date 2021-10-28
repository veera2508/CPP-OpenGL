#include "LUtil.h"
#include "LUtil.h"

float xmin, xmax, ymin, ymax, _x1, _x2, _y1, _y2;

class Point
{
public:
    float x, y;

    Point(float _x, float _y) : x(_x), y(_y) {}

    string getRegionCode()
    {
        //TBRL
        string rc = "0000";
        if (x < xmin) rc[3] = '1';
        if (x > xmax) rc[2] = '1';
        if (y < ymin) rc[1] = '1';
        if (y > ymax) rc[0] = '1';
        return rc;
    }
};

Point p1(0, 0), p2(0, 0);

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

    cout<<"Enter xmin, xmax, ymin, ymax: ";
    cin>>xmin>>xmax>>ymin>>ymax;

    cout<<"Enter _x1, _y1: ";
    cin>>_x1>>_y1;

    cout<<"Enter x2, _y2: ";
    cin>>_x2>>_y2;

    p1 = Point(_x1, _y1);
    p2 = Point(_x2, _y2);

    return true;
}

bool trivialAccept(string rc1, string rc2)
{
    int _rc1 = stoi(rc1);
    int _rc2 = stoi(rc2);
    if((_rc1 | _rc2) == 0)
        return true;
    return false;
}

bool trivialReject(string rc1, string rc2)
{
    int _rc1 = stoi(rc1);
    int _rc2 = stoi(rc2);
    if ((_rc1 & _rc2) != 0)
        return true;
    return false;
}

Point findInter(Point p, string s)
{
    if (stoi(p.getRegionCode()) == 0)
        return p;
    float m = (_y2 - _y1)/(_x2 - _x1);
    Point q(0, 0);
    if (s == "r")
    {
        // y - _y1 = m (x - _x1) -> y = m (xmax - _x1) + _y1
        q.y = m * (xmax - p.x) + p.y;
        q.x = xmax;
    }

    else if (s == "l")
    {
        // y - _y1 = m (x - _x1) -> y = m (xmin - _x1) + _y1
        q.y = m * (xmin - p.x) + p.y;
        q.x = xmin;
    }

    else if (s == "r")
    {
        // y - _y1 = m (x - _x1) -> (ymax - y1)/m + x1
        q.x = (ymax - p.y)/m + p.x;
        q.y = ymax;
    }

    else if (s == "l")
    {
        // y - _y1 = m (x - _x1) -> (ymax - y1)/m + x1
        q.x = (ymin - p.y)/m + p.x;
        q.y = ymin;
    }

    int rcq = stoi(q.getRegionCode());
    if (rcq == 0) return q;
    return p;
}

void update() {

}

bool done = false;

void render()
{
    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );

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

    glColor3f(0.f, 0.f, 0.75f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(xmin, ymin);
    glVertex2f(xmax, ymin);
    glVertex2f(xmax, ymax);
    glVertex2f(xmin, ymax);
    glEnd();

    glColor3f(0.f, 0.75f, 0.f);
    glBegin(GL_LINES);
    glVertex2f(_x1, _y1);
    glVertex2f(_x2, _y2);
    glEnd();

    while (!done)
    {
        string rc1 = p1.getRegionCode();
        string rc2 = p2.getRegionCode();
        if (trivialAccept(rc1, rc2) || trivialReject(rc1, rc2)) 
        {
            done = true;
            break;
        }

        Point p3(p1.x, p1.y);
        if (rc1[0] == '1')
            p3 = findInter(p1, "t");
        if (rc1[1] == '1')
            p3 = findInter(p1, "b");
        if (rc1[2] == '1')
            p3 = findInter(p1, "r");
        if (rc1[3] == '1')
            p3 = findInter(p1, "l");
        p1 = p3;

        Point p4(p2.x, p2.y);
        if (rc2[0] == '1')
            p4 = findInter(p2, "t");
        if (rc2[1] == '1')
            p4 = findInter(p2, "b");
        if (rc2[2] == '1')
            p4 = findInter(p2, "r");
        if (rc2[3] == '1')
            p4 = findInter(p2, "l");
        p2 = p4;


        rc1 = p1.getRegionCode();
        rc2 = p2.getRegionCode();
        if (trivialAccept(rc1, rc2) || trivialReject(rc1, rc2)) 
        {
            done = true;
            break;
        }
    }

    glColor3f(0.75f, 0.f, 0.f);
    glBegin(GL_LINES);
    glVertex2f(p1.x, p1.y);
    glVertex2f(p2.x, p2.y);
    glEnd();



    //Update screen
    glutSwapBuffers();

    cout<<"New Points (x1, y1, x2, y2): ";
    cout<<p1.x<<" "<<p1.y<<"\t"<<p2.x<<" "<<p2.y<<endl;
    int x;
    cin>>x;
}
