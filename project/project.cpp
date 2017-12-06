#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "math.h"
#include <algorithm> 
#include "board.h"	//include board class

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

//**************

#include <vector>
#include <string>
#include <fstream>
#include<cstdio>




//*********

//all variable initialization

float camPos[] = {10, 15, 10};
float pl1Cam[] = {-0.5,20,10};
float pl2Cam[] = {-0.5,20,-10};

int camTrack = 0;

int kingObj;
int pawnObj;
int bishopObj;
int queenObj;
int rookObj;
int knightObj;



//********************************************************************
struct coordinate{ //for vertices
	float x,y,z;
	coordinate(float a,float b,float c) : x(a),y(b),z(c) {};
};

struct face{ //face can be made from quad or triangle
	int faceN;
	bool four;
	int faces[4];
    face(int facen, int f1, int f2, int f3) : faceN(facen){ //constructor for triangle
        faces[0]=f1;
		faces[1]=f2;
		faces[2]=f3;
		four=false;
	}
	face(int facen, int f1,int f2,int f3,int f4) : faceN(facen){ //constructor for quads
		faces[0]=f1;
		faces[1]=f2;
		faces[2]=f3;
		faces[3]=f4;
		four=true;
	}
};

int loadObject(const char* filename)
{
	std::vector<std::string*> coord;//every single line of the obj file as a string
	std::vector<coordinate*> vertex;
	std::vector<face*> faces;
    std::vector<coordinate*> normals; //normals for each vertex
	std::ifstream in(filename);	//open the .obj file
	if(!in.is_open())	//if not opened, exit with -1
	{
		std::cout << "Unable to open file" << std::endl;
		return -1;
	}
	char buf[256];
	//read in every line to coord
	while(!in.eof())
	{
		in.getline(buf,256);
		coord.push_back(new std::string(buf));
	}
	//read all strings and their first character to see if Vertex, face or comment etc.
	for(int i=0;i<coord.size();i++)
	{
		if((*coord[i])[0]=='v' && (*coord[i])[1]==' ')	//if vector & not vn
		{
			float tmpx,tmpy,tmpz;
			sscanf(coord[i]->c_str(),"v %f %f %f",&tmpx,&tmpy,&tmpz);	//store to 3 variables tmpx,tmpy,tmpz
			vertex.push_back(new coordinate(tmpx,tmpy,tmpz));	// push to vertex vector
		}else if((*coord[i])[0]=='v' && (*coord[i])[1]=='n')	//if vn(vertex normal)
        {
            float tmpx,tmpy,tmpz;	//for each vertex coord
			sscanf(coord[i]->c_str(),"vn %f %f %f",&tmpx,&tmpy,&tmpz);
            normals.push_back(new coordinate(tmpx,tmpy,tmpz));	
        }
        else if((*coord[i])[0]=='f')	//if face
		{
			int normal,a,b,c,d;
			if(count(coord[i]->begin(),coord[i]->end(),' ')==3)	//if it is a triangle
			{
		  sscanf(coord[i]->c_str(),"f %d//%d %d//%d %d//%d",&a,&normal,&b,&normal,&c,&normal);
				faces.push_back(new face(normal, a,b,c));	//push to face vector
			}else{
				sscanf(coord[i]->c_str(),"f %d//%d %d//%d %d//%d %d//%d",&a,&normal,&b,&normal,&c,&normal,&d,&normal); //quads
				faces.push_back(new face(normal, a,b,c,d));	//push to face vector
			}
		}
	}
	//draw
	int num;//the id for the list
	num=glGenLists(1);
	glNewList(num,GL_COMPILE);
	for(int i=0;i<faces.size();i++)	
	{
        if (faces[i]->four) //quad
        {
			glBegin(GL_QUADS);
            //face number is linekd to index of each normal
				glNormal3f(normals[faces[i]->faceN-1]->x,normals[faces[i]->faceN-1]->y,normals[faces[i]->faceN-1]->z);
                //draw vertices to draw each face
				glVertex3f(vertex[faces[i]->faces[0]-1]->x,vertex[faces[i]->faces[0]-1]->y,vertex[faces[i]->faces[0]-1]->z);
				glVertex3f(vertex[faces[i]->faces[1]-1]->x,vertex[faces[i]->faces[1]-1]->y,vertex[faces[i]->faces[1]-1]->z);
				glVertex3f(vertex[faces[i]->faces[2]-1]->x,vertex[faces[i]->faces[2]-1]->y,vertex[faces[i]->faces[2]-1]->z);
				glVertex3f(vertex[faces[i]->faces[3]-1]->x,vertex[faces[i]->faces[3]-1]->y,vertex[faces[i]->faces[3]-1]->z);
			glEnd();
		}else{
			glBegin(GL_TRIANGLES); //triangle
            //face number is linekd to index of each normal
				glNormal3f(normals[faces[i]->faceN-1]->x,normals[faces[i]->faceN-1]->y,normals[faces[i]->faceN-1]->z);
                //draw vertices to draw each face
				glVertex3f(vertex[faces[i]->faces[0]-1]->x,vertex[faces[i]->faces[0]-1]->y,vertex[faces[i]->faces[0]-1]->z);
				glVertex3f(vertex[faces[i]->faces[1]-1]->x,vertex[faces[i]->faces[1]-1]->y,vertex[faces[i]->faces[1]-1]->z);
				glVertex3f(vertex[faces[i]->faces[2]-1]->x,vertex[faces[i]->faces[2]-1]->y,vertex[faces[i]->faces[2]-1]->z);
			glEnd();
		}
	}
	glEndList();
	//delete vectors after use to prevent memory leaks
	for(int i=0;i<coord.size();i++)
		delete coord[i];
	for(int i=0;i<faces.size();i++)
		delete faces[i];
	for(int i=0;i<vertex.size();i++)
		delete vertex[i];
    for(int i=0;i<normals.size();i++)
		delete normals[i];
	return num;	//return with the id
}



//********************************************************************

void keyboard(unsigned char key, int xIn, int yIn)
{
	switch (key)
	{
        //exit functionality
        case 'q':
		case 27:
			exit(0);
            break;

        case 'w':
            if (camTrack == 0){
                camTrack = 1;
            }
            else if (camTrack == 1){
                camTrack = 2;
            }
            else if (camTrack == 2){
                camTrack = 0;
            }
            glutPostRedisplay();
            break;
    }
}

void special(int key, int x, int y){
    //arrow keys ability to control the camera position
    switch(key)
    {
        case GLUT_KEY_UP:
            camPos[1] += 1;
            break;
        case GLUT_KEY_DOWN:
            camPos[1] -= 1;
            break;
        case GLUT_KEY_LEFT:
            camPos[2] +=  1;
            break;
        case GLUT_KEY_RIGHT:
            camPos[2] -= 1;
            break;      
    }
    //call to redisplay in order for the camera position change to take affect
    glutPostRedisplay();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    //create camera viewing transformations
    if (camTrack == 0){
        gluLookAt(camPos[0], camPos[1], camPos[2], 0,0,0, 0,1,0);
    }
    else if (camTrack == 1){
        gluLookAt(pl1Cam[0], pl1Cam[1], pl1Cam[2], -0.5,0,-0.5, 0,1,0);
    }
    else if (camTrack == 2){
        gluLookAt(pl2Cam[0], pl2Cam[1], pl2Cam[2], -0.5,0,-0.5, 0,1,0);
    }

    //board
    glPushMatrix(); //push board
    Board b;
    b.drawBoard();

    glPushMatrix(); //king
    glColor3f(0.7, 0.1, 0.2);
    glScalef(2, 2, 2);
    glTranslatef(1, 2, 1);
    glCallList(kingObj);	//draw the 3D mesh
    glPopMatrix();

	glPushMatrix(); //queen
    glColor3f(0.5, 0.5, 0.5);
    glScalef(2, 2, 2);
    glTranslatef(1, 3, 1);
    glCallList(queenObj);	//draw the 3D mesh
    glPopMatrix();

	glPushMatrix(); //knight
    glColor3f(0.9, 0.1, 0.5);
    glScalef(0.5, 1, 0.5);
    glTranslatef(2, 0.5, 3);
    glCallList(knightObj);	//draw the 3D mesh
    glPopMatrix();


	glPushMatrix(); //bishop
    glColor3f(0.2, 0.8, 0.3);
    glScalef(2, 2, 2);
    glTranslatef(1, 2, 2);
    glCallList(bishopObj);	//draw the 3D mesh
    glPopMatrix();


	glPushMatrix(); //rook
    glColor3f(0.7, 0.8, 0.1);
    glScalef(2, 2, 2);
    glTranslatef(2, 3, 3);
    glCallList(rookObj);	//draw the 3D mesh
    glPopMatrix();


    glPushMatrix(); //PAWN
    glColor3f(0.1, 0.3, 0.7);
    glScalef(2, 2, 2);
    glTranslatef(2, 2, 1);
    glCallList(pawnObj);	//draw the 3D mesh
    glPopMatrix();

    glPushMatrix(); //PAWN
    glColor3f(0.1, 0.3, 0.7);
    glScalef(2, 2, 2);
    glTranslatef(2, 3, 1);
    glCallList(pawnObj);	//draw the 3D mesh
    glPopMatrix();

    glPushMatrix(); //pieces
    glColor3f(0.1, 0.3, 0.7);
    glScalef(2, 2, 2);
    glTranslatef(2.5, 2, 1);
    glCallList(pawnObj);	//draw the 3D mesh
    glPopMatrix();

    glPopMatrix();//pop board

    glutSwapBuffers();
}


void myInit(void)
{
	glClearColor(0.8, 0.8, 1, 1);
    glColor3f(1, 1, 1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 1, 400);

    kingObj=loadObject("king.obj");	//load the king.obj file
    pawnObj=loadObject("pawn.obj");	//load the pawn.obj file
	bishopObj=loadObject("bishop.obj");	//load the king.obj file
    queenObj=loadObject("queen.obj");	//load the pawn.obj file
	rookObj=loadObject("rook.obj");	//load the pawn.obj file
	knightObj=loadObject("knightNoTexture.obj");	//load the pawn.obj file

    //backface culling
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);		//starts up GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 750);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Chess Bois");	//creates the window
    //below is print out to terminal with all info needed to use program

    glutDisplayFunc(display); //registers "display" as the display callback function
    glutKeyboardFunc(keyboard);	
    glutSpecialFunc(special);

    glEnable(GL_DEPTH_TEST);

    myInit();
    glutMainLoop();	//starts the event loop

	return(0);					//return may not be necessary on all compilers
}