#include <GL/glut.h>
#include<math.h>

void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3){
    glBegin(GL_POLYGON);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);
    glEnd();
}

void drawRectangle(float x1, float y1, float x2, float y2){
    glBegin(GL_POLYGON);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

void drawCircle(float cx, float cy, float radius, int segments){
    glBegin(GL_POLYGON);
    for(int i = 0; i < segments; i++){
            float theta = 2.0f*3.1416f*i/segments;
        float x = radius*cos(theta);
        float y = radius*sin(theta);
        glVertex2f(x + cx, y + cy);
    }
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.180f, 0.647f, 1.000f);
    drawRectangle(-1.0f, -0.9f, 1.0f, -0.5f);

    glColor3f(1.0f, 0.6f, 0.0f);
    drawCircle(-0.1f, 0.6f, 0.15f, 100);

    glColor3f(0.4f, 0.2f, 0.1f);
    drawRectangle(-0.8f, -0.5f, -0.7f, 0.0f);

    glColor3f(0.1f, 0.5f, 0.1f);
    drawTriangle(-0.9f, 0.0f, -0.75, 0.3f, -0.6f, 0.0f);

    glColor3f(0.6f, 0.3f, 0.2f);
    drawRectangle(0.35f, -0.5f, 0.8f, 0.1f);

    glColor3f(0.3f, 0.3f, 0.3f);
    drawTriangle(0.3f, 0.1f, 0.575f, 0.45f, 0.85f, 0.1f);

    glColor3f(0.7f, 0.8f, 0.9f);
    drawRectangle(0.52f, -0.5f, 0.63f, -0.15f);

    glColor3f(0.95f, 0.95f, 0.7f);
    drawRectangle(0.4f, -0.15f, 0.5f, 0.0f);

    drawRectangle(0.67f, -0.15f, 0.77f, 0.0f);

glFlush();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(640, 360);
    glutCreateWindow("OpenGL 64bit");
    glutDisplayFunc(display);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glutMainLoop();
    return 0;
}
