#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "fssimplewindow.h"

void DrawCircle(int cx,int cy,int rad,int fill)
{
    const double YS_PI=3.1415927;
    if(0!=fill)
    {
        glBegin(GL_POLYGON);
    }
    else
    {
        glBegin(GL_LINE_LOOP);
    }
    int i;
    for(i=0; i<64; i++)
    {
        double angle=(double)i*YS_PI/32.0;
        double x=(double)cx+cos(angle)*(double)rad;
        double y=(double)cy+sin(angle)*(double)rad;
        glVertex2d(x,y);
    }
    glEnd();
}
class Asteroid
{
public:
    int state;
    int x,y;
    int rad;
    int vx,vy;
	int velmult;

    void Initialize(int windowWidth,int playerX, int playerY);
    void Move(int stage);
    int CheckHitPlayer(int playerX, int playerY);
	int CheckOnScreen(int windowWidth, int windowHeight);
    void Delete(void);
    void Draw(void);
};
void Asteroid::Initialize(int windowWidth,int playerX, int playerY)
{
	y=-rad;
	x=rand()%windowWidth;
	double ratio=5/sqrt(1.0*(playerX-x)*(playerX-x)+(playerY-y)*(playerY-y));
    state=1;
	rad=rand()%40+10;

	vx=(int)((playerX-x)*ratio);
	vy=(int)((playerY-y)*ratio);
	velmult=2;

}
void Asteroid::Move(int stage)
{
    x+=vx*velmult*stage;
	y+=vy*velmult*stage;
}
int Asteroid::CheckHitPlayer(int playerX,int playerY)
{
	int relativeX,relativeY;
    relativeX=playerX-x;
    relativeY=playerY-y;
    if(0<=relativeX && relativeX<rad && 0<=relativeY && relativeY<rad)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int Asteroid::CheckOnScreen(int windowWidth, int windowHeight)
{
	return 1;
}
void Asteroid::Delete(void)
{
    state=0;
}
void Asteroid::Draw(void)
{
    glColor3ub(50,0,0);
	DrawCircle(x,y,rad,1);
    glEnd();
}



int main()
{
    int i,j;

    FsOpenWindow(16,16,800,600,1);

    srand(time(NULL));

    int terminate=0,playerX=400,playerY=580;
	Asteroid ast;



    while(0==terminate)
    {
        FsPollDevice();
        int key=FsInkey();

        switch(key)
        {
        case FSKEY_ESC:
            terminate=1;
            break;
        case FSKEY_SPACE:
			ast.Initialize(800,400,600);
            break;
		case FSKEY_LEFT:
			ast.Delete();
            break;
        }

      

        glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
       
		//draw
		if(ast.CheckOnScreen(800,600)==1 && ast.state==1)
		{
			ast.Move(1);
			ast.Draw();
		}
		if(ast.CheckHitPlayer(400,600)==1)
		{
			printf("hit");
			ast.Delete();
		}

        FsSwapBuffers();

        FsSleep(20);
    }

    return 0;
}
