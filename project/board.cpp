//board.cpp

#include "board.h"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#endif

Board::Board(){	//constructor
}

void Board::drawBoard(){
	glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(1,0,0);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(2,0,0);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(-1,0,0);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(-2,0,0);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(-3,0,0);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(-4,0,0);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(3,0,0);
    glutSolidCube(1);
glPopMatrix();



glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(0,0,1);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(1,0,1);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(2,0,1);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(-1,0,1);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(-2,0,1);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(-3,0,1);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(-4,0,1);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(3,0,1);
    glutSolidCube(1);
glPopMatrix();



glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(0,0,2);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(1,0,2);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(2,0,2);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(-1,0,2);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(-2,0,2);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(-3,0,2);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(-4,0,2);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(3,0,2);
    glutSolidCube(1);
glPopMatrix();



glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(0,0,3);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(1,0,3);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(2,0,3);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(-1,0,3);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(-2,0,3);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(-3,0,3);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(-4,0,3);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(3,0,3);
    glutSolidCube(1);
glPopMatrix();



glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(0,0,-1);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(1,0,-1);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(2,0,-1);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(-1,0,-1);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(-2,0,-1);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(-3,0,-1);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(-4,0,-1);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(3,0,-1);
    glutSolidCube(1);
glPopMatrix();



glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(0,0,-2);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(1,0,-2);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(2,0,-2);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(-1,0,-2);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(-2,0,-2);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(-3,0,-2);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(-4,0,-2);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(3,0,-2);
    glutSolidCube(1);
glPopMatrix();



glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(0,0,-3);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(1,0,-3);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(2,0,-3);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(-1,0,-3);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(-2,0,-3);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(-3,0,-3);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(-4,0,-3);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(3,0,-3);
    glutSolidCube(1);
glPopMatrix();



glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(0,0,-4);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(1,0,-4);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(2,0,-4);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(-1,0,-4);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(-2,0,-4);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(-3,0,-4);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(-4,0,-4);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(3,0,-4);
    glutSolidCube(1);
glPopMatrix();
}