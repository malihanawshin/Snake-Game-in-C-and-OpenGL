#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>
#include <vector>

#define PI 3.14159

using namespace std;

float posX = 0,posY = 0,posZ = 0,move_unit = 1.0;
float foodX,foodY,bodyX=-7,bodyY=-20,cnt = 2;
int flag = 0,score = 0;

vector<pair<float,float>>point;

void scoreCount()
{
    cout << "\t\t\t\t 10 points! " << endl;
    score += 10;
}

void gameover()
{
    cout << endl ;
    cout << "\t\t\t\t GAME OVER !! " << endl;
    cout << endl;
    cout << "\t\t\t\t TOTAL SCORE: " << score << endl;
    exit(0);
}

void newBody()
{
    cnt = cnt + 1.0;
    point.push_back(make_pair(bodyX-cnt,bodyY));
}

void newFood()
{
    float f = (rand() % 80);
    int v = rand() % 2;
    foodX = f;
    if(v) foodX = -foodX;
    v = rand() % 2;
    if(v) foodY = -foodY;
    f = (rand() % 60);
    foodY = f;
}

void head(GLfloat x, GLfloat y, GLfloat radius)
{
    int i, lineAmount = 20;
    GLfloat twicePi = 2.0f * PI;

    glBegin(GL_TRIANGLE_FAN);
    for(i = 0; i <= lineAmount; i++)
    {
        glColor3f (1,1, 0.0);
        glVertex2f(
            x + (radius*sin(i*twicePi/lineAmount)),
            y + (radius*cos(i*twicePi/lineAmount))
        );
    }
    glEnd();
}

void food(float x, float y)
{
    glBegin(GL_POLYGON);
    glColor3f(1,0.0,0); // set food color to red

    glVertex2f(x+1, y-1); // lower right co-ordinate
    glVertex2f(x+1, y+1); // upper right co-ordinate
    glVertex2f(x-1, y+1);  // upper left co-ordinate
    glVertex2f(x-1, y-1); // lower left co-ordinate

    glEnd();
}

void body(float x,float y)
{
    glBegin(GL_POLYGON);
    glColor3f(0.2, 0.6, 0.0); // set body color to green

    glVertex2f(x-1,y+1);  // upper left co-ordinate
    glVertex2f(x-1,y-1); // lower left co-ordinate
    glVertex2f(x+1,y-1); // lower right co-ordinate
    glVertex2f(x+1,y+1); // upper right co-ordinate

    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT); // clear display window
    glMatrixMode(GL_MODELVIEW); // glMatrixMode — specify which matrix is the current matrix ,
    // GL_MODELVIEW Applies subsequent matrix operations to the model view matrix stack.
    glLoadIdentity(); // glLoadIdentity replaces the current matrix with the identity matrix

    food(foodX, foodY);

    glPushMatrix();     // pushes the current matrix stack down by one, duplicating the current matrix.
                        // after a glPushMatrix call, the matrix on top of the stack is identical to the one below it.
    glTranslatef(posX,posY,posZ);   // multiply the current matrix by a translation matrix

    for(auto i : point)
    {
        body(i.first,i.second);
    }

    head(-7,-20,1.0); //posX = -8, posY = -20;

    glPopMatrix(); // pops the current matrix stack, replacing the current matrix with the one below it on the stack
    glFlush();  // process all opengl routines as quickly as possible
}

void init(void)
{
    glClearColor(0.0,0.0,0.0,0.0); // set display window color to black
    glColor3f(1.0,1.0,1.0);

    glMatrixMode(GL_PROJECTION); // set projection parameters
    glLoadIdentity();
    gluOrtho2D(-80.0,80.0,-60.0,60.0); // area of world co-ordinates x range : -1 to 1 , y range : -1 to 1
}

void processSpecialKeys(int key, int a, int b)
{
    if(key == GLUT_KEY_RIGHT && flag != 0  && posX <= 86)
    {
        posX = posX + move_unit;
        flag = 0;

        if(key == GLUT_KEY_UP && flag != 2 && posY <= 77)
        {
            posY = posY + move_unit;
            flag = 2;
        }

    }

    else if(flag != 0 && key == GLUT_KEY_LEFT  && posX >= -70)
    {
        posX = posX - move_unit;
        flag = 1;
    }
   /* else if(key == GLUT_KEY_UP /*&& flag != 2 && posY <= 77)
    {
        posY = posY + move_unit;
        flag = 2;
    }
    else if(key == GLUT_KEY_DOWN /*&& flag != 3 &&  posY >=-38)
    {
        posY = posY - move_unit;
        flag = 3;
    }
    */
    glutPostRedisplay();
}

void update(int value)
{
    if((posX - 7.0) == foodX && (posY-20.0) == foodY)
    {
        newFood();
        newBody();
        scoreCount();
    }

    if(posX <= 87 && flag == 0)
    {
        posX = posX + move_unit;
        if(posX == 86) gameover();
    }
    else if(posX >= -67 && flag == 1)
    {
        posX = posX - move_unit;
        if(posX == -67) gameover();
    }
    else if(posY <= 77 && flag == 2)
    {
        posY = posY + move_unit;
        if(posY == -77) gameover();
    }
    else if(posY >= -38 && flag == 3)
    {
        posY = posY - move_unit;
        if(posY == -38) gameover();
    }

    glutPostRedisplay();
    glutTimerFunc(200, update, 0);
}

void processNormalKeys(unsigned char key, int a, int b)
{
    if (key == 27)
        exit(0);
}

int main(int argc, char** argv)
{
    glutInit(&argc,argv);   // initialize GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);    // set display mode
    glutInitWindowPosition(0,0); // top left display window position
    glutInitWindowSize(800,600); // width and height
    glutCreateWindow("*** SNAKE GAME ***");

    cout << endl;
    cout << "\t\t ********** WELCOME TO SNAKE GAME ********** " << endl;
    cout << endl;

    point.push_back(make_pair(bodyX-1,bodyY));
    point.push_back(make_pair(bodyX-2,bodyY));

    glutDisplayFunc(display);   // send graphics to display window
    init();

    srand(time(NULL));            // execute initialization procedure
    newFood();

    glutTimerFunc(200, update, 0);

    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(processSpecialKeys);

    glutMainLoop(); // display everything and wait
}
