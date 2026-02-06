#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

// World
float worldH = 640;
float worldW = 480;

// Bird
float birdX, birdY;
float birdVY = 0;
float birdR = 14;

// Pipe
float pipeX;
float pipeGapY = 300;
float pipeW = 60;
float pipeGap = 160;
float pipeSpeed = 150;

// Cloud movement
float cloudMove = 0;
float treeMove = 0;
float hillMove = 0;
// Score
int score = 0;
int highScore = 0;

// Physics
float GRAVITY = 900;
float FLAP = 300;

// Game state
bool running = false;
bool gameOver = false;


float wingOffset = 0;
bool wingUp = true;
float wingSpeed = 50;

// ---------------- DRAWING ----------------

void drawRect(float x, float y, float w, float h)
{
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

void drawCircle(float cx, float cy, float r)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= 100; i++)
    {
        float a = i * 2 * 3.1416f / 100;
        glVertex2f(cx + cos(a) * r, cy + sin(a) * r);
    }
    glEnd();
}

void drawText(float x, float y, const char* text)
{
    glRasterPos2f(x, y);
    for (int i = 0; text[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
}

// Cloud
void drawCloud(float x, float y)
{
    glColor3f(1, 1, 1);
    drawCircle(x, y, 20);
    drawCircle(x + 20, y + 10, 25);
    drawCircle(x - 20, y + 10, 25);
    drawCircle(x + 40, y, 18);
}

void drawTree(float x, float y)
{

    glColor3f(0.55, 0.27, 0.07);
    drawRect(x - 15, y, 30, 100);

    glColor3f(0.0, 0.6, 0.0);
    drawCircle(x, y + 130, 55);
    drawCircle(x - 35, y + 100, 45);
    drawCircle(x + 35, y + 100, 45);
}

// sun drawing

void drawSun(float x, float y, float r)
{
    glColor3f(1.0f, 0.9f, 0.0f);
    drawCircle(x, y, r);
}


// Background
void drawBackground()
{

    // // Sky
    glColor3f(0.53, 0.81, 0.98);
    drawRect(0, 0, worldW, worldH);


    // Sun
    drawSun(worldW - 60, worldH - 60, 50);

    //  cloud
    drawCloud(100 + cloudMove, 520);
    drawCloud(280 + cloudMove, 560);
    drawCloud(380 + cloudMove, 500);





    // Hills
    glColor3f(0.45, 0.35, 0.25);
    drawCircle(120 + hillMove, 80, 150);
    drawCircle(360 + hillMove, 80, 180);
    drawCircle(120 + hillMove + 500, 80, 150);
    drawCircle(360 + hillMove + 500, 80, 180);

        glColor3f(0.2f, 0.6f, 0.2f);
    drawRect(0, 0, worldW, 80);

       // Trees
    drawTree(100 + treeMove, 80);
    drawTree(420 + treeMove, 80);
    drawTree(100 + treeMove + 500, 80);
    drawTree(420 + treeMove + 500, 80);
}

// Bird
void drawBird()
{
    // Body
    glColor3f(1,0.9,0.4);
    drawCircle(birdX, birdY, birdR);
    glColor3f(1,0.8,0.2);
    drawCircle(birdX, birdY, birdR - 3);

    // Wings
    glColor3f(1,0.7,0.1);
    drawCircle(birdX - birdR + 2, birdY + 2, 6);
    drawCircle(birdX + birdR - 2, birdY + 2, 6);


    // Wings with animation
    glColor3f(1,0.7,0.1);
    drawCircle(birdX - birdR + 2, birdY + 2 + wingOffset, 6);
    drawCircle(birdX + birdR - 2, birdY + 2 + wingOffset, 6);

    // Tail
    glColor3f(1,0.6,0.0);
    glBegin(GL_TRIANGLES);
    glVertex2f(birdX - birdR, birdY);
    glVertex2f(birdX - birdR - 8, birdY + 4);
    glVertex2f(birdX - birdR - 8, birdY - 4);
    glEnd();

    // Eye
    glColor3f(1,1,1);
    drawCircle(birdX + 5, birdY + 4, 3);
    glColor3f(0,0,0);
    drawCircle(birdX + 5, birdY + 4, 2);

    // Beak
    glColor3f(1,0.5,0);
    glBegin(GL_TRIANGLES);
    glVertex2f(birdX + birdR, birdY);
    glVertex2f(birdX + birdR + 8, birdY + 4);
    glVertex2f(birdX + birdR + 8, birdY - 4);
    glEnd();
}


// Pipe
void drawPipe()
{
     glColor3f(0.1, 0.65, 0.1);
    drawRect(pipeX, 0, pipeW, pipeGapY - pipeGap / 2);
    drawRect(pipeX, pipeGapY + pipeGap / 2,
             pipeW, worldH - (pipeGapY + pipeGap / 2));

    glColor3f(0.0, 0.4, 0.0);
    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
        glVertex2f(pipeX, 0);
        glVertex2f(pipeX + pipeW, 0);
        glVertex2f(pipeX + pipeW, pipeGapY - pipeGap/2);
        glVertex2f(pipeX, pipeGapY - pipeGap/2);
    glEnd();
}

// Collision
bool hitPipe()
{
    if (birdX + birdR > pipeX && birdX - birdR < pipeX + pipeW)
    {
        if (birdY - birdR < pipeGapY - pipeGap / 2 ||
            birdY + birdR > pipeGapY + pipeGap / 2)
            return true;
    }

    if (birdY - birdR < 80 || birdY + birdR > worldH)
        return true;

    return false;
}

// Score
void drawScore()
{
    glColor3f(0, 0, 0);
    drawText(10, 610, "Score:");
    drawText(70, 610, to_string(score).c_str());

    drawText(10, 580, "High:");
    drawText(70, 580, to_string(highScore).c_str());
}

// ---------------- UPDATE ----------------

void update(float dt)
{
    if (!running || gameOver) return;

    birdVY -= GRAVITY * dt;
    birdY += birdVY * dt;

    pipeX -= pipeSpeed * dt;

    cloudMove -= 20 * dt;
    if (cloudMove < -500) cloudMove = 0;


    treeMove -= 80 * dt;
if (treeMove < -500) treeMove = 0;


hillMove -= 40 * dt;
if (hillMove < -500) hillMove = 0;



    if (pipeX + pipeW < 0)
    {
        pipeX = worldW;
        pipeGapY = 200 + rand() % 200;
        score++;

        if (score % 5 == 0)
            pipeSpeed += 10;
    }

    if (hitPipe())
    {
        gameOver = true;
        running = false;
        if (score > highScore)
            highScore = score;
    }


if (wingUp) {
    wingOffset += wingSpeed * dt;
    if (wingOffset > 6) wingUp = false;
} else {
    wingOffset -= wingSpeed * dt;
    if (wingOffset < -6) wingUp = true;
}
}

// ---------------- GLUT ----------------

int lastTime = 0;

void timer(int)
{
    int now = glutGet(GLUT_ELAPSED_TIME);
    float dt = (now - lastTime) / 1000.0f;
    lastTime = now;

    update(dt);
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawBackground();
    drawPipe();
    drawBird();
    drawScore();

    if (!running && !gameOver)
    {
        glColor3f(0, 0, 0);
        drawText(120, 340, "Press SPACE to Start");
    }

    if (gameOver)
    {
        glColor3f(1, 0, 0);
        drawText(150, 350, "GAME OVER");
        drawText(110, 320, "Press R to Restart");
    }

    glutSwapBuffers();
}

void keyboard(unsigned char key, int, int)
{
    if (key == ' ')
    {
        running = true;
        birdVY = FLAP;
    }
    if (key == 'r' || key == 'R')
    {
        birdX = 120;
        birdY = 320;
        birdVY = 0;
        pipeX = worldW;
        pipeSpeed = 150;
        score = 0;
        running = false;
        gameOver = false;
    }
}

int main(int argc, char** argv)
{
    srand(time(0));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(worldW, worldH);
    glutCreateWindow("Flappy Bird");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, worldW, 0, worldH, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    birdX = 120;
    birdY = 320;
    pipeX = worldW;

    lastTime = glutGet(GLUT_ELAPSED_TIME);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, timer, 0);

    glutMainLoop();
}
