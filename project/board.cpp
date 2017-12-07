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

Square::Square(){
}
Square::Square(int x, int z, int piece,int team, int colour, int highlight){
    this->x = x;
    this->z = z;
    this->piece = piece;
    this->team = team;
    this->colour = colour;
    this->highlight = highlight;
}
int Square::getX(){
    return x;
}
int Square::getZ(){
    return z;
}
int Square::getPiece(){
    return piece;
}
int Square::getTeam(){
    return team;
}
int Square::getColour(){
    return colour;
}
int Square::getHighlight(){
    return highlight;
}

void Square::setX(int value){
    x = value;
}
void Square::setZ(int value){
    z = value;
}
void Square::setPiece(int value){
    piece = value;
}
void Square::setTeam(int value){
    team = value;
}
void Square::setColour(int value){
    colour = value;
}
void Square::setHighlight(int value){
    highlight = value;
}


void Board::drawBoard(){
	glPushMatrix();
    glColor3f(1,0,0);
    glScalef(1,0.40,1);
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
    glTranslatef(-5,0,0);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(-6,0,0);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(-7,0,0);
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
    glTranslatef(-5,0,-1);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(-6,0,-1);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(-7,0,-1);
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
    glTranslatef(-5,0,-2);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(-6,0,-2);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(-7,0,-2);
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
    glTranslatef(-5,0,-3);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(-6,0,-3);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(-7,0,-3);
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
    glTranslatef(-5,0,-4);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(-6,0,-4);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(-7,0,-4);
    glutSolidCube(1);
glPopMatrix();



glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(0,0,-5);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(-1,0,-5);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(-2,0,-5);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(-3,0,-5);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(-4,0,-5);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(-5,0,-5);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(-6,0,-5);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(-7,0,-5);
    glutSolidCube(1);
glPopMatrix();



glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(0,0,-6);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(-1,0,-6);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(-2,0,-6);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(-3,0,-6);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(-4,0,-6);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(-5,0,-6);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(-6,0,-6);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(-7,0,-6);
    glutSolidCube(1);
glPopMatrix();



glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(0,0,-7);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(-1,0,-7);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(-2,0,-7);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(-3,0,-7);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(-4,0,-7);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(-5,0,-7);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(0,0,0);
    glScalef(1,0.40,1);
    glTranslatef(-6,0,-7);
    glutSolidCube(1);
glPopMatrix();

glPushMatrix();
    glColor3f(1,1,1);
    glScalef(1,0.40,1);
    glTranslatef(-7,0,-7);
    glutSolidCube(1);
glPopMatrix();
}