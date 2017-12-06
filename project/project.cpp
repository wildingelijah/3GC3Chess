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
float pl1Cam[] = {-3.5,20,10};
float pl2Cam[] = {-3.5,20,-10};

int camTrack = 0;

int xmouse = 0;
int ymouse = 0;

float bOff = 1;
int selected = 0;
int select = 0;
bool intersect = false;

int kingObj;
int pawnObj;
int bishopObj;
int queenObj;
int rookObj;
int knightObj;

int board[8][8];

Square* squares = new Square[64];

// const int start[8][8] = {rookObj, knightObj, bishopObj, queenObj, kingObj, bishopObj, knightObj, rookObj, pawnObj, 
//                         pawnObj, pawnObj, pawnObj, pawnObj, pawnObj, pawnObj, pawnObj, 0, 0, 0, 0, 0, 0, 0, 0, 
//                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -pawnObj,
//                         -pawnObj, -pawnObj, -pawnObj, -pawnObj, -pawnObj, -pawnObj, -pawnObj, -rookObj, -knightObj, 
//                         -bishopObj, -queenObj, -kingObj, -bishopObj, -knightObj, -rookObj};

//********************************************************************

// void setupBoard(void){
//     for (int i = 0; i < 8; i++){
//         for (int j = 0; j < 8; j++){
//             board[i][j] = start[i][j];
//         }
//     }
// }

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

//valid intersection calculations for our rays
void interCalc(){

      GLdouble matlOfModel[16], matlOfProj[16], ray0[3], ray1[3], rayD[3];
      GLint matlView[4];

      glGetDoublev(GL_MODELVIEW_MATRIX, matlOfModel);
      glGetDoublev(GL_PROJECTION_MATRIX, matlOfProj);
      glGetIntegerv(GL_VIEWPORT, matlView);

      gluUnProject(xmouse, ymouse, 0.0, matlOfModel, matlOfProj, matlView, &ray0[0], &ray0[1], &ray0[2]);

      gluUnProject(xmouse, ymouse, 1.0, matlOfModel, matlOfProj, matlView, &ray1[0], &ray1[1], &ray1[2]);

      rayD[0] = ray1[0] - ray0[0];
      rayD[1] = ray1[1] - ray0[1];
      rayD[2] = ray1[2] - ray0[2];

      GLdouble m = sqrt(rayD[0]*rayD[0] + rayD[1]*rayD[1] + rayD[2]*rayD[2]);
      rayD[0] /= m;
      rayD[1] /= m;
      rayD[2] /= m;

      //calculatng the intersection point
      for (int i = 0; i < 100; i++){
            double pt[3];
            double temp = (squares[i].getZ() - ray0[2]);
            double t = temp/rayD[2];
            pt[0] = ray0[0] + t * rayD[0];
            pt[1] = ray0[1] + t * rayD[1];
            pt[2] = squares[i].getZ();

            //detect if point clicked is inside our wireframe
            if (pt[1] > 0.3 - bOff && pt[2] < squares[i].getZ() + bOff && pt[0] > squares[i].getX() - bOff && 
            pt[0] < squares[i].getX() + bOff && pt[1] < 0.3 + bOff && pt[2] > squares[i].getZ() - bOff){
                  select = i;
                  bOff = 1;
                  intersect = true;
            } 
            else {
                  intersect = false;
            }
      }
}

//drawing wireframe for object selected
void selectedObj(int type){
      glPushMatrix();
      float x = squares[type].getX();
      float y = 0.3;
      float z = squares[type].getZ();
      glColor3f(1, 0, 0);
      glTranslatef(x, y, z);

      //wireframe scaling
      float wire = 0.5;

      glBegin(GL_LINES);
            glVertex3f(-wire, -wire, -wire);
            glVertex3f(-wire, wire, -wire);
            glVertex3f(-wire, -wire, -wire);
            glVertex3f(-wire, -wire, wire);
            glVertex3f(-wire, -wire, -wire);
            glVertex3f(wire, -wire, -wire);
            
            glVertex3f(wire, wire, wire);
            glVertex3f(wire, wire, -wire);
            glVertex3f(wire, wire, wire);
            glVertex3f(-wire, wire, wire);
            glVertex3f(wire, wire, wire);
            glVertex3f(wire, -wire, wire);

            glVertex3f(-wire, -wire, wire);
            glVertex3f(-wire, wire, wire);
            glVertex3f(-wire, -wire, wire);
            glVertex3f(wire, -wire, wire);
            glVertex3f(-wire, wire, wire);
            glVertex3f(-wire, wire, -wire);

            glVertex3f(-wire, wire, -wire);
            glVertex3f(wire, wire, -wire);
            glVertex3f(wire, wire, -wire);
            glVertex3f(wire, -wire, -wire);
            glVertex3f(wire, -wire, -wire);
            glVertex3f(wire, -wire, wire);           
      glEnd();
      glPopMatrix();

}

//handles mouse interaction and passes values to ray calculations
void mouse(int btn, int state, int x, int y){
      
      xmouse = x;
      ymouse = 800 - y;

      if (btn == GLUT_LEFT_BUTTON && state == GLUT_UP){
            selected = select;
      } 
      interCalc();
}

void FPS(int val){
      glutPostRedisplay();
      glutTimerFunc(17, FPS, 0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    float lightPos[] = {-1.0,1.0, -2.0, 1.0 };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    //create camera viewing transformations
    if (camTrack == 0){
        gluLookAt(camPos[0], camPos[1], camPos[2], -3.5,0,-3.5, 0,1,0);
    }
    else if (camTrack == 1){
        gluLookAt(pl1Cam[0], pl1Cam[1], pl1Cam[2], -3.5,0,-3.5, 0,1,0);
    }
    else if (camTrack == 2){
        gluLookAt(pl2Cam[0], pl2Cam[1], pl2Cam[2], -3.5,0,-3.5, 0,1,0);
    }

    //board
    squares[0] = Square(0,0,rookObj,0,0,0);
	squares[1] = Square(-1,0,knightObj,0,1,0);
	squares[2] = Square(-2,0,bishopObj,0,0,0);
	squares[3] = Square(-3,0,queenObj,0,1,0);
	squares[4] = Square(-4,0,kingObj,0,0,0);
	squares[5] = Square(-5,0,bishopObj,0,1,0);
	squares[6] = Square(-6,0,knightObj,0,0,0);
	squares[7] = Square(-7,0,rookObj,0,1,0);

	squares[8] = Square(0,-1,pawnObj,0,1,0);
	squares[9] = Square(-1,-1,pawnObj,0,0,0);
	squares[10] = Square(-2,-1,pawnObj,0,1,0);
	squares[11] = Square(-3,-1,pawnObj,0,0,0);
	squares[12] = Square(-4,-1,pawnObj,0,1,0);
	squares[13] = Square(-5,-1,pawnObj,0,0,0);
	squares[14] = Square(-6,-1,pawnObj,0,1,0);
	squares[15] = Square(-7,-1,pawnObj,0,0,0);

	squares[16] = Square(0,-2,0,0,0,0);
	squares[17] = Square(-1,-2,0,0,1,0);
	squares[18] = Square(-2,-2,0,0,0,0);
	squares[19] = Square(-3,-2,0,0,1,0);
	squares[20] = Square(-4,-2,0,0,0,0);
	squares[21] = Square(-5,-2,0,0,1,0);
	squares[22] = Square(-6,-2,0,0,0,0);
	squares[23] = Square(-7,-2,0,0,1,0);

	squares[24] = Square(0,-3,0,0,1,0);
	squares[25] = Square(-1,-3,0,0,0,0);
	squares[26] = Square(-2,-3,0,0,1,0);
	squares[27] = Square(-3,-3,0,0,0,0);
	squares[28] = Square(-4,-3,0,0,1,0);
	squares[29] = Square(-5,-3,0,0,0,0);
	squares[30] = Square(-6,-3,0,0,1,0);
	squares[31] = Square(-7,-3,0,0,0,0);

	squares[32] = Square(0,-4,0,0,0,0);
	squares[33] = Square(-1,-4,0,0,1,0);
	squares[34] = Square(-2,-4,0,0,0,0);
	squares[35] = Square(-3,-4,0,0,1,0);
	squares[36] = Square(-4,-4,0,0,0,0);
	squares[37] = Square(-5,-4,0,0,1,0);
	squares[38] = Square(-6,-4,0,0,0,0);
	squares[39] = Square(-7,-4,0,0,1,0);

	squares[40] = Square(0,-5,0,0,1,0);
	squares[41] = Square(-1,-5,0,0,0,0);
	squares[42] = Square(-2,-5,0,0,1,0);
	squares[43] = Square(-3,-5,0,0,0,0);
	squares[44] = Square(-4,-5,0,0,1,0);
	squares[45] = Square(-5,-5,0,0,0,0);
	squares[46] = Square(-6,-5,0,0,1,0);
	squares[47] = Square(-7,-5,0,0,0,0);

	squares[48] = Square(0,-6,pawnObj,1,0,0);
	squares[49] = Square(-1,-6,pawnObj,1,1,0);
	squares[50] = Square(-2,-6,pawnObj,1,0,0);
	squares[51] = Square(-3,-6,pawnObj,1,1,0);
	squares[52] = Square(-4,-6,pawnObj,1,0,0);
	squares[53] = Square(-5,-6,pawnObj,1,1,0);
	squares[54] = Square(-6,-6,pawnObj,1,0,0);
	squares[55] = Square(-7,-6,pawnObj,1,1,0);

	squares[56] = Square(0,-7,rookObj,1,1,0);
	squares[57] = Square(-1,-7,knightObj,1,0,0);
	squares[58] = Square(-2,-7,bishopObj,1,1,0);
	squares[59] = Square(-3,-7,queenObj,1,0,0);
	squares[60] = Square(-4,-7,kingObj,1,1,0);
	squares[61] = Square(-5,-7,bishopObj,1,0,0);
	squares[62] = Square(-6,-7,knightObj,1,1,0);
	squares[63] = Square(-7,-7,rookObj,1,0,0);
	
	for (int i = 0; i < 64; i++){
		glPushMatrix();
			glColor3f(squares[i].getColour(),squares[i].getColour(),squares[i].getColour());
    		glScalef(1,0.40,1);
			glTranslatef(squares[i].getX(),0,squares[i].getZ());
    		glutSolidCube(1);
		glPopMatrix();

		if (squares[i].getPiece() != 0){
			glPushMatrix();
				if (squares[i].getTeam() == 0){
					glColor3f(0,0,0);
				}
				else {
					glColor3f(1,1,1);
				}
				glTranslatef(squares[i].getX(),0.3,squares[i].getZ());
				glCallList(squares[i].getPiece());
			glPopMatrix();
		}
	}

	selectedObj(selected);
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
//************lighting*****************
 glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        float lightColour[]={1.0,1.0,1.0,1.0};
        glLightfv(GL_LIGHT0,GL_DIFFUSE,lightColour);

        //********************************************
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
	glutMouseFunc(mouse);
	glutTimerFunc(0, FPS, 0);

    glEnable(GL_DEPTH_TEST);

    myInit();
    glutMainLoop();	//starts the event loop

	return(0);					//return may not be necessary on all compilers
}