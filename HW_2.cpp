/*********
   CTIS164 - Template Source Program
----------
STUDENT : Abdullah Khaldoun Yaser Abdeen
SECTION : 04
HOMEWORK: 2
----------
PROBLEMS:
----------
ADDITIONAL FEATURES:
1. You can see the angles for each car on top of the tank.
2. Two objects are being hit separately at the same time.
3. There are two cars as objects.
4. You can see the tank nozzles that chase the objects.
5. Game over screen.
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 600

#define TIMER_PERIOD  50 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532
#define PI 3.142

typedef struct {
    double angle;
    float x, y;
} player_t;

typedef struct {
    float x, y,
        r,
        g,
        b;
} object_t;

typedef struct {
    float x, y,
        angle;
} bullet_t;

/* Global Variables for Template File */
player_t player1 = { 0 }, player2 = { 0 };
object_t object1, object2;
bullet_t bullet1 = { 0, 0, 0 }, bullet2 = { 0 };
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height
double timeLeft = 60 * 1000;

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

void drawScreen() {
    glColor3f(0.3, 0.3, 0.3);
    glRectf(-500, 300, 500, 250);
    glColor3ub(119, 224, 236);
    glRectf(-500, 250, 500, -300);

    glColor3f(0, 0, 0);
    vprint(-440, 270, GLUT_BITMAP_9_BY_15, "Abdullah Khaldoun Yaser Abdeen - 22001486");
}

void calculateAngle(object_t* object, player_t* player, bullet_t* bullet);

void drawPlayer(player_t* player, object_t* object, bullet_t* bullet) {
    calculateAngle(object, player, bullet);
    glBegin(GL_LINES);
    glVertex2f((*player).x, (*player).y);
    glVertex2f((*player).x + 50 * cos((*player).angle * D2R), (*player).y + 50 * sin((*player).angle * D2R));
    glEnd();

    glColor3ub(23, 98, 38);
    glRectf((*player).x - 35, (*player).y, (*player).x + 35, (*player).y - 20);
    glRectf((*player).x - 20, (*player).y + 15, (*player).x + 20, (*player).y);

    glColor3f(0, 0, 0);
    vprint((*player).x - 12.5, (*player).y, GLUT_BITMAP_8_BY_13, "%.0f", (*player).angle);

}

void drawObject(object_t* object) {
    glColor3ub((*object).r, (*object).g, (*object).b);
    glRectf((*object).x, (*object).y, (*object).x + 57.5, (*object).y + 15);
    glRectf((*object).x + 10, (*object).y + 15, (*object).x + 47.5, (*object).y + 27.5);
    glColor3f(1, 1, 1);
    glRectf((*object).x + 32.5, (*object).y + 17.5, (*object).x + 45, (*object).y + 25);
    glColor3ub(245, 237, 3);
    circle((*object).x + 52.5, (*object).y + 10, 2.5);
    glColor3f(0, 0, 0);
    circle((*object).x + 12.5, (*object).y, 5);
    circle((*object).x + 45, (*object).y, 5);
    glColor3f(0.2, 0.2, 0.2);
    circle((*object).x + 12.5, (*object).y, 3);
    circle((*object).x + 45, (*object).y, 3);
}

void drawBullet(bullet_t* bullet) {
    glColor3f(1, 1, 1);
    circle((*bullet).x, (*bullet).y, 5);
}

void resetObject(object_t* object) {
    (*object).r = rand() % 255;
    (*object).g = rand() % 255;
    (*object).b = rand() % 255;
    (*object).x = rand() % 500 + 280;
    (*object).y = rand() % 300 + 150;
    (*object).x = (*object).x - winWidth / 2;
    (*object).y = winHeight / 2 - (*object).y;
    if ((*object).x > 400)
        (*object).x -= 75;

    if ((*object).y < -300)
        (*object).y += 75;
}

double calculateDistance(int x1, int y1, int x2, int y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void calculateAngle(object_t* object, player_t* player, bullet_t* bullet) {
    double opp = calculateDistance((*object).x, (*object).y, (*object).x, 0),
        hyp = calculateDistance((*player).x, (*player).y, (*object).x, (*object).y);

    (*player).angle = asin(opp / hyp) * 180 / PI;
    if ((*object).x < 0 && (*object).y > 0) {
        if ((*player).angle == 90)
            (*player).angle = 180;
        else
            (*player).angle = 180 - (*player).angle;
    }
    else if ((*object).x < 0 && (*object).y < 0)
        (*player).angle += 180;
    else if ((*object).x > 0 && (*object).y < 0)
        (*player).angle = 360 - (*player).angle;

    (*bullet).angle = (*player).angle;
}

bool checkCollision(bullet_t bullet, object_t object) {
    if (calculateDistance(bullet.x, bullet.y, object.x, object.y) < 15)
        return true;
    else
        return false;
}

//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    if (timeLeft > 0) {
        drawScreen();
        drawObject(&object1);
        drawObject(&object2);
        drawBullet(&bullet1);
        drawBullet(&bullet2);
        drawPlayer(&player1, &object1, &bullet1);
        drawPlayer(&player2, &object2, &bullet2);
        vprint(player1.x - 12.5, player1.y - 15, GLUT_BITMAP_8_BY_13, "%.0f", player1.angle);
        vprint(-25, 225, GLUT_BITMAP_9_BY_15, "%.0f", timeLeft / 1000);
    }
    else {
        glClearColor(0.3, 0.3, 0.3, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        vprint(-50, 0, GLUT_BITMAP_HELVETICA_18, "GAME OVER");
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

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    if (timeLeft > 0)
        glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.

    if (checkCollision(bullet1, object1)) {
        resetObject(&object1);
        bullet1.x = player1.x;
        bullet1.y = player1.y;
    }

    if (checkCollision(bullet2, object2)) {
        resetObject(&object2);
        bullet2.x = player2.x;
        bullet2.y = player2.y;
    }

    bullet1.x += 10 * cos(bullet1.angle * D2R);
    bullet1.y += 10 * sin(bullet1.angle * D2R);
    bullet2.x += 10 * cos(bullet2.angle * D2R);
    bullet2.y += 10 * sin(bullet2.angle * D2R);

    if (timeLeft > 0)
        timeLeft -= TIMER_PERIOD;

    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // resetObject();
    srand(time(NULL));
    resetObject(&object1);
    resetObject(&object2);
}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("TANK GAME");

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