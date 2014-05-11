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
//Asteroid Class
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
	if(state==1)
	{
	DrawCircle(x,y,rad,1);
	}
    glEnd();
}

//AstArray Class

class AstArray
{
public:
    Asteroid ast;
	int index;
    AstArray *next;

    AstArray();
    void Initialize(int windowWidth,int playerX, int playerY, int currentTime, int startTime);
    void MoveAll(int stage, AstArray head);
    int CheckAllHitPlayer(int playerX, int playerY, AstArray *head);
	int CheckAllOnScreen(int windowWidth, int windowHeight, AstArray *head);
	void DeleteAst(AstArray *head);
    void DeleteAll(AstArray *head);
    void DrawAll(AstArray *head);
	void AddAst(AstArray *head, AstArray *tail, int windowWidth,int playerX, int playerY);
	void SetVelMult(int velMult, AstArray *head);
};
AstArray::AstArray()
{
    next=NULL;
}
void Initialize(int windowWidth,int playerX, int playerY, int currentTime, int startTime)
{

}
void AddAst(AstArray *head, AstArray *tail, int windowWidth,int playerX, int playerY)
{
	Asteroid newAsteroid;
	newAsteroid.Initialize(windowWidth, playerX, playerY);
	AstArray *newItem=new AstArray;
	newItem->ast=newAsteroid;

    if(NULL==tail)
    {
    head=newItem;
    tail=newItem;
    }
    else
    {
    tail->next=newItem;
    tail=newItem;
    }
}
void MoveAll(int stage, AstArray *head)
{
	AstArray *ptr;
    for(ptr=head; NULL!=ptr; ptr=ptr->next)
    {
        ptr->ast.Move(stage);
    }
}
int CheckAllHitPlayer(int playerX, int playerY, AstArray *head)
{
	int hit=0;
	AstArray *ptr;
    for(ptr=head; NULL!=ptr; ptr=ptr->next)
    {
		if(ptr->ast.CheckHitPlayer(playerX,playerY)==1)
		{
			hit=1;
			ptr->ast.state=0;
		}
    }
	return hit;
}
int CheckAllOnScreen(int windowWidth, int windowHeight, AstArray *head)
{
	int onScreen=0;
	AstArray *ptr;
    for(ptr=head; NULL!=ptr; ptr=ptr->next)
    {
		if(ptr->ast.CheckOnScreen(windowWidth, windowHeight)==1)
		{
			onScreen=1;
			ptr->ast.state=0;
		}
    }
	return onScreen;
}
void DeleteAst(AstArray *head)
{
    AstArray *next;
	AstArray *ptr;
    ptr=head;
    while(NULL!=ptr)
    {
		if(ptr->ast.state==0)
		{
        next=ptr->next;
        delete ptr;
        ptr=next;
		}
    }
}
void DrawAll(AstArray *head)
{
	AstArray *ptr;
    for(ptr=head; NULL!=ptr; ptr=ptr->next)
    {
        ptr->ast.Draw();
    }
}
//clear function for powerup for rui
void DeleteAll(AstArray *head)
{
	AstArray *ptr;
    for(ptr=head; NULL!=ptr; ptr=ptr->next)
    {
        ptr->ast.Delete();
    }
}
//more powerup related functions for rui
void SetVelMult(int velMult, AstArray *head)
{
		AstArray *ptr;
    for(ptr=head; NULL!=ptr; ptr=ptr->next)
    {
        ptr->ast.velmult=velMult;
    }
}


//main
int main()
{
    int i,j;
	AstArray *head=NULL,*tail=NULL;


    FsOpenWindow(16,16,800,600,1);

    srand(time(NULL));

    int terminate=0,playerX=400,playerY=580;
	AstArray astAr;



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
			astAr.AddAst(head,tail,800,400,600);
            break;
		case FSKEY_LEFT:
            break;
        }

      

        glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
       
		astAr.CheckAllOnScreen(800,600,head);
		astAr.CheckAllHitPlayer(400,600,head);
		astAr.MoveAll(1,*head);
		astAr.DrawAll(head);
		astAr.DeleteAst(head);

        FsSwapBuffers();

        FsSleep(20);
    }

    return 0;
}
