/*********
STUDENT : Abdullah Khaldoun Abdeen
SECTION : 04
HOMEWORK: 3
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
#include "vec.h"

#define WINDOW_WIDTH  700
#define WINDOW_HEIGHT 500

#define TIMER_PERIOD     30 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

#define MENU 0
#define START 1
#define PAUSE 2
#define END 3

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height
int state = MENU;

typedef struct {
    vec_t velocity;
    vec_t pos;
    float angle;
} bullet_t;

typedef struct {
    vec_t pos;
    bool visibility;
} object_t;

bullet_t knife = { {0, 0} , {0, 0}, 0 };
object_t cow = { { 0, -100 }, true };
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

//
// To display onto window using OpenGL commands
//

void drawScreen() {

    glColor3ub(26, 255, 255);
    glRectd(-350, 250, 350, -250);

    glColor3f(0.3, 0.3, 0.3);
    glRectd(-350, 250, 350, 150);

    glColor3f(1, 1, 1);
    vprint(-335, 225, GLUT_BITMAP_HELVETICA_12, "ABDULLAH KHALDOUN ABDEEN - 22001486");
    vprint(-85, 175, GLUT_BITMAP_HELVETICA_18, "MAKE THE BEEF");
    vprint(200, 225, GLUT_BITMAP_HELVETICA_12, "Space to pause/resume");
    vprint(200, 175, GLUT_BITMAP_HELVETICA_12, "b to go back to the menu");
}

void drawCow(int cowX, int cowY) {

    int radius = 25;


    glColor3f(0, 0, 0);
    circle(cowX, cowY, radius + 1);

    glColor3f(1, 1, 1);
    circle(cowX, cowY, radius);


    glColor3f(0, 0, 0);
    circle(cowX + 20, cowY - 10, 3);
    circle(cowX + 15, cowY, 6);
    circle(cowX - 10, cowY - 12, 5);
    circle(cowX - 17, cowY - 7, 6);

    glColor3f(0, 0, 0);
    circle(cowX, cowY + 10, radius * 0.7 + 1);
    
    glColor3f(1, 1, 1);
    circle(cowX, cowY + 10, radius * 0.7);
    
    glColor3f(0, 0, 0);
    circle(cowX + 5, cowY + 18, 8);
    circle(cowX + 4, cowY + 12, 5);

    glColor3f(0, 0, 0);
    circle(cowX - 6, cowY + 13, 4);
    circle(cowX + 6, cowY + 13, 4);

    glColor3f(1, 1, 1);
    circle(cowX - 6, cowY + 13, 3);
    circle(cowX + 6, cowY + 13, 3);

    glColor3f(0, 0, 0);
    circle(cowX - 6, cowY + 13, 2);
    circle(cowX + 6, cowY + 13, 2);

    glColor3ub(255, 78, 244);
    circle(cowX, cowY + 2, 7);

    glColor3f(0, 0, 0);
    circle(cowX - 3, cowY + 2, 1);
    circle(cowX + 3, cowY + 2, 1);
    glRectf(cowX + 3, cowY - 24, cowX + 9, cowY - 31);
    glRectf(cowX - 9, cowY - 24, cowX - 3, cowY - 31);

    glColor3f(1, 1, 1);
    glRectf(cowX + 4, cowY - 25, cowX + 8, cowY - 30);
    glRectf(cowX - 8, cowY - 25, cowX - 4, cowY - 30);
}

void drawKnife(float x, float y) {
    vec_t E1 = addV({ x, y }, pol2rec({ 30, knife.angle - 180 }));
    vec_t E2 = addV({ x, y }, pol2rec({ sqrt(1000), knife.angle - 161.67 }));
    vec_t end = addV({ x, y }, pol2rec({ 45, knife.angle - (180 - atan(4 / 45)) }));

    glColor3ub(152, 152, 152);
    glBegin(GL_TRIANGLES);
    glVertex2f(E2.x, E2.y);
    glVertex2f(x, y);
    glVertex2f(E1.x, E1.y);
    glEnd();

    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(E1.x, E1.y);
    glVertex2f(end.x, end.y);
    glEnd();
}

double calculateDist(float x1, float y1, float x2, float y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void display() {
    //
    // clear window to black
    //
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    if (state == MENU) {
        glColor3ub(24, 142, 28);
        glRectf(-350, -250, 350, 250);
        glColor3ub(192, 28, 28);
        vprint(-50, 0, GLUT_BITMAP_9_BY_15, "MAKE THE BEEF");
        vprint(-45, -25, GLUT_BITMAP_8_BY_13, "Press Space");
    }

    else {//(state == START || state == PAUSE){
    drawScreen();

        if (cow.visibility)
            drawCow(cow.pos.x, cow.pos.y);
        else {
            if (knife.pos.x > 340 || knife.pos.x < -340) {
                knife.velocity.x *= -1;
            }

            if (knife.pos.y > 150 || knife.pos.y < -240) {
                knife.velocity.y *= -1;
            }
        }

        drawKnife(knife.pos.x, knife.pos.y);
    }
    if (state == END) {
        vprint(-125, 0, GLUT_BITMAP_HELVETICA_18, "YOU HAVE MADE THE BEEF");
        vprint(-65, -15, GLUT_BITMAP_HELVETICA_12, "'P' to play again");
    }

    

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

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    if (key == ' ') {
        if (state != PAUSE)
            state++;
        else if (state == PAUSE)
            state--;
    }

    if (key == 'p' && state == END) {
        knife.pos = { 0, 0 };
        cow.pos = { 0, -100 };
        state = START;
    }

    if (key == 'b') {
        state = MENU;
    }
    

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
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
//
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.



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

    if (state == START) {
        cow.pos.x = x - winWidth / 2;
        cow.pos.y = winHeight / 2 - y;
    }
    
    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.

    double difx = cow.pos.x - knife.pos.x,
        dify = cow.pos.y - knife.pos.y;

    if (state == START) {

        if (cow.visibility) {
            knife.angle = atan((knife.pos.y - cow.pos.y) / (knife.pos.x - cow.pos.x)) / D2R;

            if (difx < 0 && dify > 0)
                knife.angle = 180 + knife.angle;
            else if (difx <= 0 && dify <= 0)
                knife.angle += 180;
            else if (difx > 0 && dify <= 0)
                knife.angle = 360 + knife.angle;
        }

        if (cow.visibility)
            knife.velocity = mulV(2, unitV(subV(cow.pos, knife.pos)));

        if (cow.pos.x > 350 || cow.pos.x < -350 || cow.pos.y > 110 || cow.pos.y < -225)
            cow.visibility = false;
        else
            cow.visibility = true;

        if (cow.visibility == true) {
            knife.pos = addV(knife.pos, knife.velocity);
        }
        else {
            knife.pos = addV(knife.pos, knife.velocity);
        }
    }
    
    if (calculateDist(knife.pos.x, knife.pos.y, cow.pos.x, cow.pos.y) < 25)
        state = END;

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