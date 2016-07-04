#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include "image.h"
//  #include "image.c"
//#include <windows.h>	// Arquivo Header do Windows
#include <gl\gl.h>	// Arquivo Header da biblioteca OpenGL32
#include <GL/glext.h>
//-lglut32 -lglu32 -lopengl32 -lgdi32 -lwinmm
#define PI 3.1415

#define COORD_TEXTURA_PLANO 1.0
#define COORD_TEXTURA_AVIAO 1.0
#define COR_DO_PLANO 0.52,0.52,0.78,1.0
#define COR_DO_AVIAO 0.3,0.52,0.18,1.0
#define TEXTURA_DO_PLANO "montanhas2.rgb"
#define TEXTURA_DO_AVIAO "images.rgb"
#define TAM_HELICOPETERO 0.5
#define DELTA 1;
#define VELOX 100
#define DESLOC 0.05
//#define N_PACOTES 6
#define NUM_PARTIC    1000          /* Numero de Particulas  */
#define NUM_PEDAC       1000            /* Numero de pedacos     */
#define PAUSE              0
#define NORMALIZE_SPEED    1
#define QUIT               2

// EXPLOSÃO

/* Uma particula */

struct particleData {
	float   position[3];
	float   speed[3];
	float   color[3];
};
typedef struct particleData    particleData;

/* pedaços */

struct debrisData {
	float   position[3];
	float   speed[3];
	float   orientation[3];        /* angulos de rotação x, y, z */
	float   orientationSpeed[3];
	float   color[3];
	float   scale[3];
};
typedef struct debrisData    debrisData;
particleData     particles[NUM_PARTIC];
debrisData       debris[NUM_PEDAC];
int              fuel = 0;
GLfloat  light0Amb[4] =  { 1.0, 0.6, 0.2, 1.0 };
GLfloat  light0Dif[4] =  { 1.0, 0.6, 0.2, 1.0 };
GLfloat  light0Spec[4] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat  light0Pos[4] =  { 0.0, 0.0, 0.0, 1.0 };

GLfloat  light1Amb[4] =  { 0.0, 0.0, 0.0, 1.0 };
GLfloat  light1Dif[4] =  { 1.0, 1.0, 1.0, 1.0 };
GLfloat  light1Spec[4] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat  light1Pos[4] =  { 0.0, 5.0, 5.0, 0.0 };

GLfloat  materialAmb[4] = { 0.25, 0.22, 0.26, 1.0 };
GLfloat  materialDif[4] = { 0.63, 0.57, 0.60, 1.0 };
GLfloat  materialSpec[4] = { 0.99, 0.91, 0.81, 1.0 };
GLfloat  materialShininess = 27.8;

int      wantNormalize = 0;
int      wantPause = 0;

// fim das declarações para a explosão

static int ativarHelice = 0;
static int ativarHelice1 = 0;
static int ativarHelice2 = 0;
static int ativarHelice3 = 0;
static int velocidade = VELOX;
static int velocidade2 = VELOX;
static int on_Off = 0;
static int metralhadora_on = 0;
static int lancar_torpedo = 0;
static int delta = DELTA;
static int deltaMissel = 0;
GLfloat subir = -6.0;
GLfloat descer = 0.0;
GLfloat recuar = 0.0;
GLfloat avancar =  -4.0;
GLfloat decolar = 0.0;
GLfloat pousar = -1.0;
GLfloat girar = 0.0;
GLfloat lancarTorpedo1 = 0.0;
GLfloat lancarTorpedo2 = 0.0;
GLfloat lancarTorpedo3 = 0.0;
GLfloat lancarTorpedo4 = 0.0;
GLfloat posicao = 0.0; // calculo da posicao do missel no alvo
GLfloat atirar = 0.0;
GLfloat x = 0.0, y = 0.5, z = 0.0;
int totalDePacotes = 0;

//objetivos

GLfloat pos_X = 0.0, pos_Y = 0.0;
GLfloat descarregar[3] = {0.0, 0.0, 0.0};

GLint WIDTH = 800;
GLint HEIGHT = 600;

GLfloat obs[3] = {0.0, 9.0, 0.0};
GLfloat look[3] = {1.0, 0.0, 0.0};
GLuint  textura_plano;
GLuint  textura_aviao;

GLshort texturas = 1;
GLfloat tetaxz = 0;
GLfloat raioxz = 15;
GLuint  jato;

// FUNÇÕES PARA EXPLODIR

void newSpeed (float dest[3]) {
	float    x;
	float    y;
	float    z;
	float    len;

	x = (2.0 * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;
	y = (2.0 * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;
	z = (2.0 * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;


	if (wantNormalize) {
		len = sqrt (x * x + y * y + z * z);
		if (len) {
			x = x / len;
			y = y / len;
			z = z / len;
		}
	}

	dest[0] = x;
	dest[1] = y;
	dest[2] = z;
}

void newExplosion (void) {
	int    i;
	for (i = 0; i < NUM_PARTIC; i++) {
		particles[i].position[0] = 0.0;
		particles[i].position[1] = 0.0;
		particles[i].position[2] = 0.0;

		particles[i].color[0] = 1.0;
		particles[i].color[1] = 1.0;
		particles[i].color[2] = 0.5;

		newSpeed (particles[i].speed);
	}

	for (i = 0; i < NUM_PEDAC; i++) {
		debris[i].position[0] = 0.0;
		debris[i].position[1] = 0.0;
		debris[i].position[2] = 0.0;

		debris[i].orientation[0] = 0.0;
		debris[i].orientation[1] = 0.0;
		debris[i].orientation[2] = 0.0;

		debris[i].color[0] = 0.7;
		debris[i].color[1] = 0.7;
		debris[i].color[2] = 0.7;

		debris[i].scale[0] = (2.0 *
							  ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;
		debris[i].scale[1] = (2.0 *
							  ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;
		debris[i].scale[2] = (2.0 *
							  ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;

		newSpeed (debris[i].speed);
		newSpeed (debris[i].orientationSpeed);
	}

	fuel = 300;
}

//FIM DAS FUNCOES PARA EXPLODIR

GLfloat ctp[4][2] = {
	{ -COORD_TEXTURA_PLANO, -COORD_TEXTURA_PLANO},
	{ +COORD_TEXTURA_PLANO, -COORD_TEXTURA_PLANO},
	{ +COORD_TEXTURA_PLANO, +COORD_TEXTURA_PLANO},
	{ -COORD_TEXTURA_PLANO, +COORD_TEXTURA_PLANO}
};

GLfloat cta[4][2] = {
	{ -COORD_TEXTURA_AVIAO, -COORD_TEXTURA_AVIAO},
	{ +COORD_TEXTURA_AVIAO, -COORD_TEXTURA_AVIAO},
	{ +COORD_TEXTURA_AVIAO, +COORD_TEXTURA_AVIAO},
	{ -COORD_TEXTURA_AVIAO, +COORD_TEXTURA_AVIAO}
};

void reshape(int width, int height) {
	WIDTH = width;
	HEIGHT = height;

	glViewport(0, 0, (GLint)width, (GLint)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(70.0, width / (float)height, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

void compoe_jato(void) {
	GLUquadricObj * quadric;

	GLfloat asa[][3] = {
		{3.5, -0.7, 5.5},
		{ -3.5, -0.7, 5.5},
		{0.0, -0.7, 4.0} //ponto da asa, vertice central central
	};
	//asa da cauda
	GLfloat asa2[][3] = {
		{1.5, 0.1, 1.0},
		{ -1.5, 0.1, 1.0},
		{0.0, 0.1, 0.1} //ponto da asa, vertice central central
	};

	GLfloat cauda[][3] = {
		{0.0, 0.0, 0.0},
		{0.0, 2.0, -1.0},
		{0.0, 2.0, 0.0},
		{0.0, 0.0, 2.0}
	};
	//	 origem= avancar+recuar;
	/* inicia a composicao do jato */
	jato = glGenLists(1);
	glNewList(jato, GL_COMPILE);
	//para movimentar o helicoptero

	glTranslatef(-subir, decolar , recuar);
	glTranslatef(-descer, pousar, avancar);
	glRotatef(-girar, 0.0 , 1.0, 0.0);
	glTranslatef(0.0, 0.0 , -1.5);

	glScalef(TAM_HELICOPETERO, TAM_HELICOPETERO, TAM_HELICOPETERO); //aqui define o tamanho do helicoptero

	/* asas */
	glBegin(GL_TRIANGLES);
	glTexCoord2fv(cta[0]);
	glVertex3fv(asa[0]);
	glTexCoord2fv(cta[1]);
	glVertex3fv(asa[1]);
	glTexCoord2fv(cta[3]);
	glVertex3fv(asa[2]);
	glEnd();

	//asa da cauda
	glBegin(GL_TRIANGLES);
	glTexCoord2fv(cta[0]);
	glVertex3fv(asa2[0]);
	glTexCoord2fv(cta[1]);
	glVertex3fv(asa2[1]);
	glTexCoord2fv(cta[3]);
	glVertex3fv(asa2[2]);
	glEnd();

	/* corpo */
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	gluCylinder(quadric, 0.1, 1.6, 5, 24, 3);

	/* nariz */
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glTranslatef(0, 0, 5.0);
	gluCylinder(quadric, 1.6, 0.35, 2, 24, 3);
	glPopMatrix();

	/* cauda */
	glBegin(GL_POLYGON);
	glTexCoord2fv(cta[0]);
	glVertex3fv(cauda[0]);
	glTexCoord2fv(cta[1]);
	glVertex3fv(cauda[1]);
	glTexCoord2fv(cta[2]);
	glVertex3fv(cauda[2]);
	glTexCoord2fv(cta[3]);
	glVertex3fv(cauda[3]);
	glEnd();

	/*narizinho*/
	glPushMatrix();
	glTranslatef(0, 0., 6.85);
	glScalef(1.1, 1.1, 1.1);
	quadric = gluNewQuadric();
	glColor3f(0.8, 0.8, 0.8);
	glDisable(GL_TEXTURE_2D);
	gluSphere(quadric, 0.35, 12, 12);
	glPopMatrix();

	/* cabine do piloto */
	glTranslatef(0, 0.7, 5.5);
	glRotatef(25.5, 1.0, 0.0, 0.0);
	glPushMatrix();
	glScalef(3.0, 2.5, 3.5);
	quadric = gluNewQuadric();
	glColor3f(0.6, 0.8, 1);
	glDisable(GL_TEXTURE_2D);
	gluSphere(quadric, 0.35, 12, 12);
	glPopMatrix();

	// bases das helices superior
	glTranslatef(0, -0.2, -1.5);
	glRotatef(-120.0, 1.0, 0.0, 0.0);
	glPushMatrix();
	quadric = gluNewQuadric();
	glColor3f(0.0, 0.0, 0.0);
	gluQuadricTexture(quadric, GL_TRUE);
	gluCylinder(quadric, 0.3, 0.3, 1, 24, 3);
	glPopMatrix();

	// base das helices da cauda
	glPushMatrix();
	glTranslatef(0.3, 4.35, 0.8);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	quadric = gluNewQuadric();
	glColor3f(0.0, 0.0, 0.0);
	gluQuadricTexture(quadric, GL_TRUE);
	gluCylinder(quadric, 0.1, 0.1, 0.3, 24, 3);
	glPopMatrix();

	//pernas do helicoptero
	//perna1
	glPushMatrix();
	glTranslatef(0.8, -1.2, -2.25);
	glRotatef(-200.0, 0.0, 1.0, 0.0);
	quadric = gluNewQuadric();
	glColor3f(0.0, 0.0, 0.0);
	gluQuadricTexture(quadric, GL_TRUE);
	gluCylinder(quadric, 0.1, 0.1, 0.8, 24, 3); //(raio,raio,comprimentoDocilindro,quantidadeDefaticas,naosei)
	glPopMatrix();

	//perna2
	glPushMatrix();
	glTranslatef(-0.8, -1.2, -2.25);
	glRotatef(200.0, 0.0, 1.0, 0.0);
	quadric = gluNewQuadric();
	glColor3f(0.0, 0.0, 0.0);
	gluQuadricTexture(quadric, GL_TRUE);
	gluCylinder(quadric, 0.1, 0.1, 0.8, 24, 3);
	glPopMatrix();

	//perna3
	glPushMatrix();
	glTranslatef(-0.8, 0.35, -1.75);
	glRotatef(210.0, 1.0, 0.0, 0.0);
	quadric = gluNewQuadric();
	glColor3f(0.0, 0.0, 0.0);
	gluQuadricTexture(quadric, GL_TRUE);
	gluCylinder(quadric, 0.1, 0.1, 1.3, 24, 3);
	glPopMatrix();

	//perna4
	glPushMatrix();
	glTranslatef(0.8, 0.35, -1.75);
	glRotatef(210.0, 1.0, 0.0, 0.0);
	quadric = gluNewQuadric();
	glColor3f(0.0, 0.0, 0.0);
	gluQuadricTexture(quadric, GL_TRUE);
	gluCylinder(quadric, 0.1, 0.1, 1.3, 24, 3);
	glPopMatrix();

	//base da perna lado esqrdo
	glPushMatrix();
	glTranslatef(0.7, 1.5, -2.80);
	glRotatef(95.0, 1.0, 0.0, 0.0);
	glRotatef(8.0, 0.0, 1.0, 0.0);
	quadric = gluNewQuadric();
	glColor3f(0.0, 0.0, 0.0);
	gluQuadricTexture(quadric, GL_TRUE);
	gluCylinder(quadric, 0.1, 0.1, 3.3, 24, 3);
	glPopMatrix();

	//base da perna lado direito
	glPushMatrix();
	glTranslatef(-0.7, 1.5, -2.80);
	glRotatef(95.0, 1.0, 0.0, 0.0);
	glRotatef(-8.0, 0.0, 1.0, 0.0);
	quadric = gluNewQuadric();
	glColor3f(0.0, 0.0, 0.0);
	gluQuadricTexture(quadric, GL_TRUE);
	gluCylinder(quadric, 0.1, 0.1, 3.3, 24, 3);
	glPopMatrix();

	// begin base do torpedo da esqrda
	glPushMatrix();
	glTranslatef(2.0, -1.0, -2.18);
	glRotatef(0.0, 1.0, 0.0, 0.0);
	quadric = gluNewQuadric();
	glColor3f(0.0, 0.0, 0.0);
	gluQuadricTexture(quadric, GL_TRUE);
	gluCylinder(quadric, 0.05, 0.05, 0.225, 24, 3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.6, -1.0, -2.18);
	glRotatef(0.0, 1.0, 0.0, 0.0);
	quadric = gluNewQuadric();
	glColor3f(0.0, 0.0, 0.0);
	gluQuadricTexture(quadric, GL_TRUE);
	gluCylinder(quadric, 0.05, 0.05, 0.225, 24, 3);
	glPopMatrix();

	// base do torpedo
	glPushMatrix();
	glTranslatef(2.7, -1.0, -2.18);
	glRotatef(0.0, 1.0, 0.0, 0.0);
	quadric = gluNewQuadric();
	glColor3f(0.0, 0.0, 0.0);
	gluQuadricTexture(quadric, GL_TRUE);
	gluCylinder(quadric, 0.05, 0.05, 0.225, 24, 3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.3, -1.0, -2.18);
	glRotatef(0.0, 1.0, 0.0, 0.0);
	quadric = gluNewQuadric();
	glColor3f(0.0, 0.0, 0.0);
	gluQuadricTexture(quadric, GL_TRUE);
	gluCylinder(quadric, 0.05, 0.05, 0.225, 24, 3);
	glPopMatrix(); //end base dos torpedos da esquerda

	// begin base do torpedo da direita
	glPushMatrix();
	glTranslatef(-2.0, -1.0, -2.18);
	glRotatef(0.0, 1.0, 0.0, 0.0);
	quadric = gluNewQuadric();
	glColor3f(0.0, 0.0, 0.0);
	gluQuadricTexture(quadric, GL_TRUE);
	gluCylinder(quadric, 0.05, 0.05, 0.225, 24, 3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.6, -1.0, -2.18);
	glRotatef(0.0, 1.0, 0.0, 0.0);
	quadric = gluNewQuadric();
	glColor3f(0.0, 0.0, 0.0);
	gluQuadricTexture(quadric, GL_TRUE);
	gluCylinder(quadric, 0.05, 0.05, 0.225, 24, 3);
	glPopMatrix();

	// base do torpedo
	glPushMatrix();
	glTranslatef(-2.7, -1.0, -2.18);
	glRotatef(0.0, 1.0, 0.0, 0.0);
	quadric = gluNewQuadric();
	glColor3f(0.0, 0.0, 0.0);
	gluQuadricTexture(quadric, GL_TRUE);
	gluCylinder(quadric, 0.05, 0.05, 0.225, 24, 3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.3, -1.0, -2.18);
	glRotatef(0.0, 1.0, 0.0, 0.0);
	quadric = gluNewQuadric();
	glColor3f(0.0, 0.0, 0.0);
	gluQuadricTexture(quadric, GL_TRUE);
	gluCylinder(quadric, 0.05, 0.05, 0.225, 24, 3);
	glPopMatrix(); //end base dos torpedos da esquerda

	/* cabine do piloto */
	if(lancar_torpedo <= 4 /*|| lancar_torpedo <= 2*/) {
		//torpedos
		if(-lancarTorpedo1 >= -50 + posicao) {
			glPushMatrix();
			glTranslatef(0.0, -lancarTorpedo1, 0.0);
			glTranslatef(2.5, -1.0, -2.3);
			glScalef(0.8, 2.5, 0.5);
			quadric = gluNewQuadric();
			glColor3f(0.3, 0.7, 0.3);
			glDisable(GL_TEXTURE_2D);
			gluSphere(quadric, 0.35, 12, 12);
			glPopMatrix();

			//begin cauda do torpedo3
			glPushMatrix();
			glTranslatef (0.3, 4.35, 0.8);
			glTranslatef (0.2, -4.5, -5.1);
			glTranslatef (2.0, -lancarTorpedo1, 2.0);
			glScalef (0.6, 0.2, 0.05);
			glColor3f(0.3, 0.3, 0.3);
			glutSolidCube(1.0);
			glPopMatrix();

			glPushMatrix();
			glTranslatef (0.3, 4.35, 0.8);
			glTranslatef (0.2, -4.5, -5.1);
			glTranslatef (2.0, -lancarTorpedo1, 2.0);
			glScalef (0.05, 0.2, 0.6);
			glColor3f(0.3, 0.3, 0.3);
			glutSolidCube(1.0);
			glPopMatrix(); //end cauda


		}
		if(-lancarTorpedo3 >= -50 + posicao) {
			glPushMatrix();
			glTranslatef(0.0, -lancarTorpedo3, 0.0);
			glTranslatef(1.8, -1.0, -2.3);
			//glRotatef(25.5, 1.0, 0.0, 0.0);
			glScalef(0.8, 2.5, 0.5);
			quadric = gluNewQuadric();
			glColor3f(0.3, 0.7, 0.3);
			glDisable(GL_TEXTURE_2D);
			gluSphere(quadric, 0.35, 12, 12);
			glPopMatrix();

			//begin cauda do torpedo1
			glPushMatrix();
			glTranslatef (0.3, 4.35, 0.8);
			glTranslatef (-0.5, -4.5, -5.1);
			glTranslatef (2.0, -lancarTorpedo3, 2.0);
			glScalef (0.6, 0.2, 0.05);
			glColor3f(0.3, 0.3, 0.3);
			glutSolidCube(1.0);
			glPopMatrix();

			glPushMatrix();
			glTranslatef (0.3, 4.35, 0.8);
			glTranslatef (-0.5, -4.5, -5.1);
			glTranslatef (2.0, -lancarTorpedo3, 2.0);
			glScalef (0.05, 0.2, 0.6);
			glColor3f(0.3, 0.3, 0.3);
			glutSolidCube(1.0);
			glPopMatrix(); //end cauda

		}

		if(-lancarTorpedo2 >= -50 + posicao) {
			glPushMatrix();
			glTranslatef(0.0, -lancarTorpedo2, 0.0);
			glTranslatef(-2.5, -1.0, -2.3);
			//glRotatef(25.5, 1.0, 0.0, 0.0);
			glScalef(0.8, 2.5, 0.5);
			quadric = gluNewQuadric();
			glColor3f(0.3, 0.7, 0.3);
			glDisable(GL_TEXTURE_2D);
			gluSphere(quadric, 0.35, 12, 12);
			glPopMatrix();

			//begin cauda do torpedo4
			glPushMatrix();
			glTranslatef (0.3, 4.35, 0.8);
			glTranslatef (-4.8, -4.5, -5.1);
			glTranslatef (2.0, -lancarTorpedo2, 2.0);
			glScalef (0.6, 0.2, 0.05);
			glColor3f(0.3, 0.3, 0.3);
			glutSolidCube(1.0);
			glPopMatrix();

			glPushMatrix();
			glTranslatef (0.3, 4.35, 0.8);
			glTranslatef (-4.8, -4.5, -5.1);
			glTranslatef (2.0, -lancarTorpedo2, 2.0);
			glScalef (0.05, 0.2, 0.6);
			glColor3f(0.3, 0.3, 0.3);
			glutSolidCube(1.0);
			glPopMatrix(); //end cauda
		}
		if(-lancarTorpedo4 >= -50 + posicao) {
			glPushMatrix();
			glTranslatef(0.0, -lancarTorpedo4, 0.0);
			glTranslatef(-1.8, -1.0, -2.3);
			//glRotatef(25.5, 1.0, 0.0, 0.0);
			glScalef(0.8, 2.5, 0.5);
			quadric = gluNewQuadric();
			glColor3f(0.3, 0.7, 0.3);
			glDisable(GL_TEXTURE_2D);
			gluSphere(quadric, 0.35, 12, 12);
			glPopMatrix(); //fim do desenho dos torpedos

			//begin cauda do torpedo2
			glPushMatrix();
			glTranslatef (0.3, 4.35, 0.8);
			glTranslatef (-4.1, -4.5, -5.1);
			glTranslatef (2.0, -lancarTorpedo4, 2.0);
			glScalef (0.6, 0.2, 0.05);
			glColor3f(0.3, 0.3, 0.3);
			glutSolidCube(1.0);
			glPopMatrix();

			glPushMatrix();
			glTranslatef (0.3, 4.35, 0.8);
			glTranslatef (-4.1, -4.5, -5.1);
			glTranslatef (2.0, -lancarTorpedo4, 2.0);
			glScalef (0.05, 0.2, 0.6);
			glColor3f(0.3, 0.3, 0.3);
			glutSolidCube(1.0);
			glPopMatrix(); 
			//fim cauda
		}
	}
	//metralhadora
	if(metralhadora_on == 1) {
		x = 0.0;
		y = 0.5;
		z = 0.0;
	}
	glPushMatrix();
	glTranslatef(0.3, -2.5, -1.8);
	glScalef(0.5, 0.5, 0.5);
	quadric = gluNewQuadric();
	glColor3f(0.0, 0.0, 0);
	glDisable(GL_TEXTURE_2D);
	gluSphere(quadric, 0.35, 12, 12);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.3, -2.5, -1.8);
	glScalef(0.5, 0.5, 0.5);
	quadric = gluNewQuadric();
	glColor3f(0.0, 0.0, 0);
	glDisable(GL_TEXTURE_2D);
	gluSphere(quadric, 0.35, 12, 12);
	glPopMatrix(); //fim do desenho dos t

	glPushMatrix();
	glTranslatef(0.3, -2.6, -1.8);
	glScalef(0.2, 0.5, 0.2);
	quadric = gluNewQuadric();
	glColor3f(x, y, z);
	glDisable(GL_TEXTURE_2D);
	gluSphere(quadric, 0.35, 12, 12);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.3, -2.6, -1.8);
	glScalef(0.2, 0.5, 0.2);
	quadric = gluNewQuadric();
	glColor3f(x, y, z);
	glDisable(GL_TEXTURE_2D);
	gluSphere(quadric, 0.35, 12, 12);
	glPopMatrix(); 
	//fim do desenho da metralhadora

	if(metralhadora_on != 0) {
		x = 1.0;
		y = 1.0;
		z = 0.2;
		glPushMatrix();
		glTranslatef(0.0, -atirar, 0.0);
		glTranslatef(-0.3, -2.6, -1.8);
		glScalef(0.2, 0.5, 0.2);
		quadric = gluNewQuadric();
		glColor3f(1.0, 0.0, 0.0);
		glDisable(GL_TEXTURE_2D);
		gluSphere(quadric, 0.35, 12, 12);
		glPopMatrix(); //fim do desenho da metralhadora

		glPushMatrix();
		glTranslatef(0.0, -atirar, 0.0);
		glTranslatef(0.3, -2.5, -1.8);
		glScalef(0.2, 0.5, 0.2);
		quadric = gluNewQuadric();
		glColor3f(1.0, 0.0, 0.0);
		glDisable(GL_TEXTURE_2D);
		gluSphere(quadric, 0.35, 12, 12);
		glPopMatrix();
	}
	glEndList();

}

int visao = 0;
void display(void) {
	int i;
	compoe_jato();
	glDepthMask(GL_TRUE);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	/* calcula a posicao do observador */
	obs[0] = raioxz * cos(2 * PI * tetaxz / 360);
	obs[2] = raioxz * sin(2 * PI * tetaxz / 360);
	/*
	if(visao == 1) {
		glRotatef(girar, 0.0, 1.0, 1.0);
		gluLookAt ((-subir - descer) + 0.5, 5.0, (recuar + avancar) - 4, (-subir - descer) + 0.5,
		 1.0, (recuar + avancar) + 1, 1, 1, 75);
	} else {
		gluLookAt(obs[0], obs[1], obs[2], look[0], look[1], look[2], 0.0, 1.0, 0.0);
	}
	*/
	gluLookAt(obs[0], obs[1], obs[2], look[0], look[1], look[2], 0.0, 1.0, 0.0);

	if(fuel == 0) {
		
		
		glEnable (GL_LIGHTING);
		glDisable (GL_LIGHT0);
		glEnable (GL_DEPTH_TEST);
				
		glPushMatrix();
		glTranslatef ( (descarregar[0] / 2), 0, 0.0 + (descarregar[0] / 2.8));
		glRotatef(45, 0, 45, 0);
		glColor3f(0.0, 0.4, 0.0);
		glScalef (3,3,3);
		glColor3f(0.647059, 0.264706, 0.094706);
		glutSolidOctahedron(); //(2.2, 2, 100, 10);
		glPopMatrix(); 
		
	}

	if (fuel > 0) {
		glPushMatrix ();

		glDisable (GL_LIGHTING);
		glDisable (GL_DEPTH_TEST);

		glBegin (GL_POINTS);

		for (i = 0; i < NUM_PARTIC; i++) {
			glColor3fv (particles[i].color);
			glVertex3fv (particles[i].position);
		}

		glEnd ();

		glPopMatrix ();

		glEnable (GL_LIGHTING);
		glEnable (GL_LIGHT0);
		glEnable (GL_DEPTH_TEST);

		glNormal3f (0.0, 0.0, 1.0);

		for (i = 0; i < NUM_PEDAC; i++) {
			glColor3fv (debris[i].color);

			glPushMatrix ();

			glTranslatef (debris[i].position[0],
						  debris[i].position[1],
						  debris[i].position[2]);

			glRotatef (debris[i].orientation[0], 1.0, 0.0, 0.0);
			glRotatef (debris[i].orientation[1], 0.0, 1.0, 0.0);
			glRotatef (debris[i].orientation[2], 0.0, 0.0, 1.0);

			glScalef (debris[i].scale[0],
					  debris[i].scale[1],
					  debris[i].scale[2]);

			glBegin (GL_TRIANGLES);
			glVertex3f (0.0, 0.5, 0.0);
			glVertex3f (-0.25, 0.0, 0.0);
			glVertex3f (0.25, 0.0, 0.0);
			glEnd ();

			glPopMatrix ();
		}
	}

	/* habilita/desabilita uso de texturas*/
	if(texturas) {
		glEnable(GL_TEXTURE_2D);
	} else {
		glDisable(GL_TEXTURE_2D);
	}


	glColor4f(COR_DO_PLANO);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, textura_plano);

	glBegin(GL_QUADS);
	glTexCoord2fv(ctp[0]);
	glVertex3f(-10, 0, 10);
	glTexCoord2fv(ctp[1]);
	glVertex3f(10, 0, 10);
	glTexCoord2fv(ctp[2]);
	glVertex3f(10, 0, -10);
	glTexCoord2fv(ctp[3]);
	glVertex3f(-10, 0, -10);
	glEnd();
	glTranslatef(0.0, 2.0, -3.0);
	glColor4f(COR_DO_AVIAO);
	glBindTexture(GL_TEXTURE_2D, textura_aviao);
	glCallList(jato);

	//helice da cauda1
	glPushMatrix();
	glTranslatef (0.3, 4.35, 0.8);
	glRotatef ( -ativarHelice2, 1.0, 0.0, .0);
	glTranslatef (-2.0, -2.0, -2.0);// para girar em um determinado ponto fixo
	glTranslatef (2.0, 2.0, 2.0);
	glScalef (0.05, 1.5, 0.2);
	glColor3f(0.3, 0.3, 0.3);
	glutSolidCube(1.0);
	glPopMatrix();

	//helice cauda2
	glPushMatrix();
	glTranslatef (0.3, 4.35, 0.8);
	glRotatef ( -ativarHelice3, 1.0, 0.0, 0.0);
	glTranslatef (3.0, 3.0, 3.0);
	glTranslatef (-3.0, -3.0, -3.0);// para girar em um determinado ponto fixo
	glScalef (0.05, 0.2, 1.5);
	glColor3f(0.2, 0.2, 0.2);
	glutSolidCube(1.0);
	glPopMatrix();

	/* helices superior */
	glRotatef ( -ativarHelice, 0.0, 0.0, 1.0);
	glRotatef ( 0.0, 0.0, 0.0, 0.0);
	glTranslatef (0.0, 0.0, 1.0);// para girar em um determinado ponto fixo
	glPushMatrix();
	glScalef (0.35, 7.0, 0.05);
	glColor3f(0.3, 0.3, 0.3);
	glutSolidCube(1.0);
	glPopMatrix();
	//printf("helice1=%d\n", ativarHelice);

	//helices superior
	glRotatef ( -ativarHelice1, 0.0, 0.0, 1.0);
	glRotatef ( 90.0, 0.0, 0.0, 1.0);
	glTranslatef (0.0, 0.0, 0.0);// para girar em um determinado ponto fixo
	glPushMatrix();
	glScalef (0.35, 7.0, 0.05);
	glColor3f(0.3, 0.3, 0.3);
	glutSolidCube(1.0);
	glPopMatrix();
	glPopMatrix();

	glutSwapBuffers();
}

void carregar_texturas(void) {
	IMAGE * img;
	GLenum gluerr;
	/* textura do plano */
	glGenTextures(1, & textura_plano);
	glBindTexture(GL_TEXTURE_2D, textura_plano);

	if(!(img = ImageLoad(TEXTURA_DO_PLANO))) {
		fprintf(stderr, "Error reading a texture.\n");
		exit(-1);
	}

	gluerr = gluBuild2DMipmaps(GL_TEXTURE_2D, 3,
							   img->sizeX, img->sizeY,
							   GL_RGB, GL_UNSIGNED_BYTE,
							   (GLvoid * )(img->data));
	if(gluerr) {
		fprintf(stderr, "GLULib%s\n", gluErrorString(gluerr));
		exit(-1);
	}

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	/* textura do aviao */
	glGenTextures(1, & textura_aviao);
	glBindTexture(GL_TEXTURE_2D, textura_aviao);


	if(!(img = ImageLoad(TEXTURA_DO_AVIAO))) {
		fprintf(stderr, "Error reading a texture.\n");
		exit(-1);
	}

	gluerr = gluBuild2DMipmaps(GL_TEXTURE_2D, 3,
							   img->sizeX, img->sizeY,
							   GL_RGB, GL_UNSIGNED_BYTE,
							   (GLvoid * )(img->data));
	if(gluerr) {
		fprintf(stderr, "GLULib%s\n", gluErrorString(gluerr));
		exit(-1);
	}
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
}

//funcao para girar helice

void girarHelice( int value ) {
	if( on_Off == 1 ) {
		if(velocidade >= 75) {
			deltaMissel = deltaMissel + 1;
			velocidade = velocidade - 1;
			//break;
		}
		if(ativarHelice == 400) {
			ativarHelice = 0;
			ativarHelice1 = 0;
			delta = 0;
		}
		ativarHelice = (ativarHelice + deltaMissel) % 360;
		ativarHelice1 = (ativarHelice1) % 360;
		ativarHelice2 = (ativarHelice2 + deltaMissel) % 360;
		ativarHelice3 = (ativarHelice3 + deltaMissel) % 360;
		glutPostRedisplay();
	}
	glutTimerFunc( velocidade, girarHelice, 1);// 10ms
}

//funcao para lancar torpedo

void funcTorpedo( int value ) {

	if( lancar_torpedo > 0 ) {
		if(velocidade2 != 5) {
			delta = delta + 0.5;
			velocidade2 = velocidade2 - 5;
			//break;
		}

		if(lancar_torpedo == 1) {
			lancarTorpedo1 = lancarTorpedo1 + delta;
		}
		if(lancar_torpedo == 3) {
			lancarTorpedo3 = lancarTorpedo3 + delta;
		}
		if(lancar_torpedo == 4) {
			lancarTorpedo4 = lancarTorpedo4 + delta;
		}
		if(lancar_torpedo == 2) {
			lancarTorpedo2 = lancarTorpedo2 + delta;
		}
		glutPostRedisplay();
	}
	glutTimerFunc( velocidade2, funcTorpedo, 1);// 10ms
}

//funcao para atirar

void funcMetralhadora(int value ) {
	if( metralhadora_on == 1 ) {
		if(atirar >= 10) {
			atirar = 0;
		}
		atirar = atirar + 0.5;
		glutPostRedisplay();
	}
	atirar = 0;
	glutTimerFunc( 0.5, funcMetralhadora, 1);// 10ms
}

void special(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		if(ativarHelice != 0 && subir <= 1 && on_Off == 1) {
			subir = subir + DESLOC;
			descer = descer + DESLOC;
			//cont++;
			glutPostRedisplay();
			break;
		}
		break;
	case GLUT_KEY_DOWN:
		if(ativarHelice != 0 && descer > -1.0 && on_Off == 1) {
			descer = descer - DESLOC;
			subir = subir - DESLOC;
			//cont--;
		}
		glutPostRedisplay();

		break;

	case GLUT_KEY_LEFT:
		if(ativarHelice != 0 && avancar <= 3.65 && on_Off == 1) {
			avancar = avancar + DESLOC;
			recuar = recuar + DESLOC;
			glutPostRedisplay();
			break;
		}
	case GLUT_KEY_RIGHT:
		if(ativarHelice != 0 && recuar > -1.0 && on_Off == 1) {
			recuar = recuar - DESLOC;
			avancar = avancar - DESLOC;
			glutPostRedisplay();
			break;
		}
	}

}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		exit(0);
		break;
	case '+':
		texturas = !texturas;
		glutPostRedisplay();
		break;

	case 'r':
		raioxz = raioxz + 1;
		glutPostRedisplay();
		break;
	case 'R':
		raioxz = raioxz - 1;
		if(raioxz == 0) {
			raioxz = 1;
		}
		glutPostRedisplay();
		break;

		//movimentar o plano
	case 'w':
		obs[1] = obs[1] + 1;
		glutPostRedisplay();
		break;
	case 's':
		obs[1] = obs[1] - 1;
		glutPostRedisplay();
		break;
	case 'd':
		tetaxz = tetaxz + 2;
		glutPostRedisplay();
		break;
	case 'a':
		tetaxz = tetaxz - 2;
		glutPostRedisplay();
		break;

	case 'i':
		on_Off = 1;
		glutPostRedisplay();
		break;
	case 'I':
		if(pousar <= -1) {
			on_Off = 0;
			ativarHelice = 0;
			ativarHelice1 = 0;
			velocidade = VELOX;
			delta = DELTA;
		}
		glutPostRedisplay();
		break;
	case 't':
		lancar_torpedo = lancar_torpedo == 0 ? 1 : 3;
		velocidade2 = VELOX;
		glutPostRedisplay();
		break;
	case 'T':
		lancar_torpedo = lancar_torpedo == 3 ? 2 : 4;
		velocidade2 = VELOX;
		glutPostRedisplay();
		break;
	case 'm':

		metralhadora_on = 1;
		glutPostRedisplay();
		break;

	case 'M':
		x = 0.0;
		y = 0.5;
		z = 0.0;
		metralhadora_on = 0;
		glutPostRedisplay();
		break;

	case 'g': //girar helicopero
		if(on_Off == 1) {
			girar = girar + 5.5;
		}

		glutPostRedisplay();
		break;
	case 'G': // girar helicoptero
		if(on_Off == 1) {
			girar = girar - 5.5;
		}
		glutPostRedisplay();
		break;

	case 'v': // para escolher visao do piloto
		visao = 1;
		glutPostRedisplay();
		break;

	case 'V': //voltar para a visao do plano
		visao = 0;
		glutPostRedisplay();
		break;

	case 'f':
		if(decolar <= 2.2 && on_Off == 1) {
			decolar = decolar + DESLOC;
			pousar = pousar + DESLOC;
		}
		break;
	case 'F':
		if(pousar > -1 && on_Off == 1) {
			pousar = pousar - DESLOC;
			decolar = decolar - DESLOC;
		}
		break;
	case 'x':
		newExplosion();
		break;
	}
}

void idle (void) {
	int    i;
	if (!wantPause) {
		if (fuel > 0) {
			for (i = 0; i < NUM_PARTIC; i++) {
				particles[i].position[0] += particles[i].speed[0] * 0.2;
				particles[i].position[1] += particles[i].speed[1] * 0.2;
				particles[i].position[2] += particles[i].speed[2] * 0.2;

				particles[i].color[0] -= 1.0 / 500.0;
				if (particles[i].color[0] < 0.0) {
					particles[i].color[0] = 0.0;
				}

				particles[i].color[1] -= 1.0 / 100.0;
				if (particles[i].color[1] < 0.0) {
					particles[i].color[1] = 0.0;
				}

				particles[i].color[2] -= 1.0 / 50.0;
				if (particles[i].color[2] < 0.0) {
					particles[i].color[2] = 0.0;
				}
			}
			for (i = 0; i < NUM_PEDAC; i++) {
				debris[i].position[0] += debris[i].speed[0] * 0.1;
				debris[i].position[1] += debris[i].speed[1] * 0.1;
				debris[i].position[2] += debris[i].speed[2] * 0.1;

				debris[i].orientation[0] += debris[i].orientationSpeed[0] * 10;
				debris[i].orientation[1] += debris[i].orientationSpeed[1] * 10;
				debris[i].orientation[2] += debris[i].orientationSpeed[2] * 10;
			}
			--fuel;
		}
	}
	glutPostRedisplay ();
}

void init() {
	carregar_texturas();
	compoe_jato();
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glutTimerFunc(velocidade, girarHelice, 1 ); //
	glutTimerFunc(velocidade2, funcTorpedo, 20 ); //
	glutTimerFunc( 0.5, funcMetralhadora, 3);// 10ms
	glEnable (GL_LIGHT0);
	glEnable (GL_LIGHT1);
	glLightfv (GL_LIGHT0, GL_AMBIENT, light0Amb);
	glLightfv (GL_LIGHT0, GL_DIFFUSE, light0Dif);
	glLightfv (GL_LIGHT0, GL_SPECULAR, light0Spec);
	glLightfv (GL_LIGHT0, GL_POSITION, light0Pos);
	glLightfv (GL_LIGHT1, GL_AMBIENT, light1Amb);
	glLightfv (GL_LIGHT1, GL_DIFFUSE, light1Dif);
	glLightfv (GL_LIGHT1, GL_SPECULAR, light1Spec);
	glLightfv (GL_LIGHT1, GL_POSITION, light1Pos);
	glLightModelf (GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT, materialAmb);
	glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, materialDif);
	glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, materialSpec);
	glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, materialShininess);
	glEnable (GL_NORMALIZE);
}

int main(int argc, char * * argv) {
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInit( & argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	if(!glutCreateWindow("Helicopter		@RafaelEliasLX")) {
		fprintf(stderr, "Error opening a window.\n");
		exit(-1);
	}
	init();
	glutKeyboardFunc(keyboard);
	glutIdleFunc (idle);
	glutSpecialFunc(special);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	srand (time (NULL));
	glutMainLoop();
	return(0);
}
