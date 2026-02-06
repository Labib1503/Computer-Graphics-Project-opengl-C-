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

// Score
int score = 0;
int highScore = 0;

// Physics
float GRAVITY = 900;
float FLAP = 300;

// Game state
bool running = false;
bool gameOver = false;

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

// Background
void drawBackground()
{
    glColor3f(0.53, 0.81, 0.98);
    drawRect(0, 0, worldW, worldH);

    drawCloud(100 + cloudMove, 520);
    drawCloud(280 + cloudMove, 560);
    drawCloud(380 + cloudMove, 500);

    glColor3f(0.2, 0.6, 0.2);
    drawCircle(120, 80, 150);
    drawCircle(360, 80, 180);

    glColor3f(0.36, 0.25, 0.2);
    drawRect(0, 0, worldW, 80);
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
    drawCircle(birdX - birdR + 2, birdY + 2, 6);  // Left wing
    drawCircle(birdX + birdR - 2, birdY + 2, 6);  // Right wing

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
    glColor3f(0, 0.7, 0);
    drawRect(pipeX, 0, pipeW, pipeGapY - pipeGap / 2);
    drawRect(pipeX, pipeGapY + pipeGap / 2,
             pipeW, worldH - (pipeGapY + pipeGap / 2));
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
