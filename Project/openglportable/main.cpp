#include <GL/glut.h>
#include <math.h>

//World
float worldH =640;
float worldW =480;

// Bird
float birdX,birdY;
float birdVY =0;
float birdR =14;

// Pipe
float pipeX;
float pipeGapY =300;
float pipeW =60;
float pipeGap =160;
float pipeSpeed =150;

//Draw Rectengular
void drawRect(float x,float y,float w,float h)
{
    glBegin(GL_POLYGON);
    glVertex2f(x,y);
    glVertex2f(x+w,y);
    glVertex2f(x+w,y+h);
    glVertex2f(x,y+h);
    glEnd();
}

//Draw Circle
void drawCircle(float cx,float cy,float r)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx,cy);
    for(int i=0;i<=100;i++)
    {
        float a=i*2*3.1416f/100;
        glVertex2f(cx+cos(a)*r,cy+sin(a)*r);
    }
    glEnd();
}

//Cloud
void drawCloud(float x,float y)
{
    glColor3f(1,1,1);
    drawCircle(x,y,20);
    drawCircle(x+20,y+10,25);
    drawCircle(x-20,y+10,25);
    drawCircle(x+40,y,18);
}

//  Background
void drawHill()
{
    glColor3f(0.2,0.6,0.2);
    drawCircle(120,80,150);
    drawCircle(360,80,180);
}
void drawBackground()
{
    glColor3f(0.53,0.81,0.98);
    drawRect(0,0,worldW,worldH);

    // Clouds
    drawCloud(100,520);
    drawCloud(280,560);
    drawCloud(380,500);

    drawHill();

    glColor3f(0.36,0.25,0.2);
    drawRect(0,0,worldW,80);
}

//  Bird
void drawBird()
{
    // Body
    glColor3f(1,0.9,0.4);
    drawCircle(birdX, birdY, birdR);
    glColor3f(1,0.8,0.2);
    drawCircle(birdX,birdY,birdR-3);

    // Wings
    glColor3f(1,0.7,0.1);
    drawCircle(birdX-birdR+2,birdY+2,6);
    drawCircle(birdX+birdR-2,birdY+2,6);

    // Tail
    glColor3f(1,0.6,0.0);
    glBegin(GL_TRIANGLES);
    glVertex2f(birdX-birdR,birdY);
    glVertex2f(birdX-birdR-8,birdY+4);
    glVertex2f(birdX-birdR-8,birdY-4);
    glEnd();

    // Eye
    glColor3f(1,1,1);
    drawCircle(birdX+5,birdY+4,3);
    glColor3f(0,0,0);
    drawCircle(birdX+5,birdY+4,2);

    // Beak
    glColor3f(1,0.5,0);
    glBegin(GL_TRIANGLES);
    glVertex2f(birdX+birdR,birdY);
    glVertex2f(birdX+birdR+8,birdY+4);
    glVertex2f(birdX+birdR+8,birdY-4);
    glEnd();
}

//  Pipe
void drawPipe()
{
    glColor3f(0,0.7,0);
    drawRect(pipeX,0,pipeW,pipeGapY-pipeGap/2);
    drawRect(pipeX,pipeGapY+pipeGap/2,pipeW,
             worldH-(pipeGapY+pipeGap/2));
}
//  Collision

bool hitPipe()

{

    if(birdX+birdR>pipeX && birdX-birdR<pipeX+pipeW)

    {

        if(birdY-birdR<pipeGapY-pipeGap/2 ||

           birdY+birdR>pipeGapY+pipeGap/2)

            return true;

    }

    if(birdY-birdR<80 || birdY+birdR>worldH)

        return true;

    return false;

}

 
//  Score

int score = 0;
void drawScore()
{
    int s = score;
    glColor3f(0,0,0);
    glRasterPos2f(10,610);
 
    if(s==0) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'0');
 
    char buf[10];
    int i=0;
    while(s>0)
    {
        buf[i++] = (s%10)+'0';
        s/=10;
    }
    for(int j=i-1;j>=0;j--)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,buf[j]);
}

// Physics
float GRAVITY = 900;
float FLAP = 300;

// Game
bool running = false;
bool gameOver = false;
int score = 0;

//  Update
void update(float dt)
{
    if(!running || gameOver) return;

    birdVY -= GRAVITY*dt;
    birdY += birdVY*dt;

    pipeX -= pipeSpeed*dt;

    if(pipeX+pipeW<0)
    {
        pipeX = worldW;
        pipeGapY = (pipeGapY==300)?260:300;
        score++;
    }

    if(hitPipe())
    {
        gameOver=true;
        running=false;
    }
}

int lastTime = 0;


void timer(int)
{
    int now = glutGet(GLUT_ELAPSED_TIME);
    float dt = (now-lastTime)/1000.0f;
    lastTime = now;
    if(dt>0.05) dt=0.05;

    update(dt);
    glutPostRedisplay();
    glutTimerFunc(16,timer,0);
}
 

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawBackground();
    drawPipe();
    drawBird();
    drawScore();
    glutSwapBuffers();

}
void keyboard(unsigned char key,int,int)
{
    if(key==' ')
    {
        running=true;
        birdVY=FLAP;
    }
    if(key=='r' || key=='R')
    {
        birdX=120; birdY=320; birdVY=0;
        pipeX=worldW;
        score=0;
        running=false;
        gameOver=false;
    }
}

int main(int argc,char** argv)
{
    glutInit(&argc,argv);
    glutInitWindowSize(500,700);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);


}
