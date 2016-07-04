#include<windows.h>
#include<stdlib.h>
#include<C:\GLUT\include\GL\glut.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "image.h"
#include "image.c"

#define PI 3.1415
#define PLAN_TEXTURE_COORD 1.0
#define PLAN_TEXTURE_HELICOPTERO 0.0
#define PLAN_COLOR 0.3,0.3,0.5,1.0
#define HELICOPTER_COLOR 0.3,0.4,0.2,1.0
#define PLAN_TEXTURE "mountains.rgb"
#define HELICOPTER_TEXTURE "textura.rgb"

GLint Largura = 1000;
GLint Altura = 600;
GLfloat Observador[3]={0.0,7.0,0.0};
GLfloat look[3]={0.0,3.0,0.0};
GLuint  texturaDoPlano;
GLuint  texturaDoHelicoptero;
GLshort textures=1;
GLfloat eixoxz=0;
GLfloat raioxz=6;
GLuint  helicoptero;

int AnguloDaHelice = 0;
int ligado = 0;
int tiro = 0;
int torpedoEsquerdo = 0;
int torpedoDireito = 0;
int torpleft =0, torprigth=0;
float movVertical = -0.55;
float movHorizontal = 0;
float movBala = 6.0;
float movTorpEsq[3] = {0,0,0};
float movTorpDir[3] = {0,0,0};


GLfloat ctp[4][2]={
  {-PLAN_TEXTURE_COORD,-PLAN_TEXTURE_COORD},
  {+PLAN_TEXTURE_COORD,-PLAN_TEXTURE_COORD},
  {+PLAN_TEXTURE_COORD,+PLAN_TEXTURE_COORD},
  {-PLAN_TEXTURE_COORD,+PLAN_TEXTURE_COORD}
};

GLfloat cta[4][2]={
  {-PLAN_TEXTURE_HELICOPTERO,-PLAN_TEXTURE_HELICOPTERO},
  {+PLAN_TEXTURE_HELICOPTERO,-PLAN_TEXTURE_HELICOPTERO},
  {+PLAN_TEXTURE_HELICOPTERO,+PLAN_TEXTURE_HELICOPTERO},
  {-PLAN_TEXTURE_HELICOPTERO,+PLAN_TEXTURE_HELICOPTERO}
};


void reshape(int width, int height){
  Largura = width;
  Altura = height;
  glViewport(0,0,(GLint)width,(GLint)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(70.0,width/(float)height,0.1,30.0);
  glMatrixMode(GL_MODELVIEW);
}

void composes_helicopter(void){
	GLUquadricObj *quadric;

	GLfloat helice[][4]={
		{0.0,0.0,0.0},
		{5.5,0.0,0.0},
		{5.5,0.0,0.3},
		{0.0,0.0,0.3}
	};
	GLfloat asaEsquerda[][3]={
	    {0.0,0.3,0.0},
	    {3.0,0.3,-1.0},
	    {3.0,0.3,0.0},
	    {0.0,0.3,2.0}
	};
	GLfloat asaDireita[][3]={
	    {0.0,0.3,0.0},
	    {-3.0,0.3,-1.0},
	    {-3.0,0.3,0.0},
	    {0.0,0.3,2.0}
	};
	GLfloat cauda[][3]={
		{0.0,0.0,0.0},
		{0.0,1.0,-1.0},
		{0.0,1.0,0.0},
		{0.0,0.0,2.0}
	};
	/* Componentes do Helicóptero */
	helicoptero = glGenLists(1);
	glNewList(helicoptero, GL_COMPILE);
	glPushMatrix();
	glTranslatef(0,movVertical,movHorizontal);

    /* Ponta do Helicóptero */
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glTranslatef(0,0,5.0);
	glScalef(0.8, 1.0, 1.0);
	gluSphere(quadric, 0.7, 24, 24);
	glPopMatrix();

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glTranslatef(0,0,4.2);
	glScalef(0.8, 1.0, 1.0);
	gluCylinder(quadric, 1, 0.7, 1.0, 12, 3);
	glPopMatrix();

	/* Cabine do Helicóptero */
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glColor3f(0.8,0.9,1.0);
	glDisable(GL_TEXTURE_2D);
	glTranslatef(0,0.5,3.5);
	glScalef(0.8, 1.0, 1.0);
	gluSphere(quadric, 1.1, 24, 24);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);

	/* Corpo do Helicóptero */
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glTranslatef(0,0.3,1);
	glScalef(0.7, 1.0, 1.0);
	gluCylinder(quadric, 1.3, 1.3, 2.5, 12, 3);
	glPopMatrix();

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glTranslatef(0,0,1.2);
	glScalef(0.8, 1.0, 1.0);
	gluCylinder(quadric, 1, 1, 3, 12, 3);
	glPopMatrix();

	/* Cauda do Helicóptero */
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glTranslatef(0,-0.5,0.6);
	glScalef(0.8, 1.0, 1.0);
	glRotatef(0, 1.0, 0, 0);
	glTranslatef(0,0.8,0.5);
	gluCylinder(quadric, 0, 1.3, 0, 12, 3);
	glPopMatrix();

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glTranslatef(0,-0.4,0.1);
	glScalef(0.8, 1.0, 1.0);
	glRotatef(30, 1.0, 0, 0);
	glTranslatef(0,1.5,0);
	gluCylinder(quadric, 0.8, 1.2, 1.0, 12, 3);
	glPopMatrix();

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glTranslatef(0,1,0.8);
	glScalef(0.8, 1.0, 1.0);
	gluSphere(quadric, 0.6, 24, 24);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0,1,-0.9);

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glRotatef(10, 1.0, 0, 0);
	glTranslatef(0,0.1,-2);
	glScalef(0.8, 1.0, 1.0);

	gluCylinder(quadric, 0.2, 0.8, 4, 12, 3);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0.0,0.3,-2);
	glBegin(GL_POLYGON);

	glTexCoord2fv(cta[0]); glVertex3fv(cauda[0]);
	glTexCoord2fv(cta[1]); glVertex3fv(cauda[1]);
	glTexCoord2fv(cta[2]); glVertex3fv(cauda[2]);
	glTexCoord2fv(cta[3]); glVertex3fv(cauda[3]);
	glEnd();
  	glPopMatrix();
	glPopMatrix();


    /* Metralhadora do Helicoptero do lado esquerdo*/
    quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
//  glColor3f(0.0,0.0,0.0);
	glTranslatef(0.3,-0.5,4.6);
	gluCylinder(quadric, 0.2, 0.1, 1.6, 12, 3);
	glPopMatrix();


	/* Metralhadora do Helicoptero do lado direito*/
    quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
//	glColor3f(0.0,0.0,0.0);
	glTranslatef(-0.3,-0.5,4.6);
	gluCylinder(quadric, 0.2, 0.1, 1.6, 12, 3);
	glPopMatrix();

	/* Bala do lado esquerdo*/
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_FALSE);
	glPushMatrix();
	glDisable(GL_TEXTURE_2D);
    glColor3f(1.0,1.0,0.0);
	glTranslatef(0.3,-0.5,movBala);
	gluCylinder(quadric, 0.1, 0, 0.2, 20, 1);
	glPopMatrix();

	/* Bala do lado direito*/
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_FALSE);
	glPushMatrix();
	//glDisable(GL_TEXTURE_2D);
    glColor3f(1.0,1.0,0.0);
	glTranslatef(-0.3,-0.5,movBala);
	gluCylinder(quadric, 0.1, 0, 0.2, 20, 1);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);


	/* asa esquerda*/
	glPushMatrix();
	glTranslatef(0.0,0.3,1.8);
	glBegin(GL_POLYGON);

	glTexCoord2fv(cta[0]); glVertex3fv(asaEsquerda[0]);
	glTexCoord2fv(cta[1]); glVertex3fv(asaEsquerda[1]);
	glTexCoord2fv(cta[2]); glVertex3fv(asaEsquerda[2]);
	glTexCoord2fv(cta[3]); glVertex3fv(asaEsquerda[3]);
	glEnd();
  	glPopMatrix();


	  /* Torpedo Esquerdo 1 */
	glPushMatrix();
	glTranslatef(0, 0, movTorpEsq[0]);

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	//glColor3f(0.0,0.0,0.0);
	glTranslatef(-1.5,0.4,1.8);
	gluCylinder(quadric, 0.1, 0.1, 2.0, 12, 3);
	glPopMatrix();

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_FALSE);
	glPushMatrix();
	glTranslatef(-1.5,0.4,3.8);
	gluCylinder(quadric, 0.1, 0, 0.4, 12, 3);
	glPopMatrix();
	glPopMatrix();

	/* Torpedo Esquerdo 2*/
	glPushMatrix();
	glTranslatef(0, 0, movTorpEsq[1]);

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	//glColor3f(0.0,0.0,0.0);
	glTranslatef(2.0,0.4,1.8);
	gluCylinder(quadric, 0.1, 0.1, 2.0, 12, 3);
	glPopMatrix();

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_FALSE);
	glPushMatrix();
	glTranslatef(2.0,0.4,3.8);
	gluCylinder(quadric, 0.1, 0, 0.4, 12, 3);
	glPopMatrix();
	glPopMatrix();


	/* Torpedo Esquerdo 3*/
	glPushMatrix();
	glTranslatef(0, 0, movTorpEsq[2]);

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	//glColor3f(0.0,0.0,0.0);
	glTranslatef(2.5,0.4,1.8);
	gluCylinder(quadric, 0.1, 0.1, 2.0, 12, 3);
	glPopMatrix();

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_FALSE);
	glPushMatrix();
	glTranslatef(2.5,0.4,3.8);
	gluCylinder(quadric, 0.1, 0, 0.4, 12, 3);
	glPopMatrix();
	glPopMatrix();


	/* asa direita*/
	glPushMatrix();
	glTranslatef(0.0,0.3,1.8);
	glBegin(GL_POLYGON);

	glTexCoord2fv(cta[0]); glVertex3fv(asaDireita[0]);
	glTexCoord2fv(cta[1]); glVertex3fv(asaDireita[1]);
	glTexCoord2fv(cta[2]); glVertex3fv(asaDireita[2]);
	glTexCoord2fv(cta[3]); glVertex3fv(asaDireita[3]);
	glEnd();
  	glPopMatrix();

	  /* Torpedo Direito 1 */
	glPushMatrix();
	glTranslatef(0, 0, movTorpDir[0]);
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
//	glColor3f(0.0,0.0,0.0);
	glTranslatef(1.5,0.4,1.8);
	gluCylinder(quadric, 0.1, 0.1, 2.0, 12, 3);
	glPopMatrix();

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_FALSE);
	glPushMatrix();
	glTranslatef(1.5,0.4,3.8);
	gluCylinder(quadric, 0.1, 0, 0.4, 12, 3);
	glPopMatrix();
	glPopMatrix();

	/* Torpedo Direito 2*/
	glPushMatrix();
	glTranslatef(0, 0, movTorpDir[1]);
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
//	glColor3f(0.0,0.0,0.0);
	glTranslatef(-2.0,0.4,1.8);
	gluCylinder(quadric, 0.1, 0.1, 2.0, 12, 3);
	glPopMatrix();

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_FALSE);
	glPushMatrix();
	glTranslatef(-2.0,0.4,3.8);
	gluCylinder(quadric, 0.1, 0, 0.4, 12, 3);
	glPopMatrix();
	glPopMatrix();

	/* Torpedo Direito 3*/
	glPushMatrix();
	glTranslatef(0, 0, movTorpDir[2]);

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
//	glColor3f(0.0,0.0,0.0);
	glTranslatef(-2.5,0.4,1.8);
	gluCylinder(quadric, 0.1, 0.1, 2.0, 12, 3);
	glPopMatrix();

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_FALSE);
	glPushMatrix();
	glTranslatef(-2.5,0.4,3.8);
	gluCylinder(quadric, 0.1, 0, 0.4, 12, 3);
	glPopMatrix();
	glPopMatrix();

	/* Base do Helicóptero */
 	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glTranslatef(0.3,-1.4,1.2);
	gluCylinder(quadric, 0.1, 0.1, 2.8, 12, 3);
	glPopMatrix();

 	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glRotatef(90, 1.0, 0, 0);
	glTranslatef(0.3,1.4,0.6);
	gluCylinder(quadric, 0.1, 0.1, 0.7, 12, 3);
	glPopMatrix();

 	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glRotatef(90, 1.0, 0, 0);
	glTranslatef(0.3,3.8,0.6);
	gluCylinder(quadric, 0.1, 0.1, 0.7, 12, 3);
	glPopMatrix();

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glTranslatef(-0.4,-1.4,1.2);
	gluCylinder(quadric, 0.1, 0.1, 2.8, 12, 3);
	glPopMatrix();

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glRotatef(90, 1.0, 0, 0);
	glTranslatef(-0.4,3.8,0.6);
	gluCylinder(quadric, 0.1, 0.1, 0.7, 12, 3);
	glPopMatrix();

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glRotatef(90, 1.0, 0, 0);
	glTranslatef(-0.4,1.4,0.6);
	gluCylinder(quadric, 0.1, 0.1, 0.7, 12, 3);
	glPopMatrix();

	/* Hélice */
	glPushMatrix();

	glTranslatef(0,2.2,2.5);
	glRotatef(AnguloDaHelice, 0, 1.0, 0);
	glTranslatef(0,-2.2,-2.5);

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glTranslatef(0,2.2,2.5);
	glRotatef(90, 1.0, 0, 0);
	gluCylinder(quadric, 0.05, 0.05, 1.0, 12, 3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.8,2.1,2.25);
	glBegin(GL_POLYGON);
	glTexCoord2fv(cta[0]); glVertex3fv(helice[0]);
	glTexCoord2fv(cta[1]); glVertex3fv(helice[1]);
	glTexCoord2fv(cta[2]); glVertex3fv(helice[2]);
	glTexCoord2fv(cta[3]); glVertex3fv(helice[3]);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 0, 1.0, 0);
	glTranslatef(-5.2,2.1,-0.2);
	glBegin(GL_POLYGON);
	glTexCoord2fv(cta[0]); glVertex3fv(helice[0]);
	glTexCoord2fv(cta[1]); glVertex3fv(helice[1]);
	glTexCoord2fv(cta[2]); glVertex3fv(helice[2]);
	glTexCoord2fv(cta[3]); glVertex3fv(helice[3]);
	glEnd();
	glPopMatrix();


	// Segunda Helice

	glPushMatrix();

	glTranslatef(-2.8,1.8,2.25);
	//glRotatef(45, 0, 1.0, 0);
	glBegin(GL_POLYGON);
	glTexCoord2fv(cta[0]); glVertex3fv(helice[0]);
	glTexCoord2fv(cta[1]); glVertex3fv(helice[1]);
	glTexCoord2fv(cta[2]); glVertex3fv(helice[2]);
	glTexCoord2fv(cta[3]); glVertex3fv(helice[3]);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 0, 1.0, 0);
	glTranslatef(-5.2,1.8,-0.2);
	glBegin(GL_POLYGON);
	glTexCoord2fv(cta[0]); glVertex3fv(helice[0]);
	glTexCoord2fv(cta[1]); glVertex3fv(helice[1]);
	glTexCoord2fv(cta[2]); glVertex3fv(helice[2]);
	glTexCoord2fv(cta[3]); glVertex3fv(helice[3]);
	glEnd();
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	glEndList();

}

void display(void){
  glEnable(GL_DEPTH_TEST);
  glDeleteLists(helicoptero, 1);
  composes_helicopter();

  glDepthMask(GL_TRUE);
  glClearColor(1.0,1.0,1.0,1.0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  glPushMatrix();

  /* Posição do Observador */
  Observador[0]=raioxz*cos(2*PI*eixoxz/360);
  Observador[2]=raioxz*sin(2*PI*eixoxz/360);
  gluLookAt(Observador[0],Observador[1],Observador[2],look[0],look[1],look[2],0.0,1.0,0.0);

  if(textures){
    glEnable(GL_TEXTURE_2D);
  }
  else{
    glDisable(GL_TEXTURE_2D);
  }
  glColor4f(PLAN_COLOR);
  glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
  glBindTexture(GL_TEXTURE_2D,texturaDoPlano);

  glBegin(GL_QUADS);
  glTexCoord2fv(ctp[0]);  glVertex3f(-30,0,30);
  glTexCoord2fv(ctp[1]);  glVertex3f(30,0,30);
  glTexCoord2fv(ctp[2]);  glVertex3f(30,0,-30);
  glTexCoord2fv(ctp[3]);  glVertex3f(-30,0,-30);
  glEnd();
  glTranslatef(0.0,2.0,-3.0);

  glColor4f(HELICOPTER_COLOR);
  glBindTexture(GL_TEXTURE_2D,texturaDoHelicoptero);
  glCallList(helicoptero);

  glPopMatrix();
  glutSwapBuffers();
}


void special(int key, int x, int y){
	switch (key) {
		case GLUT_KEY_UP:
			if(ligado == 1){
				movVertical = movVertical + 0.05;
			}
			glutPostRedisplay();
    		break;
		case GLUT_KEY_DOWN:
			movVertical = movVertical - 0.05;
			if(movVertical < -0.55)
			{
				movVertical = -0.55;
			}
			glutPostRedisplay();
			break;
		case GLUT_KEY_LEFT:
			if (ligado == 1)
			{
	 			movHorizontal = movHorizontal + 0.05;
			}
			glutPostRedisplay();
			break;
		case GLUT_KEY_RIGHT:
			if (ligado == 1)
			{
				movHorizontal = movHorizontal - 0.05;
			}
			glutPostRedisplay();
			break;
	}
}

void keyboard(unsigned char key, int x, int y){
	switch (key) {
		case 27:
			exit(0);
			break;
		case 'i':
			ligado = 1;
			glutPostRedisplay();
			break;
		case 'I':
			if(movVertical <= -0.55)
			{
				ligado = 0;
			}
			glutPostRedisplay();
			break;
		case 'm': //tiro
		case 'M':
			tiro = 1;
			glutPostRedisplay();
			break;
		case 't': //atirar torpedo esquerdo
			torpedoEsquerdo = 1;
			glutPostRedisplay();
			if(torpleft<3){
				torpleft++;
			//	torpedoEsquerdo = 0;
			}
			break;
		case 'T': //atirar torpedo direito
			torpedoDireito = 1;
			glutPostRedisplay();
			if(torprigth<3){
				torprigth++;
				//torpedoDireito = 0;
			}
			break;
		case 'w':
		case 'W':
			Observador[1]=Observador[1]+1;
			glutPostRedisplay();
			break;
		case 'a':
		case 'A':
			eixoxz=eixoxz+2;
			glutPostRedisplay();
			break;
		case 's':
		case 'S':
			Observador[1] =Observador[1]-1;
			glutPostRedisplay();
			break;
		case 'd':
		case 'D':
			eixoxz=eixoxz-2;
			glutPostRedisplay();
			break;
		case 'r':
			raioxz=raioxz+1;
			glutPostRedisplay();
			break;
		case 'R':
			raioxz=raioxz-1;
			if(raioxz==0){
				raioxz=1;
			}
		glutPostRedisplay();
		break;
	}
}

void load_textures(void){
  IMAGE *img;
  GLenum gluerr;

  /* textura do plano */
  glGenTextures(1, &texturaDoPlano);
  glBindTexture(GL_TEXTURE_2D, texturaDoPlano);

  if(!(img=ImageLoad(PLAN_TEXTURE))) {
    fprintf(stderr,"Error reading a texture.\n");
    exit(-1);
  }

  gluerr=gluBuild2DMipmaps(GL_TEXTURE_2D, 3,
			   img->sizeX, img->sizeY,
			   GL_RGB, GL_UNSIGNED_BYTE,
			   (GLvoid *)(img->data));
  if(gluerr){
    fprintf(stderr,"GLULib%s\n",gluErrorString(gluerr));
    exit(-1);
  }

  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);

  /* textura do helicoptero */
  glGenTextures(1, &texturaDoHelicoptero);
  glBindTexture(GL_TEXTURE_2D, texturaDoHelicoptero);


  if(!(img=ImageLoad(HELICOPTER_TEXTURE))) {
    fprintf(stderr,"Error reading a texture.\n");
    exit(-1);
  }

  gluerr=gluBuild2DMipmaps(GL_TEXTURE_2D, 3,
			   img->sizeX, img->sizeY,
			   GL_RGB, GL_UNSIGNED_BYTE,
			   (GLvoid *)(img->data));
  if(gluerr){
    fprintf(stderr,"GLULib%s\n",gluErrorString(gluerr));
    exit(-1);
  }

  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);

}

void init(){
  load_textures();
  composes_helicopter();
  glShadeModel(GL_FLAT);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
}

/* Controle da Hélice, tiro e torpedo */
void controle()
{
	if (ligado == 1)
	{
		AnguloDaHelice = (AnguloDaHelice + 15) % 360;
		glutPostRedisplay();
	}

	if (tiro == 1)
	{
		movBala = movBala + 0.05;
		if (movBala > 15.0)
		{
			   tiro = 0;
			   movBala = 6.0;
		}
		glutPostRedisplay();
	}

	if (torpedoEsquerdo == 1)
	{
		movTorpEsq[torpleft] = movTorpEsq[torpleft] + 0.05;
		if(movTorpEsq[torpleft] > 15.0)
		{
			torpedoEsquerdo = 1;
		}
		glutPostRedisplay();
	}

	if (torpedoDireito == 1)
	{
		movTorpDir[torprigth] = movTorpDir[torprigth] + 0.05;
		if(movTorpDir[torprigth] > 15.0)
		{
			torpedoDireito = 1;
		}
		glutPostRedisplay();
	}
}

int main(int argc,char **argv){
  glutInitWindowPosition(0,0);
  glutInitWindowSize(Largura,Altura);
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE);

  if(!glutCreateWindow("HELICOPTERO")) {
    fprintf(stderr,"Error opening a window.\n");
    exit(-1);
  }

  init();

  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutIdleFunc(controle);
  glutMainLoop();

  return(0);
}
