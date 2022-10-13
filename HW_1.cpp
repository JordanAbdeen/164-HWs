/*********
   CTIS164 - Template Source Program
----------
STUDENT : Abdullah Khaldoun Yaser Abdeen 22001486
SECTION : 4
HOMEWORK: 1
----------
PROBLEMS:
----------
ADDITIONAL FEATURES: Changing the color of the car, moving clouds, and the moving road
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD  10 // Period for the timer.
#define TIMER_ON 1

#define D2R 0.0174532

// for controlling the direction in the autonomous mode
#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4

// for the state machine
#define MENU 0
#define START 1
#define CONTROLS 2

// initialization of the state machine and the autonomous mode direction 
int state = MENU;
int direction = RIGHT;

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height
 
//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

// for the color of the car
int r = 220, g = 61, b = 61;
//function that draws the car with the origingal color being red
void drawCar(int x, int y) {
    glColor3ub(r, g, b);
    glRectf(x, y, x + 115, y + 30);
    glRectf(x + 20, y + 30, x + 95, y + 55);
    glColor3f(1, 1, 1);
    glRectf(x + 65, y + 35, x + 90, y + 50);
    glColor3ub(245, 237, 3);
    circle(x + 105, y + 20, 5);
    glColor3f(0, 0, 0);
    circle(x + 25, y, 10);
    circle(x + 90, y, 10);
    glColor3f(0.2, 0.2, 0.2);
    circle(x + 25, y, 6);
    circle(x + 90, y, 6);
}

// draws the cloud in a standard shape
void drawCloud(int x, int y) {
    glColor3f(1, 1, 1);
    circle(x, y, 50);
    circle(x + 40, y - 20, 35);
    circle(x - 45, y - 10, 25);
}

//
// To display onto window using OpenGL commands
//
// the car information
int carX = 420, carY = 380;
// cloud initial positions;
int cloudX1 = 440, cloudX2 = 540, cloudX3 = 620, cloudX4 = 750, cloudX5 = 870;
int roadX1 = -400, roadX2 = 0;
void display() {
    //
    // clear window to grey
    //
    glClearColor(0.2, 0.2, 0.2, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    if (state == START) {
        glColor3ub(100, 206, 228);
        glRectf(-400, -320, 400, 320);

        glColor3f(0.5, 0.5, 0.5);
        vprint(-250, 200, GLUT_BITMAP_HELVETICA_18, "The ANTIGRAVITY car");

        glColor3f(0.1, 0.1, 0.1);
        glRectf(-400, -400, 400, -250);

        glColor3ub(255, 255, 51);
        glRectf(roadX1, -320, roadX1 + 300, -300);
        glRectf(roadX2, -320, roadX2 + 300, -300);

        drawCloud(cloudX1, 100);
        drawCloud(cloudX2, 270);
        drawCloud(cloudX3, -50);
        drawCloud(cloudX4, 60);
        drawCloud(cloudX5, 180);

        if (carX > 500) {
            carX = -500;
        }

        if (carY >= 350) {
            carY = -300;
        }

        if (carY < -300) {
            carY = 320;
        }

        if (carX < -500) {
            carX = 500;
        }

        drawCar(carX, carY);

        glColor3f(0.2, 0.2, 0.2);
        glRectf(-400, 320, 400, 400);

        glColor3f(1, 1, 1);
        vprint(-360, 360, GLUT_BITMAP_HELVETICA_12, "Abdullah Abdeen 22001486");
        vprint(320, 380, GLUT_BITMAP_HELVETICA_10, "F2 for the menu");
    } // this is the project itself
    else if (state == MENU) {
        vprint(-140, 50, GLUT_BITMAP_TIMES_ROMAN_24, "The ANTIGRAVITY CAR");
        vprint(-100, 25, GLUT_BITMAP_TIMES_ROMAN_24, "Press space to start");
        vprint(-125, 0, GLUT_BITMAP_TIMES_ROMAN_24, "Press F3 to see controls");
        vprint(-360, 360, GLUT_BITMAP_HELVETICA_12, "Abdullah Abdeen 22001486");
    } // this is the menu of the project
    else if (state == CONTROLS) {
        vprint(-250, 100, GLUT_BITMAP_HELVETICA_18, "F1          - Change mode from manual to automatic and vice versa");
        vprint(-250, 80, GLUT_BITMAP_HELVETICA_18,  "Arrows      - Change direction/move in that direction");
        vprint(-250, 60, GLUT_BITMAP_HELVETICA_18,  "Right click - Change the color of the car");
        vprint(-250, 40, GLUT_BITMAP_HELVETICA_18,  "Left click  - Change the position of the car to the position of the cursor");
        vprint(-250, 20, GLUT_BITMAP_HELVETICA_18,  "F2          - Go back to the menu");
        vprint(-360, 360, GLUT_BITMAP_HELVETICA_12, "Abdullah Abdeen 22001486");
    } // this is the controls information of the project

    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to give access to the game only from the menu and not from the controls
    if (key == ' ' && state == MENU)
        state = START;

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//

bool activeTimer = false;
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
    }

    // the if statement that checks if the active timer is on is meant to not make the car accelerate
    if (key == GLUT_KEY_F1)
        activeTimer = !activeTimer;
    else if (key == GLUT_KEY_F2)
        state = MENU;
    else if (key == GLUT_KEY_F3 && state == MENU) // to give access to the controls only from the menu not from the game itself
        state = CONTROLS;
    else if (key == GLUT_KEY_DOWN) {
        direction = DOWN;
        if (activeTimer == false)
            carY -= 5;
    }
    else if (key == GLUT_KEY_UP) {
        direction = UP;
        if (activeTimer == false)
            carY += 5;
    }
    else if (key == GLUT_KEY_RIGHT) {
        direction = RIGHT;
        if (activeTimer == false)
            carX += 5;
    }
    else if (key == GLUT_KEY_LEFT) {
        direction = LEFT;
        if (activeTimer == false)
            carX -= 5;
    }


    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
void onClick(int button, int stat, int x, int y)
{
    // to translate from one coordinate system to another
    int x1 = x - winWidth / 2;
    int y1 = winHeight / 2 - y;


    srand(time(NULL));
    // to change the position of the car
    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) {
        if (y1 <= 320 && y1 >= -320)
            carX = x1, carY = y1;
    }
    else if (button == GLUT_RIGHT_BUTTON && stat == GLUT_DOWN) {
        r = rand() % 255;
        g = rand() % 255;
        b = rand() % 255;
    } // Changes the color of the car whenever the right mouse button is pressed, generates random numbers for the rgb values
    

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.
    
    if (activeTimer == true)
        switch (direction) {
        case UP: carY++; break;
        case RIGHT: carX++; break;
        case DOWN: carY--; break;
        case LEFT: carX--; break;
        } // to automates the movement of the car in any direction that the user chooses

    // the statements below make the road and the clouds move to give the impression that the car is moving

    cloudX1 -= 2;
    cloudX2 -= 3;
    cloudX3 -= 5;
    cloudX4 -= 6;
    cloudX5 -= 7;
    roadX1--;
    roadX2--;

    if (cloudX1 <= -500)
        cloudX1 = 440;
    
    if (cloudX2 <= -500)
        cloudX2 = 440;

    if (cloudX3 <= -500)
        cloudX3 = 440;

    if (cloudX4 <= -500)
        cloudX4 = 440;

    if (cloudX5 <= -500)
        cloudX5 = 440;

    if (roadX1 == -700)
        roadX1 = 400;

    if (roadX2 == -700)
        roadX2 = 400;

    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("Template File");

    glutDisplayFunc(display);
    glutSetWindowTitle("Car Ride");
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}