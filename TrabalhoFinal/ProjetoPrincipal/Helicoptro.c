/* HELICOPTERO*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
//#include <GL/freeglut.h>
#include <time.h>
#include "image.h"
#include "image.c"

#define PI 3.1415
#define PLAN_TEXTURE_COORD 1.0
#define PLAN_TEXTURE_HELICOPTER 1.0
#define PLAN_COLOR 0.52,0.52,0.78,1.0
#define HELICOPTER_COLOR 0.5,0.52,0.18,1.0
#define PLAN_TEXTURE "mountains.rgb"
#define HELICOPTER_TEXTURE "war.rgb"

GLint WIDTH = 1000;
GLint HEIGHT = 600;

GLfloat obs[3] = {0.0, 7.0, 0.0};
GLfloat look[3] = {0.0, 3.0, 0.0};
GLuint  plan_texture;
GLuint  helicopter_texture;

GLshort enableTexture = 1;
GLfloat axisxz = 0;
GLfloat radiusxz = 15;
GLuint  helicopter;
GLfloat girar = 0.0;

int screwPropellerAngle = 0;
int stop = 0;
int turn = 0;
int x2, y2, z2;
int shoot = 0;
int leftTorpedo = 0;
int rightTorpedo = 0;
float verticalMovement = -0.55;
float horizontalMovement = 0;
float machineGunBulletMovement = 2.0;
float leftTorpedoMovement = 0;
float rightTorpedoMovement = 0;

GLfloat ctp[4][2] = {
	{ -PLAN_TEXTURE_COORD, -PLAN_TEXTURE_COORD},
	{ +PLAN_TEXTURE_COORD, -PLAN_TEXTURE_COORD},
	{ +PLAN_TEXTURE_COORD, +PLAN_TEXTURE_COORD},
	{ -PLAN_TEXTURE_COORD, +PLAN_TEXTURE_COORD}
};

GLfloat cta[4][2] = {
	{ -PLAN_TEXTURE_HELICOPTER, -PLAN_TEXTURE_HELICOPTER},
	{ +PLAN_TEXTURE_HELICOPTER, -PLAN_TEXTURE_HELICOPTER},
	{ +PLAN_TEXTURE_HELICOPTER, +PLAN_TEXTURE_HELICOPTER},
	{ -PLAN_TEXTURE_HELICOPTER, +PLAN_TEXTURE_HELICOPTER}
};


void reshape(int width, int height) {
	WIDTH = width;
	HEIGHT = height;
	glViewport(0, 0, (GLint)width, (GLint)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.0, width / (float)height, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void composes_helicopter(void) {
	GLUquadricObj * quadric;

	GLfloat helice[][4] = {
		{0.0, 0.0, 0.0},
		{5.5, 0.0, 0.0},
		{5.5, 0.0, 0.5},
		{0.0, 0.0, 0.5}
	};

	GLfloat helicePequena[][4] = {
		{0.0, 0.0, 0.0},
		{0.0, 0.9, 0.0},
		{0.0, 0.9, 0.1},
		{0.0, 0.0, 0.1}
	};

	/* composition of helicopter */
	helicopter = glGenLists(1);
	glNewList(helicopter, GL_COMPILE);

	glPushMatrix();

	glTranslatef(0, verticalMovement, horizontalMovement);
	glRotatef(-girar, 0.0 , 1.0, 0.0);

	glScalef(1.0, 1.0, 1.0);
	/* draws helicopter's beak */
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glTranslatef(0, 0, 5.0);
	glScalef(0.8, 1.0, 1.0);
	gluSphere(quadric, 0.7, 24, 24);
	glPopMatrix();

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glTranslatef(0, 0, 4.2);
	glScalef(0.8, 1.0, 1.0);
	gluCylinder(quadric, 1, 0.7, 1.0, 12, 3);
	glPopMatrix();

	/* draws cabin */
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glColor3f(0.0, 0.0, 1.0);
	glDisable(GL_TEXTURE_2D);
	glTranslatef(0, 0.5, 3.5);
	glScalef(0.8, 1.0, 1.0);
	gluSphere(quadric, 1.0, 24, 24);
	glPopMatrix();
	if(enableTexture)
		glEnable(GL_TEXTURE_2D);

	/* draws helicopter's body */
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glTranslatef(0, 0.3, 1);
	glScalef(0.7, 1.0, 1.0);
	gluCylinder(quadric, 1.3, 1.3, 2.7, 12, 3);
	glPopMatrix();

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glTranslatef(0, 0, 1.2);
	glScalef(0.8, 1.0, 1.0);
	gluCylinder(quadric, 1, 1, 3, 12, 3);
	glPopMatrix();

	/* draws helicopter's tail */
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glTranslatef(0, -0.5, 0.6);
	glScalef(0.8, 1.0, 1.0);
	glRotatef(-15, 1.0, 0, 0);
	glTranslatef(0, 0.8, 0);
	gluCylinder(quadric, 0.6, 1.1, 1.0, 12, 3);
	glPopMatrix();

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glTranslatef(0, -0.4, 0.1);
	glScalef(0.8, 1.0, 1.0);
	glRotatef(15, 1.0, 0, 0);
	glTranslatef(0, 0.8, 0);
	gluCylinder(quadric, 0.6, 1.1, 1.0, 12, 3);
	glPopMatrix();

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glTranslatef(0, 0.3, 0.5);
	glScalef(0.8, 1.0, 1.0);
	gluSphere(quadric, 0.6, 24, 24);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.3, -0.9);

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glRotatef(10, 1.0, 0, 0);
	glTranslatef(0, 0.3, -1.6);
	glScalef(0.8, 1.0, 1.0);
	gluCylinder(quadric, 0.2, 0.6, 2.8, 12, 3);
	glPopMatrix();

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glTranslatef(0, 1.1, -2);
	glRotatef(45, 1.0, 0, 0);
	glScalef(0.8, 1.0, 1.0);
	gluCylinder(quadric, 0.1, 0.2, 0.8, 12, 3);
	glPopMatrix();
	glPopMatrix();

	/* draws helicopter's machine-gun */
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glTranslatef(0.3, -0.5, 4.6);
	gluCylinder(quadric, 0.2, 0.1, 1.0, 12, 3);
	glPopMatrix();

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glTranslatef(-0.3, -0.5, 4.6);
	gluCylinder(quadric, 0.2, 0.1, 1.0, 12, 3);
	glPopMatrix();


	/* draws machine-gun's bullet */
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glDisable(GL_TEXTURE_2D);
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(0.3, -0.5, machineGunBulletMovement);
	gluSphere(quadric, 0.1, 24, 24);
	glPopMatrix();

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glDisable(GL_TEXTURE_2D);
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(-0.3, -0.5, machineGunBulletMovement);
	gluSphere(quadric, 0.1, 24, 24);
	glPopMatrix();


	/* draws helicopter's torpedo */
	if(enableTexture)
		glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslatef(0, 0, rightTorpedoMovement);
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glTranslatef(1.2, 0.3, 1.8);
	gluCylinder(quadric, 0.3, 0.3, 2.0, 12, 3);
	glPopMatrix();

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glDisable(GL_TEXTURE_2D);
	glTranslatef(1.2, 0.3, 3.8);

	glColor3f(1.0, 0.0, 0.0);
	gluCylinder(quadric, 0.3, 0, 0.4, 12, 3);
	glPopMatrix();

	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, leftTorpedoMovement);

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glTranslatef(-1.1, 0.3, 1.8);
	if(enableTexture)
		glEnable(GL_TEXTURE_2D);
	gluCylinder(quadric, 0.3, 0.3, 2.0, 12, 3);
	glPopMatrix();

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glTranslatef(-1.1, 0.3, 3.8);
	glDisable(GL_TEXTURE_2D);
	gluCylinder(quadric, 0.3, 0, 0.4, 12, 3);
	glPopMatrix();

	glPopMatrix();

	/* draws helicopter's feet */
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	if(enableTexture)
		glEnable(GL_TEXTURE_2D);
	glTranslatef(0.3, -1.3, 1.2);
	gluCylinder(quadric, 0.1, 0.1, 2.8, 12, 3);
	glPopMatrix();

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glRotatef(90, 1.0, 0, 0);
	glTranslatef(0.3, 1.4, 0.6);
	gluCylinder(quadric, 0.1, 0.1, 0.7, 12, 3);
	glPopMatrix();

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glRotatef(90, 1.0, 0, 0);
	glTranslatef(0.3, 3.8, 0.6);
	gluCylinder(quadric, 0.1, 0.1, 0.7, 12, 3);
	glPopMatrix();

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glTranslatef(-0.4, -1.3, 1.2);
	gluCylinder(quadric, 0.1, 0.1, 2.8, 12, 3);
	glPopMatrix();

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glRotatef(90, 1.0, 0, 0);
	glTranslatef(-0.4, 3.8, 0.6);
	gluCylinder(quadric, 0.1, 0.1, 0.7, 12, 3);
	glPopMatrix();

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glRotatef(90, 1.0, 0, 0);
	glTranslatef(-0.4, 1.4, 0.6);
	gluCylinder(quadric, 0.1, 0.1, 0.7, 12, 3);
	glPopMatrix();

	/* composition of helicopter's main screw-propeller */
	glPushMatrix();

	glTranslatef(0, 2.2, 2.5);
	glRotatef(screwPropellerAngle, 0, 1.0, 0);
	glTranslatef(0, -2.2, -2.5);

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glTranslatef(0, 2.2, 2.5);
	glColor3f(0.0, 0.0, 0.0);
	glDisable(GL_TEXTURE_2D);
	glRotatef(90, 1.0, 0, 0);
	gluCylinder(quadric, 0.05, 0.05, 1.0, 12, 3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.8, 2.1, 2.25);
	glColor3f(0.0, 0.0, 0.0);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_POLYGON);
	glTexCoord2fv(cta[0]);
	glVertex3fv(helice[0]);
	glTexCoord2fv(cta[1]);
	glVertex3fv(helice[1]);
	glTexCoord2fv(cta[2]);
	glVertex3fv(helice[2]);
	glTexCoord2fv(cta[3]);
	glVertex3fv(helice[3]);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 0, 1.0, 0);
	glTranslatef(-5.2, 2.1, -0.2);
	glColor3f(0.0, 0.0, 0.0);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_POLYGON);
	glTexCoord2fv(cta[0]);
	glVertex3fv(helice[0]);
	glTexCoord2fv(cta[1]);
	glVertex3fv(helice[1]);
	glTexCoord2fv(cta[2]);
	glVertex3fv(helice[2]);
	glTexCoord2fv(cta[3]);
	glVertex3fv(helice[3]);
	glEnd();
	glPopMatrix();

	glPopMatrix();

	/* composition of helicopter's small screw-propeller */
	glPushMatrix();
	glTranslatef(0, 1.3, -2.7);
	glRotatef(screwPropellerAngle, 1.0, 0, 0);
	glTranslatef(0, -1.3, 2.7);

	glPushMatrix();
	glTranslatef(0.1, 0.5, -1.0);

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glTranslatef(0, 0.8, -1.7);
	glRotatef(90, 0, 1.0, 0);
	gluCylinder(quadric, 0.04, 0.04, 0.2, 12, 3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.1, 0.35, -1.8);
	glBegin(GL_POLYGON);
	glTexCoord2fv(cta[0]);
	glVertex3fv(helicePequena[0]);
	glTexCoord2fv(cta[1]);
	glVertex3fv(helicePequena[1]);
	glTexCoord2fv(cta[2]);
	glVertex3fv(helicePequena[2]);
	glTexCoord2fv(cta[3]);
	glVertex3fv(helicePequena[3]);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.1, 0.85, -2.15);
	glRotatef(90, 1.0, 0, 0);
	glBegin(GL_POLYGON);
	glTexCoord2fv(cta[0]);
	glVertex3fv(helicePequena[0]);
	glTexCoord2fv(cta[1]);
	glVertex3fv(helicePequena[1]);
	glTexCoord2fv(cta[2]);
	glVertex3fv(helicePequena[2]);
	glTexCoord2fv(cta[3]);
	glVertex3fv(helicePequena[3]);
	glEnd();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	glEndList();
}

void display(void) {
	glEnable(GL_DEPTH_TEST);
	glDeleteLists(helicopter, 1);
	composes_helicopter();

	glDepthMask(GL_TRUE);
	glClearColor(0.0, 0.3, 0.8, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	/* observer position */
	obs[0] = radiusxz * cos(2 * PI * axisxz / 360);
	obs[2] = radiusxz * sin(2 * PI * axisxz / 360);
	gluLookAt(obs[0], obs[1], obs[2], look[0], look[1], look[2], 0.0, 1.0, 0.0);

	/* enable/disable use of textures */
	if(enableTexture) {
		glEnable(GL_TEXTURE_2D);
	} else {
		glDisable(GL_TEXTURE_2D);
	}

	glColor4f(PLAN_COLOR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, plan_texture);

	glBegin(GL_QUADS);
	glTexCoord2fv(ctp[0]);
	glVertex3f(-30, 0, 30);
	glTexCoord2fv(ctp[1]);
	glVertex3f(30, 0, 30);
	glTexCoord2fv(ctp[2]);
	glVertex3f(30, 0, -30);
	glTexCoord2fv(ctp[3]);
	glVertex3f(-30, 0, -30);
	glEnd();
	glTranslatef(0.0, 2.0, -3.0);

	glColor4f(HELICOPTER_COLOR);
	glBindTexture(GL_TEXTURE_2D, helicopter_texture);
	glCallList(helicopter);

	glPopMatrix();
	glutSwapBuffers();
}


void special(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		if(turn == 1) {
			verticalMovement = verticalMovement + 0.5;
		}
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
		if(turn == 1) {
			verticalMovement = verticalMovement - 0.5;
			if(verticalMovement < -0.55) {
				verticalMovement = -0.55;
			}
		}
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT:
		if (turn == 1) {
			horizontalMovement = horizontalMovement + 0.5;
		}
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		if (turn == 1) {
			horizontalMovement = horizontalMovement - 0.5;
		}
		glutPostRedisplay();
		break;
	}
}
void mouseAction(int button, int state, int x, int y) {
	// Wheel reports as button 3(scroll up) and button 4(scroll down)
	if (button == 3) {
		radiusxz = radiusxz - 1;
		if(radiusxz == 0) {
			radiusxz = 1;
		}
		glutPostRedisplay();
	}
	if(button == 4) {
		radiusxz = radiusxz + 1;
		glutPostRedisplay();
	}
}
void mouseWeel (int button, int dir, int x, int y) {
	if (dir > 0) { //scroll up
		radiusxz = radiusxz - 1;
		if(radiusxz == 0) {
			radiusxz = 1;
		}
		glutPostRedisplay();
	}
	if(dir < 0) { //scroll down
		radiusxz = radiusxz + 1;
		glutPostRedisplay();
	}
}
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27:  //exit
		exit(0);
		break;

	case 'i': //turn on screw-propellers
		turn = 1;
		glutPostRedisplay();
		break;
	case 'I': //turn off screw-propellers
		stop = 1;
		break;

	case 'm': //shoot
	case 'M':
		shoot = 1;
		glutPostRedisplay();
		break;

	case 't': //launch left torpedo
		leftTorpedo = 1;
		glutPostRedisplay();
		break;
	case 'T': //launch right torpedo
		rightTorpedo = 1;
		glutPostRedisplay();
		break;

		/* moves the visualization */
	case 'w':
	case 'W':
		obs[1] = obs[1] + 1;
		glutPostRedisplay();
		break;
	case 'a':
	case 'A':
		axisxz = axisxz + 2;
		glutPostRedisplay();
		break;
	case 's':
	case 'S':
		obs[1] = obs[1] - 1;
		glutPostRedisplay();
		break;
	case 'd':
	case 'D':
		axisxz = axisxz - 2;
		glutPostRedisplay();
		break;

		/* zoom control */
	case 'r':
		radiusxz = radiusxz - 1;
		if(radiusxz == 0) {
			radiusxz = 1;
		}
		glutPostRedisplay();
		break;
	case 'R':
		radiusxz = radiusxz + 1;
		glutPostRedisplay();
		break;
		/*
				//Enable textures
			case 'e':
				enableTexture = 1;
				glutPostRedisplay();
				break;
				//Disable textures
			case 'E':
				enableTexture = 0;
				glutPostRedisplay();
				break;  */
		// rotate helicopter
	case 'g':
		if(turn == 1) {
			girar = girar + 5.5;
		}

		glutPostRedisplay();
		break;
	case 'G':
		if(turn == 1) {
			girar = girar - 5.5;
		}
		glutPostRedisplay();
		break;

	}
}

void load_textures(void) {
	IMAGE * img;
	GLenum gluerr;

	/* plan texture */
	glGenTextures(1, & plan_texture);
	glBindTexture(GL_TEXTURE_2D, plan_texture);

	if(!(img = ImageLoad(PLAN_TEXTURE))) {
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

	/* helicopter texture */
	glGenTextures(1, & helicopter_texture);
	glBindTexture(GL_TEXTURE_2D, helicopter_texture);


	if(!(img = ImageLoad(HELICOPTER_TEXTURE))) {
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

void init() {
	load_textures();
	composes_helicopter();
	glShadeModel(GL_FLAT);

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { 10.0, 10.0, 10.0, 10.0 };
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if(enableTexture)
		glEnable(GL_TEXTURE_2D);
}

/* function which controls the screw-propeller, machine-gun and torpedo moves */
void animation() {
	if (turn) {
		screwPropellerAngle = (screwPropellerAngle + 15) % 360;
		glutPostRedisplay();
	}

	if (shoot) {
		machineGunBulletMovement = machineGunBulletMovement + 0.3;
		if (machineGunBulletMovement > 15.0) {
			shoot = 0;
			machineGunBulletMovement = 2.0;
		}
		glutPostRedisplay();
	}

	if (leftTorpedo) {
		leftTorpedoMovement = leftTorpedoMovement + 0.3;
		if(leftTorpedoMovement > 15.0) {
			leftTorpedoMovement = 2.0;
			leftTorpedo = 1;
		}
		glutPostRedisplay();
	}

	if (rightTorpedo) {
		rightTorpedoMovement = rightTorpedoMovement + 0.5;
		if(rightTorpedoMovement > 15.0) {
			rightTorpedo = 1;
		}
		glutPostRedisplay();
	}
	if(stop) {
		verticalMovement = verticalMovement - 0.2;
		if (verticalMovement < -0.5) {
			verticalMovement = -0.52;
			stop = 0;
			turn = 0;
		}
		glutPostRedisplay();
	}
}

int main(int argc, char * * argv) {
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInit( & argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

	if(!glutCreateWindow("Helicoptero")) {
		fprintf(stderr, "Error opening a window.\n");
		exit(-1);
	}

	init();

	glutKeyboardFunc(keyboard);
	glutMotionFunc(mouseAction);
	//glutMouseWheelFunc(mouseWeel);
	glutSpecialFunc(special);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(animation);
	glutMainLoop();

	return(0);
}
