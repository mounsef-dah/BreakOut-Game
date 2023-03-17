#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include <windows.h>
#include <mmsystem.h>


#define pi 3.14

//-----------------------------Taille de l'écran----------------------------------------------

#define SCREEN_WIDTH  512
#define SCREEN_HEIGHT 512

//-----------------------------tab----------------------------------------------
#define tabX  32      //tab width
#define tabY  32      //tab height
#define tabS 16       //tab cube size


float px,py,pdx,bdx,bdy; // position du palette + balle direction
float bx,by;  			// position du balle
int rayon = 5; 		// rayon balle
int longueur = 128;   // palette
int largeur = 8; 		//palette largeur
char str[15];  		// display score
int score = 0; //score counting


int i; //for loop

int tab[]=           //the tab array
{
	2,2,2,2,2,0,0,0,0,0,0,0,0,2,2,2,2,2,2,0,0,0,0,0,0,0,0,2,2,2,2,2,
	2,2,2,2,2,0,0,0,0,0,0,0,0,2,2,0,0,2,2,0,0,0,0,0,0,0,0,2,2,2,2,2,
	2,2,2,2,2,1,1,1,1,1,1,1,1,2,2,2,2,2,2,1,1,1,1,1,1,1,1,2,2,2,2,2,
	2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,
	0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,
	0,0,2,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,2,0,0,
	0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,
	0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,
	0,0,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,0,0,
	0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,
	0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,
	0,0,2,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,2,0,0,
	0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,
	2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,
	2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,
	2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,
	2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,
	2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,
};


void vBitmapOutput(int x, int y, char *string, void *font)
{
	int len;

	glRasterPos2f(x,y); // la Positionne du premier caractère de la chaîne
	len = (int) strlen(string); // longueur de la chaîne
	for (i = 0; i < len; i++) glutBitmapCharacter(font,string[i]); //caractère
}

int x,y,xo,yo;

int intersectBall( int ObjetMinX, int ObjetMaxX, int ObjetMinY, int ObjetMaxY, int* d ){ //-1, +1, -1, +1

  // le point le plus proche a la balle

  int x1 = fmax( ObjetMinX , fmin(bx, ObjetMaxX));
  int y1 = fmax( ObjetMinY , fmin(by, ObjetMaxY));

  // la distance entre l'objet et la balle

  float distance = sqrt(
    (x1 - bx) * (x1 - bx) +
      (y1 - by) * (y1 - by)
  );

  *d = ((bx  <= ObjetMinX)||(bx  >= ObjetMaxX))?1:0; // direction balle_case 1_composent de x*(-1) / 0_composent de y*(-1)

  return distance < rayon;
}


void drawtab()
{
 for(y=0;y < (sizeof tab / sizeof tab[0]) / tabX;y++)
 {
  for(x=0;x<tabX;x++)
  {

   if(tab[y*tabY+x]==1){
	   glColor3f(0,1,1);
   }
   else if(tab[y*tabY+x]==2){
   	   glColor3f(0.5,0.5,0.5);
      }
   else{
	   glColor3f(0,0,0);
   }

   xo=x*tabS;
   yo=y*tabS;

   glBegin(GL_QUADS);
   glVertex2i( xo+1, yo+1);
   glVertex2i( xo+1, tabS+yo-1);
   glVertex2i( tabS+xo-1, tabS+yo-1);
   glVertex2i( tabS+xo-1, yo+1);
   glEnd();
  }
 }
}

//-----------------------------------------------------------------------------


//------------------------PALETTE------------------------------------------------

void drawPalette2D()
{
 glColor3f(1,0,0);
 glBegin(GL_QUADS);
 glVertex2i(px,py);
 glVertex2i(px+longueur,py);
 glVertex2i(px+longueur,py+largeur);
 glVertex2i(px,py+largeur);
 glEnd();
}

//------------------------BALL------------------------------------------------

int N = 5; // polygone de N sommets
int i = 0;  // for loop

void affiche_balle(){

  glColor3f(1.0f, 1.0f, 0.0f);
  glBegin (GL_POLYGON);

  for(i=0;i<N;i++){
	  glVertex2f (bx + rayon * cos(2*pi*i/N), by + rayon * sin(2*pi*i/N));
  }
  glEnd ();
}

//------------------------------------------------------------------------------

void Buttons(unsigned char key,int x,int y)
{
 if(key=='d'){ px+=pdx*5;}
 if(key=='a'){ px-=pdx*5;}
 if(key=='w'){ by-=pdx*1;}
 if(key=='s'){ by+=pdx*1;}
 if(px >= 384){px =384;}
 else if(px <= 0){px = 0;}
 glutPostRedisplay();

}

//-----------------------------------------------------------------------------

void ball_physics()
{

	int direction;
  // déplacement de la balle
	bx += bdx;
	by += bdy;

  // les murs

  if((bx + rayon ) >= 512)  bdx*=-1;
  if((bx - rayon ) <= 0) bdx*=-1;
  if((by - rayon ) <= 0) bdy*=-1;
  if((by + rayon ) >= 512){
	  exit(0);
	  //bdy*=-1;
  }


  // palette
  if (intersectBall( px , px + longueur, py, py+largeur, &direction)){

	  bdy*=-1;
	  bdx = (bx - (px + longueur/2))/10;
	  bdx = bdx > 0.2 ? 0.2:-0.2;

  		  	  }


  //collision ball_case

  for(i=0; i < sizeof tab / sizeof tab[0]; i++)
    {
	  x = i % tabX;
	  y = i / tabX;

	  if(tab[y*tabY+x] == 1){

		  if (intersectBall( x*tabS ,x*tabS + tabS, y*tabS, y*tabS + tabS, &direction)){

			  PlaySound(TEXT("ball_brick_effect.wav"), NULL, SND_FILENAME | SND_ASYNC);
			  tab[y*tabY+x] = 0;
			  score+=1;

			  if (direction == 1){
			  		  bdx*=-1;
			  	  }
			  	  else{
			  		  bdy*=-1;
			  	  }
		  	  }
	  }

	  else if(tab[y*tabY+x] == 2){

	  		  if (intersectBall( x*tabS ,x*tabS + tabS, y*tabS, y*tabS + tabS, &direction)){
	  			if (direction == 1){
	  					  bdx*=-1;
	  				  }
	  				  else{
	  					  bdy*=-1;
	  				  }
	  		  	  }
	  	  }
     }
  glutPostRedisplay();
  }

//------------------------------------------------------------------------------

void Mouse(int x,int y)
{

	ShowCursor(FALSE);
	px = x-longueur/2;

	if(px >= 512-longueur){
		px = 512-longueur;
	}
	else if(px <= 0){
		px = 0;
	}
}

//-----------------------------------------------------------------------------



void init()
{
 glClearColor(0,0,0,0);
 gluOrtho2D(0,512,512,0);
 px=192; py=470;
 bx = 256;
 by = 256;
 pdx = 5;
 bdx = -0.2;
 bdy = 0.2;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawtab();
	drawPalette2D();
	affiche_balle();
	ball_physics();

	glColor3d(0.2,0.6,0.2); // Texte en vert

	sprintf(str, "Score : %d", score); // integer to string
	vBitmapOutput(8,25,str,GLUT_BITMAP_TIMES_ROMAN_24);
	glutSwapBuffers();
}

void resize(int w,int h)
{
 glutReshapeWindow(512,512);
}

int main(int argc, char* argv[])
{
 glutInit(&argc, argv);
 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
 glutInitWindowSize(512,512);
 glutCreateWindow("Projet-C");
 init();
 glutDisplayFunc(display);
 glutReshapeFunc(resize);
 //glutKeyboardFunc(Buttons);
 glutPassiveMotionFunc(Mouse);
 glutMainLoop();
}













