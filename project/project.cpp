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

//all variable initialization

float camPos[] = {10, 15, 10};
float pl1Cam[] = {-0.5,20,10};
float pl2Cam[] = {-0.5,20,-10};

int camTrack = 0;

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
    Board b;
    b.drawBoard();

    glutSwapBuffers();
}

void myInit (void)
{
	glClearColor(0.8, 0.8, 1, 1);
    glColor3f(1, 1, 1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 1, 400);

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