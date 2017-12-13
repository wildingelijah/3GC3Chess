#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "math.h"
#include <algorithm>
#include "board.h" //include board class

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#endif

#include <vector>
#include <string>
#include <fstream>
#include <cstdio>

//all variable initialization

float camPos[] = {10, 15, 10};
float pl1Cam[] = {-3.5, 19, 10};
float pl2Cam[] = {-3.5, 19, -17};

bool whiteTeamInCheck = false;
bool blackTeamInCheck = false;

int camTrack = 1;

bool piecemoved = false;
int playerTurn = 1;
int currentPiece = 0;

GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 0.0};
GLfloat light_position[] = {0.0, 1.0, 0.0, 0.0};
GLfloat light_ambient[] = {0.5, 0.5, 0.5, 0.0};
GLfloat light_specular[] = {0, 0, 0, 0.0};

int xmouse = 0;
int ymouse = 0;

float bOff = 1;
int selected = 64;
int selectpiece = 0;
bool intersect = false;

float teapotangle = 0;
float teapot2angle = 0;

int kingObj;
int pawnObj;
int bishopObj;
int queenObj;
int rookObj;
int knightObj;

int board[8][8];

Square *squares = new Square[65];
Square *highlightedSquares = new Square[65];

struct coordinate
{ //for vertices
	float x, y, z;
	coordinate(float a, float b, float c) : x(a), y(b), z(c){};
};

struct face
{ //face can be made from quad or triangle
	int faceN;
	int faces[3];
	face(int facen, int f1, int f2, int f3) : faceN(facen)
	{ //constructor for triangle
		faces[0] = f1;
		faces[1] = f2;
		faces[2] = f3;
	}
};

// based off youtube channels cpluspluguy https://www.youtube.com/channel/UC6A2B9G_y-fzAXEu2hHPlMg
// and Sundeep Saradhi https://www.youtube.com/watch?v=csEv5ZRMvpQ
int loadObject(const char *filename)
{
	std::vector<std::string *> coord; //every single line of the obj file as a string
	std::vector<coordinate *> vertex;
	std::vector<face *> faces;
	std::vector<coordinate *> normals;
	std::ifstream in(filename); //open the .obj file
	if (!in.is_open())			//if not opened, exit with -1
	{
		std::cout << "Unable to open file" << std::endl;
		return -1;
	}
	char buf[256];
	//read in every line to coord
	while (!in.eof())
	{
		in.getline(buf, 256);
		coord.push_back(new std::string(buf));
	}
	//read all strings and their first character to see if Vertex, face or Vertex normal
	for (int i = 0; i < coord.size(); i++)
	{
		if ((*coord[i])[0] == 'v' && (*coord[i])[1] == ' ') //if vector & not vn
		{
			float tmpx, tmpy, tmpz;
			sscanf(coord[i]->c_str(), "v %f %f %f", &tmpx, &tmpy, &tmpz); //store to 3 variables tmpx,tmpy,tmpz
			vertex.push_back(new coordinate(tmpx, tmpy, tmpz));			  // push to vertex vector
		}
		else if ((*coord[i])[0] == 'v' && (*coord[i])[1] == 'n') //if vn
		{
			float tmpx, tmpy, tmpz;
			sscanf(coord[i]->c_str(), "vn %f %f %f", &tmpx, &tmpy, &tmpz); //store to 3 variables tmpx,tmpy,tmpz
			normals.push_back(new coordinate(tmpx, tmpy, tmpz));		   // push to normal vector
		}

		else if ((*coord[i])[0] == 'f') //if face
		{
			int normal, a, b, c, d;
			if (count(coord[i]->begin(), coord[i]->end(), ' ') == 3) //if it is a triangle
			{
				sscanf(coord[i]->c_str(), "f %d//%d %d//%d %d//%d", &a, &normal, &b, &normal, &c, &normal);
				faces.push_back(new face(normal, a, b, c)); //push to face vector
			}
		}
	}
	//draw
	int num; //the id for the list
	num = glGenLists(1);
	glNewList(num, GL_COMPILE);
	for (int i = 0; i < faces.size(); i++)
	{
			glBegin(GL_TRIANGLES); //triangle
			glNormal3f(normals[faces[i]->faceN - 1]->x, normals[faces[i]->faceN - 1]->y, normals[faces[i]->faceN - 1]->z);
			glVertex3f(vertex[faces[i]->faces[0] - 1]->x, vertex[faces[i]->faces[0] - 1]->y, vertex[faces[i]->faces[0] - 1]->z);
			glVertex3f(vertex[faces[i]->faces[1] - 1]->x, vertex[faces[i]->faces[1] - 1]->y, vertex[faces[i]->faces[1] - 1]->z);
			glVertex3f(vertex[faces[i]->faces[2] - 1]->x, vertex[faces[i]->faces[2] - 1]->y, vertex[faces[i]->faces[2] - 1]->z);
			glEnd();
	}
	glEndList();
	//delete everything to avoid memory leaks
	for (int i = 0; i < coord.size(); i++)
		delete coord[i];
	for (int i = 0; i < faces.size(); i++)
		delete faces[i];
	for (int i = 0; i < vertex.size(); i++)
		delete vertex[i];
	for (int i = 0; i < normals.size(); i++)
		delete normals[i];
	return num; //return with the unique int for obj file
}

void blackPawnCheckForCheck(){
	//checks if plack bawn causes check
		if( squares[selectpiece].getX() == 0){
			if (squares[selectpiece+9].getTeam() == 1 
			&& squares[selectpiece+9].getPiece() == kingObj){
				whiteTeamInCheck = true;
			}
		}
		if(squares[selectpiece].getX() == -7){
			if (squares[selectpiece+7].getTeam() == 1
			&& squares[selectpiece+7].getPiece() == kingObj){
				whiteTeamInCheck = true;
			}
		}
		if(squares[selectpiece].getX() != -7 && squares[selectpiece].getX() != 0){
			if (squares[selectpiece+7].getTeam() == 1
			&& squares[selectpiece+7].getPiece() == kingObj){
				whiteTeamInCheck = true;
			}
			if (squares[selectpiece+9].getTeam() == 1 
			&& squares[selectpiece+9].getPiece() == kingObj){
				whiteTeamInCheck = true;
			}
		}
}

void whitePawnCheckForCheck(){
	//checks if white pawn causes check
	for (int i = 0; i < 64; i++){
            highlightedSquares[i].setHighlight(0);
        }
		if( squares[selectpiece].getX() == 0){
			if (squares[selectpiece-7].getTeam() == 2
			&& squares[selectpiece-7].getPiece() == kingObj){
				blackTeamInCheck = true;
			}
		}
		if(squares[selectpiece].getX() == -7){
			if (squares[selectpiece-9].getTeam() == 2
			&& squares[selectpiece-9].getPiece() == kingObj){
				blackTeamInCheck = true;
			}
		}
		if(squares[selectpiece].getX() != -7 && squares[selectpiece].getX() != 0){
			if (squares[selectpiece-7].getTeam() == 2
			&& squares[selectpiece-7].getPiece() == kingObj){
				blackTeamInCheck = true;
			}
			if (squares[selectpiece-9].getTeam() == 2 
			&& squares[selectpiece-9].getPiece() == kingObj){
				blackTeamInCheck = true;
			}
		}
}

void blackRookCheckForCheck(){
	//checks if black rook causes check
	for (int i = selectpiece + 8; i < 64; i += 8)
	{
		if (squares[i].getPiece() == 0)
		{
		}
		else if (squares[i].getPiece() != 0)
		{
			if (squares[i].getTeam() == 1 && squares[i].getPiece() == kingObj)
			{
				whiteTeamInCheck = true;
			}
			break;
		}
	}
	for (int i = selectpiece - 8; i > 0; i -= 8)
	{
		if (squares[i].getPiece() == 0)
		{
		}
		else if (squares[i].getPiece() != 0)
		{
			if (squares[i].getTeam() == 1 && squares[i].getPiece() == kingObj)
			{
				whiteTeamInCheck = true;
			}
			break;
		}
	}

	if (selectpiece != 7 && selectpiece != 15 && selectpiece != 23 && selectpiece != 31 && selectpiece != 39 && selectpiece != 47 && selectpiece != 55 && selectpiece != 63)
	{
		for (int i = selectpiece + 1; i < 64; i++)
		{
			if (squares[i].getPiece() == 0)
			{
			}
			else if (squares[i].getPiece() != 0)
			{
				if (squares[i].getTeam() == 1 && squares[i].getPiece() == kingObj)
				{
					whiteTeamInCheck = true;
				}
				break;
			}
			if (i == 7 || i == 15 || i == 23 || i == 31 || i == 39 || i == 47 || i == 55 || i == 63)
			{
				break;
			}
		}
	}

	if (selectpiece != 0 && selectpiece != 8 && selectpiece != 16 && selectpiece != 24 && selectpiece != 32 && selectpiece != 40 && selectpiece != 48 && selectpiece != 56)
	{
		for (int i = selectpiece - 1; i > 0; i--)
		{
			if (squares[i].getPiece() == 0)
			{
			}
			else if (squares[i].getPiece() != 0)
			{
				if (squares[i].getTeam() == 1 && squares[i].getPiece() == kingObj)
				{
					whiteTeamInCheck = true;
				}
				break;
			}
			if (i == 0 || i == 8 || i == 16 || i == 24 || i == 32 || i == 40 || i == 48 || i == 56)
			{
				break;
			}
		}
	}
}

void whiteRookCheckForCheck(){
	//checks if black rook causes check
	for (int i = selectpiece + 8; i < 64; i += 8){
		if (squares[i].getPiece() != 0){}
		if (squares[i].getTeam() == 0 && squares[i].getPiece() == kingObj)
		{
			blackTeamInCheck = true;
		}
		break;
	}

for (int i = selectpiece - 8; i > 0; i -= 8)
{
	if (squares[i].getPiece() == 0)
	{
		squares[i].setHighlight(1);
		highlightedSquares[i].setHighlight(1);
	}
	else if (squares[i].getPiece() != 0)
	{
		if (squares[i].getTeam() == 0 && squares[i].getPiece() == kingObj)
		{
			blackTeamInCheck = true;
		}
		break;
	}
}

if (selectpiece != 7 && selectpiece != 15 && selectpiece != 23 && selectpiece != 31 && selectpiece != 39 && selectpiece != 47 && selectpiece != 55 && selectpiece != 63)
{
	for (int i = selectpiece + 1; i < 64; i++)
	{
		if (squares[i].getPiece() == 0)
		{
		}
		else if (squares[i].getPiece() != 0)
		{
			if (squares[i].getTeam() == 0 && squares[i].getPiece() == kingObj)
			{
			blackTeamInCheck = true;
			}
			break;
		}
		if (i == 7 || i == 15 || i == 23 || i == 31 || i == 39 || i == 47 || i == 55 || i == 63)
		{
			break;
		}
	}
}

if (selectpiece != 0 && selectpiece != 8 && selectpiece != 16 && selectpiece != 24 && selectpiece != 32 && selectpiece != 40 && selectpiece != 48 && selectpiece != 56)
{
	for (int i = selectpiece - 1; i > 0; i--)
	{
		if (squares[i].getPiece() == 0)
		{
		}
		else if (squares[i].getPiece() != 0)
		{
			if (squares[i].getTeam() == 0 && squares[i].getPiece() == kingObj)
			{
				blackTeamInCheck = true;
			}
			break;
		}
		if (i == 0 || i == 8 || i == 16 || i == 24 || i == 32 || i == 40 || i == 48 || i == 56)
		{
			break;
		}
	}
}
}


void blackQueenCheckForCheck(){
	// checks black queen to see if it causes check
	for (int i = selectpiece + 8; i < 64; i += 8)
	{
		if (squares[i].getPiece() != kingObj && squares[i].getPiece() != 0)
		{
			break;
		}
		else if (squares[i].getPiece() == kingObj && squares[i].getTeam() == 1)
		{
			whiteTeamInCheck = true;
		}
	}
	for (int i = selectpiece - 8; i > 0; i -= 8)
	{
		if (squares[i].getPiece() != 0 && squares[i].getPiece() != kingObj)
		{
			break;
		}
		else if (squares[i].getPiece() == kingObj && squares[i].getTeam() == 1)
		{
			whiteTeamInCheck = true;
		}
	}

	if (selectpiece != 0 && selectpiece != 8 && selectpiece != 16 && selectpiece != 24 && selectpiece != 32 && selectpiece != 40 && selectpiece != 48 && selectpiece != 56)
	{
		for (int i = selectpiece - 1; i > 0; i--)
		{
			if (squares[i].getPiece() != 0 && squares[i].getPiece() != kingObj)
			{
				break;
			}
			else if (squares[i].getPiece() == kingObj && squares[i].getTeam() == 1)
			{
				whiteTeamInCheck = true;
			}
			if (i == 0 || i == 8 || i == 16 || i == 24 || i == 32 || i == 40 || i == 48 || i == 56)
			{
				break;
			}
		}

		for (int i = selectpiece + 7; i < 64; i += 7)
		{
			if (squares[i].getPiece() != 0 && squares[i].getPiece() != kingObj)
			{
				break;
			}
			else if (squares[i].getPiece() == kingObj && squares[i].getTeam() == 1)
			{
				whiteTeamInCheck = true;
			}
			if (i == 0 || i == 8 || i == 16 || i == 24 || i == 32 || i == 40 || i == 48 || i == 56)
			{
				break;
			}
		}

		for (int i = selectpiece - 9; i > 0; i -= 9)
		{
			if (squares[i].getPiece() != 0 && squares[i].getPiece() != kingObj)
			{
				break;
			}
			else if (squares[i].getPiece() == kingObj && squares[i].getTeam() == 1)
			{
				whiteTeamInCheck = true;
			}
			if (i == 0 || i == 8 || i == 16 || i == 24 || i == 32 || i == 40 || i == 48 || i == 56)
			{
				break;
			}
		}
	}

	if (selectpiece != 7 && selectpiece != 15 && selectpiece != 23 && selectpiece != 31 && selectpiece != 39 && selectpiece != 47 && selectpiece != 55 && selectpiece != 63)
	{
		for (int i = selectpiece + 1; i < 64; i++)
		{
			if (squares[i].getPiece() != 0 && squares[i].getPiece() != kingObj)
			{
				break;
			}
			else if (squares[i].getPiece() == kingObj && squares[i].getTeam() == 1)
			{
				whiteTeamInCheck = true;
			}
			if (i == 7 || i == 15 || i == 23 || i == 31 || i == 39 || i == 47 || i == 55 || i == 63)
			{
				break;
			}
		}

		for (int i = selectpiece + 9; i < 64; i += 9)
		{
			if (squares[i].getPiece() != 0 && squares[i].getPiece() != kingObj)
			{
				break;
			}
			else if (squares[i].getPiece() == kingObj && squares[i].getTeam() == 1)
			{
				whiteTeamInCheck = true;
			}
			if (i == 7 || i == 15 || i == 23 || i == 31 || i == 39 || i == 47 || i == 55 || i == 63)
			{
				break;
			}
		}
		for (int i = selectpiece - 7; i > 0; i -= 7)
		{
			if (squares[i].getPiece() != 0 && squares[i].getPiece() != kingObj)
			{
				break;
			}
			else if (squares[i].getPiece() == kingObj && squares[i].getTeam() == 1)
			{
				whiteTeamInCheck = true;
			}
			if (i == 7 || i == 15 || i == 23 || i == 31 || i == 39 || i == 47 || i == 55 || i == 63)
			{
				break;
			}
		}
	}
}

void whiteQueenCheckForCheck(){
	// checks white queen to see if it causes check
	for (int i = selectpiece + 8; i < 64; i += 8)
	{
		if (squares[i].getPiece() != kingObj && squares[i].getPiece() != 0)
		{
			break;
		}
		else if (squares[i].getPiece() == kingObj && squares[i].getTeam() == 0)
		{
			blackTeamInCheck = true;
		}
	}
	for (int i = selectpiece - 8; i > 0; i -= 8)
	{
		if (squares[i].getPiece() != 0 && squares[i].getPiece() != kingObj)
		{
			break;
		}
		else if (squares[i].getPiece() == kingObj && squares[i].getTeam() == 0)
		{
			blackTeamInCheck = true;
		}
	}

	if (selectpiece != 0 && selectpiece != 8 && selectpiece != 16 && selectpiece != 24 && selectpiece != 32 && selectpiece != 40 && selectpiece != 48 && selectpiece != 56)
	{
		for (int i = selectpiece - 1; i > 0; i--)
		{
			if (squares[i].getPiece() != 0 && squares[i].getPiece() != kingObj)
			{
				break;
			}
			else if (squares[i].getPiece() == kingObj && squares[i].getTeam() == 0)
			{
				blackTeamInCheck = true;
			}
			if (i == 0 || i == 8 || i == 16 || i == 24 || i == 32 || i == 40 || i == 48 || i == 56)
			{
				break;
			}
		}

		for (int i = selectpiece + 7; i < 64; i += 7)
		{
			if (squares[i].getPiece() != 0 && squares[i].getPiece() != kingObj)
			{
				break;
			}
			else if (squares[i].getPiece() == kingObj && squares[i].getTeam() == 0)
			{
				blackTeamInCheck = true;
			}
			if (i == 0 || i == 8 || i == 16 || i == 24 || i == 32 || i == 40 || i == 48 || i == 56)
			{
				break;
			}
		}

		for (int i = selectpiece - 9; i > 0; i -= 9)
		{
			if (squares[i].getPiece() != 0 && squares[i].getPiece() != kingObj)
			{
				break;
			}
			else if (squares[i].getPiece() == kingObj && squares[i].getTeam() == 0)
			{
				blackTeamInCheck = true;
			}
			if (i == 0 || i == 8 || i == 16 || i == 24 || i == 32 || i == 40 || i == 48 || i == 56)
			{
				break;
			}
		}
	}

	if (selectpiece != 7 && selectpiece != 15 && selectpiece != 23 && selectpiece != 31 && selectpiece != 39 && selectpiece != 47 && selectpiece != 55 && selectpiece != 63)
	{
		for (int i = selectpiece + 1; i < 64; i++)
		{
			if (squares[i].getPiece() != 0 && squares[i].getPiece() != kingObj)
			{
				break;
			}
			else if (squares[i].getPiece() == kingObj && squares[i].getTeam() == 0)
			{
				blackTeamInCheck = true;
			}
			if (i == 7 || i == 15 || i == 23 || i == 31 || i == 39 || i == 47 || i == 55 || i == 63)
			{
				break;
			}
		}

		for (int i = selectpiece + 9; i < 64; i += 9)
		{
			if (squares[i].getPiece() != 0 && squares[i].getPiece() != kingObj)
			{
				break;
			}
			else if (squares[i].getPiece() == kingObj && squares[i].getTeam() == 0)
			{
				blackTeamInCheck = true;
			}
			if (i == 7 || i == 15 || i == 23 || i == 31 || i == 39 || i == 47 || i == 55 || i == 63)
			{
				break;
			}
		}
		for (int i = selectpiece - 7; i > 0; i -= 7)
		{
			if (squares[i].getPiece() != 0 && squares[i].getPiece() != kingObj)
			{
				break;
			}
			else if (squares[i].getPiece() == kingObj && squares[i].getTeam() == 0)
			{
				blackTeamInCheck = true;
			}
			if (i == 7 || i == 15 || i == 23 || i == 31 || i == 39 || i == 47 || i == 55 || i == 63)
			{
				break;
			}
		}
	}
}

void blackKnightCheckForCheck(){
	// checks black knight to see if it causes check
		if( squares[selectpiece].getX() != 0){
			if (squares[selectpiece - 17].getTeam() == 1 && squares[selectpiece-17].getPiece() == kingObj){
				whiteTeamInCheck = true;
			}

			if (squares[selectpiece + 15].getTeam() == 1 && squares[selectpiece+15].getPiece() == kingObj){
				whiteTeamInCheck = true;
			}
		}

		if( squares[selectpiece].getX() != -7){
			if (squares[selectpiece - 15].getTeam() == 1 && squares[selectpiece- 15].getPiece() == kingObj){
				whiteTeamInCheck = true;
			}

			if (squares[selectpiece + 17].getTeam() == 1 && squares[selectpiece+17].getPiece() == kingObj){
				whiteTeamInCheck = true;
			}
		}

		if (squares[selectpiece].getX() != -6 && squares[selectpiece].getX() != -7) {
			if (squares[selectpiece - 6].getTeam() == 1 && squares[selectpiece- 6].getPiece() == kingObj){
				whiteTeamInCheck = true;
			}

			if (squares[selectpiece + 10].getTeam() == 1 && squares[selectpiece+10].getPiece() == kingObj){
				whiteTeamInCheck = true;
			}
		}

		if (squares[selectpiece].getX() != 0 && squares[selectpiece].getX() != -1) {
			if (squares[selectpiece + 6].getTeam() == 1 && squares[selectpiece +6].getPiece() == kingObj){
				whiteTeamInCheck = true;
			}

			if (squares[selectpiece - 10].getTeam() == 1 && squares[selectpiece - 10].getPiece() == kingObj){
				whiteTeamInCheck = true;
			}
		}
}

void whiteKnightCheckForCheck(){
	// checks white knight to see if it causes check
		if( squares[selectpiece].getX() != 0){
			if (squares[selectpiece - 17].getTeam() == 0 && squares[selectpiece-17].getPiece() == kingObj){
				blackTeamInCheck = true;
			}

			if (squares[selectpiece + 15].getTeam() == 0 && squares[selectpiece+15].getPiece() == kingObj){
				blackTeamInCheck = true;
			}
		}

		if( squares[selectpiece].getX() != -7){
			if (squares[selectpiece - 15].getTeam() == 0 && squares[selectpiece- 15].getPiece() == kingObj){
				blackTeamInCheck = true;
			}

			if (squares[selectpiece + 17].getTeam() == 0 && squares[selectpiece+17].getPiece() == kingObj){
				blackTeamInCheck = true;
			}
		}

		if (squares[selectpiece].getX() != -6 && squares[selectpiece].getX() != -7) {
			if (squares[selectpiece - 6].getTeam() == 0 && squares[selectpiece- 6].getPiece() == kingObj){
				blackTeamInCheck = true;
			}

			if (squares[selectpiece + 10].getTeam() == 0 && squares[selectpiece+10].getPiece() == kingObj){
				blackTeamInCheck = true;
			}
		}

		if (squares[selectpiece].getX() != 0 && squares[selectpiece].getX() != -1) {
			if (squares[selectpiece + 6].getTeam() == 0 && squares[selectpiece +6].getPiece() == kingObj){
				blackTeamInCheck = true;
			}

			if (squares[selectpiece - 10].getTeam() == 0 && squares[selectpiece - 10].getPiece() == kingObj){
				blackTeamInCheck = true;
			}
		}
}

void blackBishopCheckForCheck(){
	// checks black bishop to see if it causes check
		if( squares[selectpiece].getX() != 0){
			for (int i = selectpiece + 7; i < 64; i += 7)
			{
				if (squares[i].getTeam() == 1 && squares[i].getPiece() == kingObj)
				{
					whiteTeamInCheck = true;					
					break;
				}
				if (i == 0 || i == 8 || i == 16 || i == 24 || i == 32 || i == 40 || i == 48 || i == 56)
				{
					break;
				}
			}

			for (int i = selectpiece - 9; i > 0; i -= 9)
			{
				if (squares[i].getTeam() == 1 && squares[i].getPiece() == kingObj)
				{
					whiteTeamInCheck = true;					
					break;
				}
				if (i == 0 || i == 8 || i == 16 || i == 24 || i == 32 || i == 40 || i == 48 || i == 56)
				{
					break;
				}
			}
		}

		if( squares[selectpiece].getX() != -7)
		{
			for (int i = selectpiece + 9; i < 64; i += 9)
			{
				if (squares[i].getTeam() == 1 && squares[i].getPiece() == kingObj)
				{
					whiteTeamInCheck = true;					
					break;
				}
				if (i == 7 || i == 15 || i == 23 || i == 31 || i == 39 || i == 47 || i == 55 || i == 63)
				{
					break;
				}
			}
			for (int i = selectpiece - 7; i > 0; i -= 7)
			{
				if (squares[i].getTeam() == 1 && squares[i].getPiece() == kingObj)
				{
					whiteTeamInCheck = true;					
					break;
				}
				if (i == 7 || i == 15 || i == 23 || i == 31 || i == 39 || i == 47 || i == 55 || i == 63)
				{
					break;
				}
			}
		}
	}

void whiteBishopCheckForCheck(){
	// checks white bishop to see if it causes check
	if( squares[selectpiece].getX() != 0){
		for (int i = selectpiece + 7; i < 64; i += 7)
		{
			if (squares[i].getTeam() == 0 && squares[i].getPiece() == kingObj)
			{
				blackTeamInCheck = true;					
				break;
			}
			if (i == 0 || i == 8 || i == 16 || i == 24 || i == 32 || i == 40 || i == 48 || i == 56)
			{
				break;
			}
		}

		for (int i = selectpiece - 9; i > 0; i -= 9)
		{
			if (squares[i].getTeam() == 0 && squares[i].getPiece() == kingObj)
			{
				blackTeamInCheck = true;					
				break;
			}
			if (i == 0 || i == 8 || i == 16 || i == 24 || i == 32 || i == 40 || i == 48 || i == 56)
			{
				break;
			}
		}
	}

	if( squares[selectpiece].getX() != -7)
	{
		for (int i = selectpiece + 9; i < 64; i += 9)
		{
			if (squares[i].getTeam() == 0 && squares[i].getPiece() == kingObj)
			{
				blackTeamInCheck = true;					
				break;
			}
			if (i == 7 || i == 15 || i == 23 || i == 31 || i == 39 || i == 47 || i == 55 || i == 63)
			{
				break;
			}
		}
		for (int i = selectpiece - 7; i > 0; i -= 7)
		{
			if (squares[i].getTeam() == 0 && squares[i].getPiece() == kingObj)
			{
				blackTeamInCheck = true;					
				break;
			}
			if (i == 7 || i == 15 || i == 23 || i == 31 || i == 39 || i == 47 || i == 55 || i == 63)
			{
				break;
			}
		}
	}
}

//below checks piece and highlights its available moves including hightlighting of pieces it can take out
void checkPiece(int selectpiece)
{
	//black pawn highlight
	if (squares[selectpiece].getPiece() == pawnObj && playerTurn % 2 == 0 && squares[selectpiece].getTeam() == 0)
	{
		for (int i = 0; i < 64; i++)
	{
		highlightedSquares[i].setHighlight(0);
	}
		piecemoved = false;
		for (int i = 0; i < 64; i++)
		{
			highlightedSquares[i].setHighlight(0);
		}
		if (squares[selectpiece + 8].getPiece() == 0)
		{
			squares[selectpiece + 8].setHighlight(1);
			highlightedSquares[selectpiece + 8].setHighlight(1);
		}
		if (squares[selectpiece + 16].getPiece() == 0 && squares[selectpiece + 8].getPiece() == 0 && squares[selectpiece].getZ() == -1)
		{
			squares[selectpiece + 16].setHighlight(1);
			highlightedSquares[selectpiece + 16].setHighlight(1);
		}
		if (squares[selectpiece].getX() == 0)
		{
			if (squares[selectpiece + 9].getTeam() == 1)
			{
				highlightedSquares[selectpiece + 9].setHighlight(2);
			}
		}
		if (squares[selectpiece].getX() == -7)
		{
			if (squares[selectpiece + 7].getTeam() == 1)
			{
				highlightedSquares[selectpiece + 7].setHighlight(2);
			}
		}
		if (squares[selectpiece].getX() != -7 && squares[selectpiece].getX() != 0)
		{
			if (squares[selectpiece + 7].getTeam() == 1)
			{
				highlightedSquares[selectpiece + 7].setHighlight(2);
			}
			if (squares[selectpiece + 9].getTeam() == 1)
			{
				highlightedSquares[selectpiece + 9].setHighlight(2);
			}
		}
	}
	//white pawn highlight
	else if (squares[selectpiece].getPiece() == pawnObj && playerTurn % 2 == 1 && squares[selectpiece].getTeam() == 1)
	{
		for (int i = 0; i < 64; i++)
	{
		highlightedSquares[i].setHighlight(0);
	}
		piecemoved = false;
		for (int i = 0; i < 64; i++)
		{
			highlightedSquares[i].setHighlight(0);
		}
		if (squares[selectpiece - 8].getPiece() == 0)
		{
			squares[selectpiece - 8].setHighlight(1);
			highlightedSquares[selectpiece - 8].setHighlight(1);
		}
		if (squares[selectpiece - 16].getPiece() == 0 && squares[selectpiece - 8].getPiece() == 0 && squares[selectpiece].getZ() == -6)
		{
			squares[selectpiece - 16].setHighlight(1);
			highlightedSquares[selectpiece - 16].setHighlight(1);
		}
		if (squares[selectpiece].getX() == 0)
		{
			if (squares[selectpiece - 7].getTeam() == 0)
			{
				highlightedSquares[selectpiece - 7].setHighlight(2);
			}
		}
		if (squares[selectpiece].getX() == -7)
		{
			if (squares[selectpiece - 9].getTeam() == 0)
			{
				highlightedSquares[selectpiece - 9].setHighlight(2);
			}
		}
		if (squares[selectpiece].getX() != -7 && squares[selectpiece].getX() != 0)
		{
			if (squares[selectpiece - 7].getTeam() == 0)
			{
				highlightedSquares[selectpiece - 7].setHighlight(2);
			}
			if (squares[selectpiece - 9].getTeam() == 0)
			{
				highlightedSquares[selectpiece - 9].setHighlight(2);
			}
		}
	}

	//black rook
	else if (squares[selectpiece].getPiece() == rookObj && playerTurn % 2 == 0 && squares[selectpiece].getTeam() == 0)
	{
		for (int i = 0; i < 64; i++)
	{
		highlightedSquares[i].setHighlight(0);
	}
		for (int i = selectpiece + 8; i < 64; i += 8)
		{
			if (squares[i].getPiece() == 0)
			{
				squares[i].setHighlight(1);
				highlightedSquares[i].setHighlight(1);
			}
			else if (squares[i].getPiece() != 0)
			{
				if (squares[i].getTeam() == 1)
				{
					highlightedSquares[i].setHighlight(2);
				}
				break;
			}
		}
		for (int i = selectpiece - 8; i > 0; i -= 8)
		{
			if (squares[i].getPiece() == 0)
			{
				squares[i].setHighlight(1);
				highlightedSquares[i].setHighlight(1);
			}
			else if (squares[i].getPiece() != 0)
			{
				if (squares[i].getTeam() == 1)
				{
					highlightedSquares[i].setHighlight(2);
				}
				break;
			}
		}

		if (selectpiece != 7 && selectpiece != 15 && selectpiece != 23 && selectpiece != 31 && selectpiece != 39 && selectpiece != 47 && selectpiece != 55 && selectpiece != 63)
		{
			for (int i = selectpiece + 1; i < 64; i++)
			{
				if (squares[i].getPiece() == 0)
				{
					squares[i].setHighlight(1);
					highlightedSquares[i].setHighlight(1);
				}
				else if (squares[i].getPiece() != 0)
				{
					if (squares[i].getTeam() == 1)
					{
						highlightedSquares[i].setHighlight(2);
					}
					break;
				}
				if (i == 7 || i == 15 || i == 23 || i == 31 || i == 39 || i == 47 || i == 55 || i == 63)
				{
					break;
				}
			}
		}

		if (selectpiece != 0 && selectpiece != 8 && selectpiece != 16 && selectpiece != 24 && selectpiece != 32 && selectpiece != 40 && selectpiece != 48 && selectpiece != 56)
		{
			for (int i = selectpiece - 1; i > 0; i--)
			{
				if (squares[i].getPiece() == 0)
				{
					squares[i].setHighlight(1);
					highlightedSquares[i].setHighlight(1);
				}
				else if (squares[i].getPiece() != 0)
				{
					if (squares[i].getTeam() == 1)
					{
						highlightedSquares[i].setHighlight(2);
					}
					break;
				}
				if (i == 0 || i == 8 || i == 16 || i == 24 || i == 32 || i == 40 || i == 48 || i == 56)
				{
					break;
				}
			}
		}
	}
	//white rook
	else if (squares[selectpiece].getPiece() == rookObj && playerTurn % 2 == 1 && squares[selectpiece].getTeam() == 1)
	{
		for (int i = 0; i < 64; i++)
	{
		highlightedSquares[i].setHighlight(0);
	}
		for (int i = selectpiece + 8; i < 64; i += 8)
		{
			if (squares[i].getPiece() == 0)
			{
				squares[i].setHighlight(1);
				highlightedSquares[i].setHighlight(1);
			}
			else if (squares[i].getPiece() != 0)
			{
				if (squares[i].getTeam() == 0)
				{
					highlightedSquares[i].setHighlight(2);
				}
				break;
			}
		}
		for (int i = selectpiece - 8; i > 0; i -= 8)
		{
			if (squares[i].getPiece() == 0)
			{
				squares[i].setHighlight(1);
				highlightedSquares[i].setHighlight(1);
			}
			else if (squares[i].getPiece() != 0)
			{
				if (squares[i].getTeam() == 0)
				{
					highlightedSquares[i].setHighlight(2);
				}
				break;
			}
		}

		if (selectpiece != 7 && selectpiece != 15 && selectpiece != 23 && selectpiece != 31 && selectpiece != 39 && selectpiece != 47 && selectpiece != 55 && selectpiece != 63)
		{
			for (int i = selectpiece + 1; i < 64; i++)
			{
				if (squares[i].getPiece() == 0)
				{
					squares[i].setHighlight(1);
					highlightedSquares[i].setHighlight(1);
				}
				else if (squares[i].getPiece() != 0)
				{
					if (squares[i].getTeam() == 0)
					{
						highlightedSquares[i].setHighlight(2);
					}
					break;
				}
				if (i == 7 || i == 15 || i == 23 || i == 31 || i == 39 || i == 47 || i == 55 || i == 63)
				{
					break;
				}
			}
		}

		if (selectpiece != 0 && selectpiece != 8 && selectpiece != 16 && selectpiece != 24 && selectpiece != 32 && selectpiece != 40 && selectpiece != 48 && selectpiece != 56)
		{
			for (int i = selectpiece - 1; i > 0; i--)
			{
				if (squares[i].getPiece() == 0)
				{
					squares[i].setHighlight(1);
					highlightedSquares[i].setHighlight(1);
				}
				else if (squares[i].getPiece() != 0)
				{
					if (squares[i].getTeam() == 0)
					{
						highlightedSquares[i].setHighlight(2);
					}
					break;
				}
				if (i == 0 || i == 8 || i == 16 || i == 24 || i == 32 || i == 40 || i == 48 || i == 56)
				{
					break;
				}
			}
		}
	}

	//black bishop
	else if (squares[selectpiece].getPiece() == bishopObj && playerTurn % 2 == 0 && squares[selectpiece].getTeam() == 0)
	{

	for (int i = 0; i < 64; i++)
	{
		highlightedSquares[i].setHighlight(0);
	}
		if (selectpiece != 0 && selectpiece != 8 && selectpiece != 16 && selectpiece != 24 && selectpiece != 32 && selectpiece != 40 && selectpiece != 48 && selectpiece != 56)
		{
			for (int i = selectpiece + 7; i < 64; i += 7)
			{
				if (squares[i].getPiece() == 0)
				{
					squares[i].setHighlight(1);
					highlightedSquares[i].setHighlight(1);
				}
				else if (squares[i].getPiece() != 0)
				{
					if (squares[i].getTeam() == 1)
					{
						highlightedSquares[i].setHighlight(2);
					}
					break;
				}
				if (i == 0 || i == 8 || i == 16 || i == 24 || i == 32 || i == 40 || i == 48 || i == 56)
				{
					break;
				}
			}

			for (int i = selectpiece - 9; i > 0; i -= 9)
			{
				if (squares[i].getPiece() == 0)
				{
					squares[i].setHighlight(1);
					highlightedSquares[i].setHighlight(1);
				}
				else if (squares[i].getPiece() != 0)
				{
					if (squares[i].getTeam() == 1)
					{
						highlightedSquares[i].setHighlight(2);
					}
					break;
				}
				if (i == 0 || i == 8 || i == 16 || i == 24 || i == 32 || i == 40 || i == 48 || i == 56)
				{
					break;
				}
			}
		}

		if (selectpiece != 7 && selectpiece != 15 && selectpiece != 23 && selectpiece != 31 && selectpiece != 39 && selectpiece != 47 && selectpiece != 55 && selectpiece != 63)
		{
			for (int i = selectpiece + 9; i < 64; i += 9)
			{
				if (squares[i].getPiece() == 0)
				{
					squares[i].setHighlight(1);
					highlightedSquares[i].setHighlight(1);
				}
				else if (squares[i].getPiece() != 0)
				{
					if (squares[i].getTeam() == 1)
					{
						highlightedSquares[i].setHighlight(2);
					}
					break;
				}
				if (i == 7 || i == 15 || i == 23 || i == 31 || i == 39 || i == 47 || i == 55 || i == 63)
				{
					break;
				}
			}
			for (int i = selectpiece - 7; i > 0; i -= 7)
			{
				if (squares[i].getPiece() == 0)
				{
					squares[i].setHighlight(1);
					highlightedSquares[i].setHighlight(1);
				}
				else if (squares[i].getPiece() != 0)
				{
					if (squares[i].getTeam() == 1)
					{
						highlightedSquares[i].setHighlight(2);
					}
					break;
				}
				if (i == 7 || i == 15 || i == 23 || i == 31 || i == 39 || i == 47 || i == 55 || i == 63)
				{
					break;
				}
			}
		}
	}

	//white bishop
	else if (squares[selectpiece].getPiece() == bishopObj && playerTurn % 2 == 1 && squares[selectpiece].getTeam() == 1)
	{
		for (int i = 0; i < 64; i++)
	{
		highlightedSquares[i].setHighlight(0);
	}
		if (selectpiece != 0 && selectpiece != 8 && selectpiece != 16 && selectpiece != 24 && selectpiece != 32 && selectpiece != 40 && selectpiece != 48 && selectpiece != 56)
		{
			for (int i = selectpiece + 7; i < 64; i += 7)
			{
				if (squares[i].getPiece() == 0)
				{
					squares[i].setHighlight(1);
					highlightedSquares[i].setHighlight(1);
				}
				else if (squares[i].getPiece() != 0)
				{
					if (squares[i].getTeam() == 0)
					{
						highlightedSquares[i].setHighlight(2);
					}
					break;
				}
				if (i == 0 || i == 8 || i == 16 || i == 24 || i == 32 || i == 40 || i == 48 || i == 56)
				{
					break;
				}
			}

			for (int i = selectpiece - 9; i > 0; i -= 9)
			{
				if (squares[i].getPiece() == 0)
				{
					squares[i].setHighlight(1);
					highlightedSquares[i].setHighlight(1);
				}
				else if (squares[i].getPiece() != 0)
				{
					if (squares[i].getTeam() == 0)
					{
						highlightedSquares[i].setHighlight(2);
					}
					break;
				}
				if (i == 0 || i == 8 || i == 16 || i == 24 || i == 32 || i == 40 || i == 48 || i == 56)
				{
					break;
				}
			}
		}

		if (selectpiece != 7 && selectpiece != 15 && selectpiece != 23 && selectpiece != 31 && selectpiece != 39 && selectpiece != 47 && selectpiece != 55 && selectpiece != 63)
		{
			for (int i = selectpiece + 9; i < 64; i += 9)
			{
				if (squares[i].getPiece() == 0)
				{
					squares[i].setHighlight(1);
					highlightedSquares[i].setHighlight(1);
				}
				else if (squares[i].getPiece() != 0)
				{
					if (squares[i].getTeam() == 0)
					{
						highlightedSquares[i].setHighlight(2);
					}
					break;
				}
				if (i == 7 || i == 15 || i == 23 || i == 31 || i == 39 || i == 47 || i == 55 || i == 63)
				{
					break;
				}
			}
			for (int i = selectpiece - 7; i > 0; i -= 7)
			{
				if (squares[i].getPiece() == 0)
				{
					squares[i].setHighlight(1);
					highlightedSquares[i].setHighlight(1);
				}
				else if (squares[i].getPiece() != 0)
				{
					if (squares[i].getTeam() == 0)
					{
						highlightedSquares[i].setHighlight(2);
					}
					break;
				}
				if (i == 7 || i == 15 || i == 23 || i == 31 || i == 39 || i == 47 || i == 55 || i == 63)
				{
					break;
				}
			}
		}
	}

	//white queen
	else if (squares[selectpiece].getPiece() == queenObj && playerTurn % 2 == 1 && squares[selectpiece].getTeam() == 1)
	{
		for (int i = 0; i < 64; i++)
	{
		highlightedSquares[i].setHighlight(0);
	}
		for (int i = selectpiece + 8; i < 64; i += 8)
		{
			if (squares[i].getPiece() == 0)
			{
				squares[i].setHighlight(1);
				highlightedSquares[i].setHighlight(1);
			}
			else if (squares[i].getPiece() != 0)
			{
				if (squares[i].getTeam() == 0)
				{
					highlightedSquares[i].setHighlight(2);
				}
				break;
			}
		}
		for (int i = selectpiece - 8; i > 0; i -= 8)
		{
			if (squares[i].getPiece() == 0)
			{
				squares[i].setHighlight(1);
				highlightedSquares[i].setHighlight(1);
			}
			else if (squares[i].getPiece() != 0)
			{
				if (squares[i].getTeam() == 0)
				{
					highlightedSquares[i].setHighlight(2);
				}
				break;
			}
		}

		if (selectpiece != 0 && selectpiece != 8 && selectpiece != 16 && selectpiece != 24 && selectpiece != 32 && selectpiece != 40 && selectpiece != 48 && selectpiece != 56)
		{
			for (int i = selectpiece - 1; i > 0; i--)
			{
				if (squares[i].getPiece() == 0)
				{
					squares[i].setHighlight(1);
					highlightedSquares[i].setHighlight(1);
				}
				else if (squares[i].getPiece() != 0)
				{
					if (squares[i].getTeam() == 0)
					{
						highlightedSquares[i].setHighlight(2);
					}
					break;
				}
				if (i == 0 || i == 8 || i == 16 || i == 24 || i == 32 || i == 40 || i == 48 || i == 56)
				{
					break;
				}
			}

			for (int i = selectpiece + 7; i < 64; i += 7)
			{
				if (squares[i].getPiece() == 0)
				{
					squares[i].setHighlight(1);
					highlightedSquares[i].setHighlight(1);
				}
				else if (squares[i].getPiece() != 0)
				{
					if (squares[i].getTeam() == 0)
					{
						highlightedSquares[i].setHighlight(2);
					}
					break;
				}
				if (i == 0 || i == 8 || i == 16 || i == 24 || i == 32 || i == 40 || i == 48 || i == 56)
				{
					break;
				}
			}

			for (int i = selectpiece - 9; i > 0; i -= 9)
			{
				if (squares[i].getPiece() == 0)
				{
					squares[i].setHighlight(1);
					highlightedSquares[i].setHighlight(1);
				}
				else if (squares[i].getPiece() != 0)
				{
					if (squares[i].getTeam() == 0)
					{
						highlightedSquares[i].setHighlight(2);
					}
					break;
				}
				if (i == 0 || i == 8 || i == 16 || i == 24 || i == 32 || i == 40 || i == 48 || i == 56)
				{
					break;
				}
			}
		}

		if (selectpiece != 7 && selectpiece != 15 && selectpiece != 23 && selectpiece != 31 && selectpiece != 39 && selectpiece != 47 && selectpiece != 55 && selectpiece != 63)
		{
			for (int i = selectpiece + 1; i < 64; i++)
			{
				if (squares[i].getPiece() == 0)
				{
					squares[i].setHighlight(1);
					highlightedSquares[i].setHighlight(1);
				}
				else if (squares[i].getPiece() != 0)
				{
					if (squares[i].getTeam() == 0)
					{
						highlightedSquares[i].setHighlight(2);
					}
					break;
				}
				if (i == 7 || i == 15 || i == 23 || i == 31 || i == 39 || i == 47 || i == 55 || i == 63)
				{
					break;
				}
			}

			for (int i = selectpiece + 9; i < 64; i += 9)
			{
				if (squares[i].getPiece() == 0)
				{
					squares[i].setHighlight(1);
					highlightedSquares[i].setHighlight(1);
				}
				else if (squares[i].getPiece() != 0)
				{
					if (squares[i].getTeam() == 0)
					{
						highlightedSquares[i].setHighlight(2);
					}
					break;
				}
				if (i == 7 || i == 15 || i == 23 || i == 31 || i == 39 || i == 47 || i == 55 || i == 63)
				{
					break;
				}
			}
			for (int i = selectpiece - 7; i > 0; i -= 7)
			{
				if (squares[i].getPiece() == 0)
				{
					squares[i].setHighlight(1);
					highlightedSquares[i].setHighlight(1);
				}
				else if (squares[i].getPiece() != 0)
				{
					if (squares[i].getTeam() == 0)
					{
						highlightedSquares[i].setHighlight(2);
					}
					break;
				}
				if (i == 7 || i == 15 || i == 23 || i == 31 || i == 39 || i == 47 || i == 55 || i == 63)
				{
					break;
				}
			}
		}
	}

	//black queen
	else if (squares[selectpiece].getPiece() == queenObj && playerTurn % 2 == 0 && squares[selectpiece].getTeam() == 0)
	{
		for (int i = 0; i < 64; i++)
	{
		highlightedSquares[i].setHighlight(0);
	}
		for (int i = selectpiece + 8; i < 64; i += 8)
		{
			if (squares[i].getPiece() == 0)
			{
				squares[i].setHighlight(1);
				highlightedSquares[i].setHighlight(1);
			}
			else if (squares[i].getPiece() != 0)
			{
				if (squares[i].getTeam() == 1)
				{
					highlightedSquares[i].setHighlight(2);
				}
				break;
			}
		}
		for (int i = selectpiece - 8; i > 0; i -= 8)
		{
			if (squares[i].getPiece() == 0)
			{
				squares[i].setHighlight(1);
				highlightedSquares[i].setHighlight(1);
			}
			else if (squares[i].getPiece() != 0)
			{
				if (squares[i].getTeam() == 1)
				{
					highlightedSquares[i].setHighlight(2);
				}
				break;
			}
		}

		if (selectpiece != 0 && selectpiece != 8 && selectpiece != 16 && selectpiece != 24 && selectpiece != 32 && selectpiece != 40 && selectpiece != 48 && selectpiece != 56)
		{
			for (int i = selectpiece - 1; i > 0; i--)
			{
				if (squares[i].getPiece() == 0)
				{
					squares[i].setHighlight(1);
					highlightedSquares[i].setHighlight(1);
				}
				else if (squares[i].getPiece() != 0)
				{
					if (squares[i].getTeam() == 1)
					{
						highlightedSquares[i].setHighlight(2);
					}
					break;
				}
				if (i == 0 || i == 8 || i == 16 || i == 24 || i == 32 || i == 40 || i == 48 || i == 56)
				{
					break;
				}
			}

			for (int i = selectpiece + 7; i < 64; i += 7)
			{
				if (squares[i].getPiece() == 0)
				{
					squares[i].setHighlight(1);
					highlightedSquares[i].setHighlight(1);
				}
				else if (squares[i].getPiece() != 0)
				{
					if (squares[i].getTeam() == 1)
					{
						highlightedSquares[i].setHighlight(2);
					}
					break;
				}
				if (i == 0 || i == 8 || i == 16 || i == 24 || i == 32 || i == 40 || i == 48 || i == 56)
				{
					break;
				}
			}

			for (int i = selectpiece - 9; i > 0; i -= 9)
			{
				if (squares[i].getPiece() == 0)
				{
					squares[i].setHighlight(1);
					highlightedSquares[i].setHighlight(1);
				}
				else if (squares[i].getPiece() != 0)
				{
					if (squares[i].getTeam() == 1)
					{
						highlightedSquares[i].setHighlight(2);
					}
					break;
				}
				if (i == 0 || i == 8 || i == 16 || i == 24 || i == 32 || i == 40 || i == 48 || i == 56)
				{
					break;
				}
			}
		}

		if (selectpiece != 7 && selectpiece != 15 && selectpiece != 23 && selectpiece != 31 && selectpiece != 39 && selectpiece != 47 && selectpiece != 55 && selectpiece != 63)
		{
			for (int i = selectpiece + 1; i < 64; i++)
			{
				if (squares[i].getPiece() == 0)
				{
					squares[i].setHighlight(1);
					highlightedSquares[i].setHighlight(1);
				}
				else if (squares[i].getPiece() != 0)
				{
					if (squares[i].getTeam() == 1)
					{
						highlightedSquares[i].setHighlight(2);
					}
					break;
				}
				if (i == 7 || i == 15 || i == 23 || i == 31 || i == 39 || i == 47 || i == 55 || i == 63)
				{
					break;
				}
			}

			for (int i = selectpiece + 9; i < 64; i += 9)
			{
				if (squares[i].getPiece() == 0)
				{
					squares[i].setHighlight(1);
					highlightedSquares[i].setHighlight(1);
				}
				else if (squares[i].getPiece() != 0)
				{
					if (squares[i].getTeam() == 1)
					{
						highlightedSquares[i].setHighlight(2);
					}
					break;
				}
				if (i == 7 || i == 15 || i == 23 || i == 31 || i == 39 || i == 47 || i == 55 || i == 63)
				{
					break;
				}
			}
			for (int i = selectpiece - 7; i > 0; i -= 7)
			{
				if (squares[i].getPiece() == 0)
				{
					squares[i].setHighlight(1);
					highlightedSquares[i].setHighlight(1);
				}
				else if (squares[i].getPiece() != 0)
				{
					if (squares[i].getTeam() == 1)
					{
						highlightedSquares[i].setHighlight(2);
					}
					break;
				}
				if (i == 7 || i == 15 || i == 23 || i == 31 || i == 39 || i == 47 || i == 55 || i == 63)
				{
					break;
				}
			}
		}
	}

	//black knight
	else if (squares[selectpiece].getPiece() == knightObj && playerTurn % 2 == 0 && squares[selectpiece].getTeam() == 0)
	{
		for (int i = 0; i < 64; i++)
	{
		highlightedSquares[i].setHighlight(0);
	}
		if (selectpiece != 0 && selectpiece != 8 && selectpiece != 16 && selectpiece != 24 && selectpiece != 32 && selectpiece != 40 && selectpiece != 48 && selectpiece != 56)
		{
			if (squares[selectpiece - 17].getPiece() == 0)
			{
				squares[selectpiece - 17].setHighlight(1);
				highlightedSquares[selectpiece - 17].setHighlight(1);
			}
			else if (squares[selectpiece - 17].getPiece() != 0 && squares[selectpiece - 17].getTeam() == 1)
			{
				highlightedSquares[selectpiece - 17].setHighlight(2);
			}
			if (squares[selectpiece + 15].getPiece() == 0)
			{
				squares[selectpiece + 15].setHighlight(1);
				highlightedSquares[selectpiece + 15].setHighlight(1);
			}
			else if (squares[selectpiece + 15].getPiece() != 0 && squares[selectpiece + 15].getTeam() == 1)
			{
				highlightedSquares[selectpiece + 15].setHighlight(2);
			}
		}

		if (selectpiece != 7 && selectpiece != 15 && selectpiece != 23 && selectpiece != 31 && selectpiece != 39 && selectpiece != 47 && selectpiece != 55 && selectpiece != 63)
		{
			if (squares[selectpiece - 15].getPiece() == 0)
			{
				squares[selectpiece - 15].setHighlight(1);
				highlightedSquares[selectpiece - 15].setHighlight(1);
			}
			else if (squares[selectpiece - 15].getPiece() != 0 && squares[selectpiece - 15].getTeam() == 1)
			{
				highlightedSquares[selectpiece - 15].setHighlight(2);
			}
			if (squares[selectpiece + 17].getPiece() == 0)
			{
				squares[selectpiece + 17].setHighlight(1);
				highlightedSquares[selectpiece + 17].setHighlight(1);
			}
			else if (squares[selectpiece + 17].getPiece() != 0 && squares[selectpiece + 17].getTeam() == 1)
			{
				highlightedSquares[selectpiece + 17].setHighlight(2);
			}
		}

		if (squares[selectpiece].getX() != -6 && squares[selectpiece].getX() != -7)
		{
			if (squares[selectpiece - 6].getPiece() == 0)
			{
				squares[selectpiece - 6].setHighlight(1);
				highlightedSquares[selectpiece - 6].setHighlight(1);
			}
			else if (squares[selectpiece - 6].getPiece() != 0 && squares[selectpiece - 6].getTeam() == 1)
			{
				squares[selectpiece - 6].setHighlight(2);
				highlightedSquares[selectpiece - 6].setHighlight(2);
			}
			if (squares[selectpiece + 10].getPiece() == 0)
			{
				squares[selectpiece + 10].setHighlight(1);
				highlightedSquares[selectpiece + 10].setHighlight(1);
			}
			else if (squares[selectpiece + 10].getPiece() != 0 && squares[selectpiece + 10].getTeam() == 1)
			{
				squares[selectpiece + 10].setHighlight(2);
				highlightedSquares[selectpiece + 10].setHighlight(2);
			}
		}

		if (squares[selectpiece].getX() != 0 && squares[selectpiece].getX() != -1)
		{
			if (squares[selectpiece + 6].getPiece() == 0)
			{
				squares[selectpiece + 6].setHighlight(1);
				highlightedSquares[selectpiece + 6].setHighlight(1);
			}
			else if (squares[selectpiece + 6].getPiece() != 0 && squares[selectpiece + 6].getTeam() == 1)
			{
				squares[selectpiece + 6].setHighlight(2);
				highlightedSquares[selectpiece + 6].setHighlight(2);
			}
			if (squares[selectpiece - 10].getPiece() == 0)
			{
				squares[selectpiece - 10].setHighlight(1);
				highlightedSquares[selectpiece - 10].setHighlight(1);
			}
			else if (squares[selectpiece - 10].getPiece() != 0 && squares[selectpiece - 10].getTeam() == 1)
			{
				squares[selectpiece - 10].setHighlight(2);
				highlightedSquares[selectpiece - 10].setHighlight(2);
			}
		}
	}

	//white knight
	else if (squares[selectpiece].getPiece() == knightObj && playerTurn % 2 == 1 && squares[selectpiece].getTeam() == 1)
	{
		for (int i = 0; i < 64; i++)
	{
		highlightedSquares[i].setHighlight(0);
	}
		if (selectpiece != 0 && selectpiece != 8 && selectpiece != 16 && selectpiece != 24 && selectpiece != 32 && selectpiece != 40 && selectpiece != 48 && selectpiece != 56)
		{
			if (squares[selectpiece - 17].getPiece() == 0)
			{
				squares[selectpiece - 17].setHighlight(1);
				highlightedSquares[selectpiece - 17].setHighlight(1);
			}
			else if (squares[selectpiece - 17].getPiece() != 0 && squares[selectpiece - 17].getTeam() == 0)
			{
				squares[selectpiece - 17].setHighlight(2);
				highlightedSquares[selectpiece - 17].setHighlight(2);
			}
			if (squares[selectpiece + 15].getPiece() == 0)
			{
				squares[selectpiece + 15].setHighlight(1);
				highlightedSquares[selectpiece + 15].setHighlight(1);
			}
			else if (squares[selectpiece + 15].getPiece() != 0 && squares[selectpiece + 15].getTeam() == 0)
			{
				squares[selectpiece + 15].setHighlight(2);
				highlightedSquares[selectpiece + 15].setHighlight(2);
			}
		}

		if (selectpiece != 7 && selectpiece != 15 && selectpiece != 23 && selectpiece != 31 && selectpiece != 39 && selectpiece != 47 && selectpiece != 55 && selectpiece != 63)
		{
			if (squares[selectpiece - 15].getPiece() == 0)
			{
				squares[selectpiece - 15].setHighlight(1);
				highlightedSquares[selectpiece - 15].setHighlight(1);
			}
			else if (squares[selectpiece - 15].getPiece() != 0 && squares[selectpiece - 15].getTeam() == 0)
			{
				squares[selectpiece - 15].setHighlight(2);
				highlightedSquares[selectpiece - 15].setHighlight(2);
			}
			if (squares[selectpiece + 17].getPiece() == 0)
			{
				squares[selectpiece + 17].setHighlight(1);
				highlightedSquares[selectpiece + 17].setHighlight(1);
			}
			else if (squares[selectpiece + 17].getPiece() != 0 && squares[selectpiece + 17].getTeam() == 0)
			{
				squares[selectpiece + 17].setHighlight(2);
				highlightedSquares[selectpiece + 17].setHighlight(2);
			}
		}

		if (squares[selectpiece].getX() != -6 && squares[selectpiece].getX() != -7)
		{
			if (squares[selectpiece - 6].getPiece() == 0)
			{
				squares[selectpiece - 6].setHighlight(1);
				highlightedSquares[selectpiece - 6].setHighlight(1);
			}
			else if (squares[selectpiece - 6].getPiece() != 0 && squares[selectpiece - 6].getTeam() == 0)
			{
				squares[selectpiece - 6].setHighlight(2);
				highlightedSquares[selectpiece - 6].setHighlight(2);
			}
			if (squares[selectpiece + 10].getPiece() == 0)
			{
				squares[selectpiece + 10].setHighlight(1);
				highlightedSquares[selectpiece + 10].setHighlight(1);
			}
			else if (squares[selectpiece + 10].getPiece() != 0 && squares[selectpiece + 10].getTeam() == 0)
			{
				squares[selectpiece + 10].setHighlight(2);
				highlightedSquares[selectpiece + 10].setHighlight(2);
			}
		}

		if (squares[selectpiece].getX() != 0 && squares[selectpiece].getX() != -1)
		{
			if (squares[selectpiece + 6].getPiece() == 0)
			{
				squares[selectpiece + 6].setHighlight(1);
				highlightedSquares[selectpiece + 6].setHighlight(1);
			}
			else if (squares[selectpiece + 6].getPiece() != 0 && squares[selectpiece + 6].getTeam() == 0)
			{
				squares[selectpiece + 6].setHighlight(2);
				highlightedSquares[selectpiece + 6].setHighlight(2);
			}
			if (squares[selectpiece - 10].getPiece() == 0)
			{
				squares[selectpiece - 10].setHighlight(1);
				highlightedSquares[selectpiece - 10].setHighlight(1);
			}
			else if (squares[selectpiece - 10].getPiece() != 0 && squares[selectpiece - 10].getTeam() == 0)
			{
				squares[selectpiece - 10].setHighlight(2);
				highlightedSquares[selectpiece - 10].setHighlight(2);
			}
		}
	}

	//white king
	else if (squares[selectpiece].getPiece() == kingObj && playerTurn % 2 == 1 && squares[selectpiece].getTeam() == 1)
	{
		for (int i = 0; i < 64; i++)
	{
		highlightedSquares[i].setHighlight(0);
	}
		if (squares[selectpiece + 8].getPiece() == 0)
		{
			squares[selectpiece + 8].setHighlight(1);
			highlightedSquares[selectpiece + 8].setHighlight(1);
		}
		else if (squares[selectpiece + 8].getPiece() != 0 && squares[selectpiece + 8].getTeam() == 0)
		{
			squares[selectpiece + 8].setHighlight(2);
			highlightedSquares[selectpiece + 8].setHighlight(2);
		}
		if (squares[selectpiece - 8].getPiece() == 0)
		{
			squares[selectpiece - 8].setHighlight(1);
			highlightedSquares[selectpiece - 8].setHighlight(1);
		}
		else if (squares[selectpiece - 8].getPiece() != 0 && squares[selectpiece - 8].getTeam() == 0)
		{
			squares[selectpiece - 8].setHighlight(2);
			highlightedSquares[selectpiece - 8].setHighlight(2);
		}

		if (selectpiece != 0 && selectpiece != 8 && selectpiece != 16 && selectpiece != 24 && selectpiece != 32 && selectpiece != 40 && selectpiece != 48 && selectpiece != 56)
		{
			if (squares[selectpiece + 7].getPiece() == 0)
			{
				squares[selectpiece + 7].setHighlight(1);
				highlightedSquares[selectpiece + 7].setHighlight(1);
			}
			else if (squares[selectpiece + 7].getPiece() != 0 && squares[selectpiece + 7].getTeam() == 0)
			{
				squares[selectpiece + 7].setHighlight(2);
				highlightedSquares[selectpiece + 7].setHighlight(2);
			}
			if (squares[selectpiece - 9].getPiece() == 0)
			{
				squares[selectpiece - 9].setHighlight(1);
				highlightedSquares[selectpiece - 9].setHighlight(1);
			}
			else if (squares[selectpiece - 9].getPiece() != 0 && squares[selectpiece - 9].getTeam() == 0)
			{
				squares[selectpiece - 9].setHighlight(2);
				highlightedSquares[selectpiece - 9].setHighlight(2);
			}
			if (squares[selectpiece - 1].getPiece() == 0)
			{
				squares[selectpiece - 1].setHighlight(1);
				highlightedSquares[selectpiece - 1].setHighlight(1);
			}
			else if (squares[selectpiece - 1].getPiece() != 0 && squares[selectpiece - 1].getTeam() == 0)
			{
				squares[selectpiece - 1].setHighlight(2);
				highlightedSquares[selectpiece - 1].setHighlight(2);
			}
		}

		if (selectpiece != 7 && selectpiece != 15 && selectpiece != 23 && selectpiece != 31 && selectpiece != 39 && selectpiece != 47 && selectpiece != 55 && selectpiece != 63)
		{
			if (squares[selectpiece - 7].getPiece() == 0)
			{
				squares[selectpiece - 7].setHighlight(1);
				highlightedSquares[selectpiece - 7].setHighlight(1);
			}
			else if (squares[selectpiece - 7].getPiece() != 0 && squares[selectpiece - 7].getTeam() == 0)
			{
				squares[selectpiece - 7].setHighlight(2);
				highlightedSquares[selectpiece - 7].setHighlight(2);
			}
			if (squares[selectpiece + 9].getPiece() == 0)
			{
				squares[selectpiece + 9].setHighlight(1);
				highlightedSquares[selectpiece + 9].setHighlight(1);
			}
			else if (squares[selectpiece + 9].getPiece() != 0 && squares[selectpiece + 9].getTeam() == 0)
			{
				squares[selectpiece + 9].setHighlight(2);
				highlightedSquares[selectpiece + 9].setHighlight(2);
			}
			if (squares[selectpiece + 1].getPiece() == 0)
			{
				squares[selectpiece + 1].setHighlight(1);
				highlightedSquares[selectpiece + 1].setHighlight(1);
			}
			else if (squares[selectpiece + 1].getPiece() != 0 && squares[selectpiece + 1].getTeam() == 0)
			{
				squares[selectpiece + 1].setHighlight(2);
				highlightedSquares[selectpiece + 1].setHighlight(2);
			}
		}
	}

	//black king
	else if (squares[selectpiece].getPiece() == kingObj && playerTurn % 2 == 0 && squares[selectpiece].getTeam() == 0)
	{
		for (int i = 0; i < 64; i++)
	{
		highlightedSquares[i].setHighlight(0);
	}
		if (squares[selectpiece + 8].getPiece() == 0)
		{
			squares[selectpiece + 8].setHighlight(1);
			highlightedSquares[selectpiece + 8].setHighlight(1);
		}
		else if (squares[selectpiece + 8].getPiece() != 0 && squares[selectpiece + 8].getTeam() == 1)
		{
			squares[selectpiece + 8].setHighlight(2);
			highlightedSquares[selectpiece + 8].setHighlight(2);
		}
		if (squares[selectpiece - 8].getPiece() == 0)
		{
			squares[selectpiece - 8].setHighlight(1);
			highlightedSquares[selectpiece - 8].setHighlight(1);
		}
		else if (squares[selectpiece - 8].getPiece() != 0 && squares[selectpiece - 8].getTeam() == 1)
		{
			squares[selectpiece - 8].setHighlight(2);
			highlightedSquares[selectpiece - 8].setHighlight(2);
		}

		if (selectpiece != 0 && selectpiece != 8 && selectpiece != 16 && selectpiece != 24 && selectpiece != 32 && selectpiece != 40 && selectpiece != 48 && selectpiece != 56)
		{
			if (squares[selectpiece + 7].getPiece() == 0)
			{
				squares[selectpiece + 7].setHighlight(1);
				highlightedSquares[selectpiece + 7].setHighlight(1);
			}
			else if (squares[selectpiece + 7].getPiece() != 0 && squares[selectpiece + 7].getTeam() == 1)
			{
				squares[selectpiece + 7].setHighlight(2);
				highlightedSquares[selectpiece + 7].setHighlight(2);
			}
			if (squares[selectpiece - 9].getPiece() == 0)
			{
				squares[selectpiece - 9].setHighlight(1);
				highlightedSquares[selectpiece - 9].setHighlight(1);
			}
			else if (squares[selectpiece - 9].getPiece() != 0 && squares[selectpiece - 9].getTeam() == 1)
			{
				squares[selectpiece - 9].setHighlight(2);
				highlightedSquares[selectpiece - 9].setHighlight(2);
			}
			if (squares[selectpiece - 1].getPiece() == 0)
			{
				squares[selectpiece - 1].setHighlight(1);
				highlightedSquares[selectpiece - 1].setHighlight(1);
			}
			else if (squares[selectpiece - 1].getPiece() != 0 && squares[selectpiece - 1].getTeam() == 1)
			{
				squares[selectpiece - 1].setHighlight(2);
				highlightedSquares[selectpiece - 1].setHighlight(2);
			}
		}

		if (selectpiece != 7 && selectpiece != 15 && selectpiece != 23 && selectpiece != 31 && selectpiece != 39 && selectpiece != 47 && selectpiece != 55 && selectpiece != 63)
		{
			if (squares[selectpiece - 7].getPiece() == 0)
			{
				squares[selectpiece - 7].setHighlight(1);
				highlightedSquares[selectpiece - 7].setHighlight(1);
			}
			else if (squares[selectpiece - 7].getPiece() != 0 && squares[selectpiece - 7].getTeam() == 1)
			{
				squares[selectpiece - 7].setHighlight(2);
				highlightedSquares[selectpiece - 7].setHighlight(2);
			}
			if (squares[selectpiece + 9].getPiece() == 0)
			{
				squares[selectpiece + 9].setHighlight(1);
				highlightedSquares[selectpiece + 9].setHighlight(1);
			}
			else if (squares[selectpiece + 9].getPiece() != 0 && squares[selectpiece + 9].getTeam() == 1)
			{
				squares[selectpiece + 9].setHighlight(2);
				highlightedSquares[selectpiece + 9].setHighlight(2);
			}
			if (squares[selectpiece + 1].getPiece() == 0)
			{
				squares[selectpiece + 1].setHighlight(1);
				highlightedSquares[selectpiece + 1].setHighlight(1);
			}
			else if (squares[selectpiece + 1].getPiece() != 0 && squares[selectpiece + 1].getTeam() == 1)
			{
				squares[selectpiece + 1].setHighlight(2);
				highlightedSquares[selectpiece + 1].setHighlight(2);
			}
		}
	}

	//below handles piece moving such if a highlighted square is selected the piece will move there
	if ((highlightedSquares[selectpiece].getHighlight() == 1 || highlightedSquares[selectpiece].getHighlight() == 2) && squares[currentPiece].getPiece() != 0)
	{
		squares[selectpiece].setPiece(squares[currentPiece].getPiece());
		squares[selectpiece].setTeam(squares[currentPiece].getTeam());
		//squares[selectpiece].setPiece(squares[currentPiece].getPiece());
		
		squares[currentPiece].setPiece(0);
		squares[currentPiece].setTeam(2);

		//below checks for check and if so it sets a variable to true
		if (squares[selectpiece].getPiece() == pawnObj && squares[selectpiece].getTeam() == 0){
			blackPawnCheckForCheck();
		}
		else if (squares[selectpiece].getPiece() == pawnObj && squares[selectpiece].getTeam() == 1){
			whitePawnCheckForCheck();
		}
		else if (squares[selectpiece].getPiece() == rookObj && squares[selectpiece].getTeam() == 0){
			blackRookCheckForCheck();
		}
		else if (squares[selectpiece].getPiece() == rookObj && squares[selectpiece].getTeam() == 1){
			whiteRookCheckForCheck();
		}
		else if (squares[selectpiece].getPiece() == queenObj && squares[selectpiece].getTeam() == 0){
			blackQueenCheckForCheck();
		}
		else if (squares[selectpiece].getPiece() == queenObj && squares[selectpiece].getTeam() == 1){
			whiteQueenCheckForCheck();
		}
		else if (squares[selectpiece].getPiece() == knightObj && squares[selectpiece].getTeam() == 0){
			blackKnightCheckForCheck();
		}
		else if (squares[selectpiece].getPiece() == knightObj && squares[selectpiece].getTeam() == 1){
			whiteKnightCheckForCheck();
		}
		else if (squares[selectpiece].getPiece() == bishopObj && squares[selectpiece].getTeam() == 0){
			blackBishopCheckForCheck();
		}
		else if (squares[selectpiece].getPiece() == bishopObj && squares[selectpiece].getTeam() == 1){
			whiteBishopCheckForCheck();
		}
		//checks variable and prints to console if check is true
		if (blackTeamInCheck == true){
			printf("Black team is in check\n");
		}
		if (whiteTeamInCheck == true){
			printf("White team is in check\n");
		}
		//returns variables to false as other team moves out of check
		blackTeamInCheck = false;
		whiteTeamInCheck = false;
		//player turn tracker
		playerTurn += 1;
		//changes camera view
		if (camTrack == 0)
		{
			camTrack = 1;
		}
		else
		{
			camTrack = 0;
		}
		//resets highlighted squares
		for (int i = 0; i < 64; i++)
		{
			highlightedSquares[i].setHighlight(0);
			squares[i].setHighlight(0);
		}
		piecemoved = true;
		selectpiece = 64;

	}
}

//valid intersection calculations for our rays
//ray casting
void interCalc()
{
	//plus 40 to x if player 1 and minus 40 if player 2
	int xmousetemp = xmouse + 40;
	if (camTrack == 1)
	{
		xmousetemp -= 75;
	}
	int ymousetemp = ymouse - 60;

	GLdouble matlOfModel[16], matlOfProj[16], ray0[3], ray1[3], rayD[3];
	GLint matlView[4];

	glGetDoublev(GL_MODELVIEW_MATRIX, matlOfModel);
	glGetDoublev(GL_PROJECTION_MATRIX, matlOfProj);
	glGetIntegerv(GL_VIEWPORT, matlView);

	gluUnProject(xmousetemp, ymousetemp, 0.0, matlOfModel, matlOfProj, matlView, &ray0[0], &ray0[1], &ray0[2]);

	gluUnProject(xmousetemp, ymousetemp, 1.0, matlOfModel, matlOfProj, matlView, &ray1[0], &ray1[1], &ray1[2]);

	rayD[0] = ray1[0] - ray0[0];
	rayD[1] = ray1[1] - ray0[1];
	rayD[2] = ray1[2] - ray0[2];

	GLdouble m = sqrt(rayD[0] * rayD[0] + rayD[1] * rayD[1] + rayD[2] * rayD[2]);
	rayD[0] /= m;
	rayD[1] /= m;
	rayD[2] /= m;

	//calculatng the intersection point
	for (int i = 0; i < 64; i++)
	{
		double pt[3];
		double temp = (squares[i].getZ() - ray0[2]);
		double t = temp / rayD[2];
		pt[0] = ray0[0] + t * rayD[0];
		pt[1] = ray0[1] + t * rayD[1];
		pt[2] = squares[i].getZ();

		//detect if point clicked is inside our wireframe
		if (pt[1] > 0.3 - bOff && pt[2] < squares[i].getZ() + bOff && pt[0] > squares[i].getX() - bOff &&
			pt[0] < squares[i].getX() + bOff && pt[1] < 0.3 + bOff && pt[2] > squares[i].getZ() - bOff)
		{
			selectpiece = i;
			bOff = 1;
			intersect = true;
		}
		else
		{
			intersect = false;
		}
	}

	for (int i = 0; i < 64; i++)
	{
		squares[i].setHighlight(0);
	}
	checkPiece(selectpiece);
	currentPiece = selectpiece;
}

//drawing wireframe for object selected
void selectedObj(int type)
{
	glDisable(GL_LIGHTING);

	glPushMatrix();
	float x = squares[type].getX();
	float y = 0.3;
	float z = squares[type].getZ();
	glColor3f(1, 0, 0);
	glTranslatef(x, y, z);

	//wireframe scaling
	float wire = 0.5;

	//draws actual wireframe
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

	glEnable(GL_LIGHTING);
}

//handles mouse interaction and passes values to ray calculations
void mouse(int btn, int state, int x, int y)
{
	xmouse = x;
	ymouse = 800 - y;

	if (btn == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		selected = selectpiece;
	}
	if (piecemoved)
	{

		selected = 64;
	}
	interCalc();
}

//handles redisplaying
void FPS(int val)
{
	glutPostRedisplay();
	glutTimerFunc(17, FPS, 0);
}

//makes starting board with everything in correct positions
void makeBoard(void)
{
	squares[0] = Square(0, 0, rookObj, 0, 0, 0);
	squares[1] = Square(-1, 0, knightObj, 0, 1, 0);
	squares[2] = Square(-2, 0, bishopObj, 0, 0, 0);
	squares[3] = Square(-3, 0, queenObj, 0, 1, 0);
	squares[4] = Square(-4, 0, kingObj, 0, 0, 0);
	squares[5] = Square(-5, 0, bishopObj, 0, 1, 0);
	squares[6] = Square(-6, 0, knightObj, 0, 0, 0);
	squares[7] = Square(-7, 0, rookObj, 0, 1, 0);

	squares[8] = Square(0, -1, pawnObj, 0, 1, 0);
	squares[9] = Square(-1, -1, pawnObj, 0, 0, 0);
	squares[10] = Square(-2, -1, pawnObj, 0, 1, 0);
	squares[11] = Square(-3, -1, pawnObj, 0, 0, 0);
	squares[12] = Square(-4, -1, pawnObj, 0, 1, 0);
	squares[13] = Square(-5, -1, pawnObj, 0, 0, 0);
	squares[14] = Square(-6, -1, pawnObj, 0, 1, 0);
	squares[15] = Square(-7, -1, pawnObj, 0, 0, 0);

	squares[16] = Square(0, -2, 0, 2, 0, 0);
	squares[17] = Square(-1, -2, 0, 2, 1, 0);
	squares[18] = Square(-2, -2, 0, 2, 0, 0);
	squares[19] = Square(-3, -2, 0, 2, 1, 0);
	squares[20] = Square(-4, -2, 0, 2, 0, 0);
	squares[21] = Square(-5, -2, 0, 2, 1, 0);
	squares[22] = Square(-6, -2, 0, 2, 0, 0);
	squares[23] = Square(-7, -2, 0, 2, 1, 0);

	squares[24] = Square(0, -3, 0, 2, 1, 0);
	squares[25] = Square(-1, -3, 0, 2, 0, 0);
	squares[26] = Square(-2, -3, 0, 2, 1, 0);
	squares[27] = Square(-3, -3, 0, 2, 0, 0);
	squares[28] = Square(-4, -3, 0, 2, 1, 0);
	squares[29] = Square(-5, -3, 0, 2, 0, 0);
	squares[30] = Square(-6, -3, 0, 2, 1, 0);
	squares[31] = Square(-7, -3, 0, 2, 0, 0);

	squares[32] = Square(0, -4, 0, 2, 0, 0);
	squares[33] = Square(-1, -4, 0, 2, 1, 0);
	squares[34] = Square(-2, -4, 0, 2, 0, 0);
	squares[35] = Square(-3, -4, 0, 2, 1, 0);
	squares[36] = Square(-4, -4, 0, 2, 0, 0);
	squares[37] = Square(-5, -4, 0, 2, 1, 0);
	squares[38] = Square(-6, -4, 0, 2, 0, 0);
	squares[39] = Square(-7, -4, 0, 2, 1, 0);

	squares[40] = Square(0, -5, 0, 2, 1, 0);
	squares[41] = Square(-1, -5, 0, 2, 0, 0);
	squares[42] = Square(-2, -5, 0, 2, 1, 0);
	squares[43] = Square(-3, -5, 0, 2, 0, 0);
	squares[44] = Square(-4, -5, 0, 2, 1, 0);
	squares[45] = Square(-5, -5, 0, 2, 0, 0);
	squares[46] = Square(-6, -5, 0, 2, 1, 0);
	squares[47] = Square(-7, -5, 0, 2, 0, 0);

	squares[48] = Square(0, -6, pawnObj, 1, 0, 0);
	squares[49] = Square(-1, -6, pawnObj, 1, 1, 0);
	squares[50] = Square(-2, -6, pawnObj, 1, 0, 0);
	squares[51] = Square(-3, -6, pawnObj, 1, 1, 0);
	squares[52] = Square(-4, -6, pawnObj, 1, 0, 0);
	squares[53] = Square(-5, -6, pawnObj, 1, 1, 0);
	squares[54] = Square(-6, -6, pawnObj, 1, 0, 0);
	squares[55] = Square(-7, -6, pawnObj, 1, 1, 0);

	squares[56] = Square(0, -7, rookObj, 1, 1, 0);
	squares[57] = Square(-1, -7, knightObj, 1, 0, 0);
	squares[58] = Square(-2, -7, bishopObj, 1, 1, 0);
	squares[59] = Square(-3, -7, queenObj, 1, 0, 0);
	squares[60] = Square(-4, -7, kingObj, 1, 1, 0);
	squares[61] = Square(-5, -7, bishopObj, 1, 0, 0);
	squares[62] = Square(-6, -7, knightObj, 1, 1, 0);
	squares[63] = Square(-7, -7, rookObj, 1, 0, 0);
	squares[64] = Square(-100, -100, 0, 0, 0, 0);
}

//highlighted board that tracks the states of highlighted places
void makeHighlightedBoard(void)
{
	highlightedSquares[0] = Square(0, 0, rookObj, 0, 0, 0);
	highlightedSquares[1] = Square(-1, 0, knightObj, 0, 1, 0);
	highlightedSquares[2] = Square(-2, 0, bishopObj, 0, 0, 0);
	highlightedSquares[3] = Square(-3, 0, queenObj, 0, 1, 0);
	highlightedSquares[4] = Square(-4, 0, kingObj, 0, 0, 0);
	highlightedSquares[5] = Square(-5, 0, bishopObj, 0, 1, 0);
	highlightedSquares[6] = Square(-6, 0, knightObj, 0, 0, 0);
	highlightedSquares[7] = Square(-7, 0, rookObj, 0, 1, 0);

	highlightedSquares[8] = Square(0, -1, pawnObj, 0, 1, 0);
	highlightedSquares[9] = Square(-1, -1, pawnObj, 0, 0, 0);
	highlightedSquares[10] = Square(-2, -1, pawnObj, 0, 1, 0);
	highlightedSquares[11] = Square(-3, -1, pawnObj, 0, 0, 0);
	highlightedSquares[12] = Square(-4, -1, pawnObj, 0, 1, 0);
	highlightedSquares[13] = Square(-5, -1, pawnObj, 0, 0, 0);
	highlightedSquares[14] = Square(-6, -1, pawnObj, 0, 1, 0);
	highlightedSquares[15] = Square(-7, -1, pawnObj, 0, 0, 0);

	highlightedSquares[16] = Square(0, -2, 0, 0, 0, 0);
	highlightedSquares[17] = Square(-1, -2, 0, 0, 1, 0);
	highlightedSquares[18] = Square(-2, -2, 0, 0, 0, 0);
	highlightedSquares[19] = Square(-3, -2, 0, 0, 1, 0);
	highlightedSquares[20] = Square(-4, -2, 0, 0, 0, 0);
	highlightedSquares[21] = Square(-5, -2, 0, 0, 1, 0);
	highlightedSquares[22] = Square(-6, -2, 0, 0, 0, 0);
	highlightedSquares[23] = Square(-7, -2, 0, 0, 1, 0);

	highlightedSquares[24] = Square(0, -3, 0, 0, 1, 0);
	highlightedSquares[25] = Square(-1, -3, 0, 0, 0, 0);
	highlightedSquares[26] = Square(-2, -3, 0, 0, 1, 0);
	highlightedSquares[27] = Square(-3, -3, 0, 0, 0, 0);
	highlightedSquares[28] = Square(-4, -3, 0, 0, 1, 0);
	highlightedSquares[29] = Square(-5, -3, 0, 0, 0, 0);
	highlightedSquares[30] = Square(-6, -3, 0, 0, 1, 0);
	highlightedSquares[31] = Square(-7, -3, 0, 0, 0, 0);

	highlightedSquares[32] = Square(0, -4, 0, 0, 0, 0);
	highlightedSquares[33] = Square(-1, -4, 0, 0, 1, 0);
	highlightedSquares[34] = Square(-2, -4, 0, 0, 0, 0);
	highlightedSquares[35] = Square(-3, -4, 0, 0, 1, 0);
	highlightedSquares[36] = Square(-4, -4, 0, 0, 0, 0);
	highlightedSquares[37] = Square(-5, -4, 0, 0, 1, 0);
	highlightedSquares[38] = Square(-6, -4, 0, 0, 0, 0);
	highlightedSquares[39] = Square(-7, -4, 0, 0, 1, 0);

	highlightedSquares[40] = Square(0, -5, 0, 0, 1, 0);
	highlightedSquares[41] = Square(-1, -5, 0, 0, 0, 0);
	highlightedSquares[42] = Square(-2, -5, 0, 0, 1, 0);
	highlightedSquares[43] = Square(-3, -5, 0, 0, 0, 0);
	highlightedSquares[44] = Square(-4, -5, 0, 0, 1, 0);
	highlightedSquares[45] = Square(-5, -5, 0, 0, 0, 0);
	highlightedSquares[46] = Square(-6, -5, 0, 0, 1, 0);
	highlightedSquares[47] = Square(-7, -5, 0, 0, 0, 0);

	highlightedSquares[48] = Square(0, -6, pawnObj, 1, 0, 0);
	highlightedSquares[49] = Square(-1, -6, pawnObj, 1, 1, 0);
	highlightedSquares[50] = Square(-2, -6, pawnObj, 1, 0, 0);
	highlightedSquares[51] = Square(-3, -6, pawnObj, 1, 1, 0);
	highlightedSquares[52] = Square(-4, -6, pawnObj, 1, 0, 0);
	highlightedSquares[53] = Square(-5, -6, pawnObj, 1, 1, 0);
	highlightedSquares[54] = Square(-6, -6, pawnObj, 1, 0, 0);
	highlightedSquares[55] = Square(-7, -6, pawnObj, 1, 1, 0);

	highlightedSquares[56] = Square(0, -7, rookObj, 1, 1, 0);
	highlightedSquares[57] = Square(-1, -7, knightObj, 1, 0, 0);
	highlightedSquares[58] = Square(-2, -7, bishopObj, 1, 1, 0);
	highlightedSquares[59] = Square(-3, -7, queenObj, 1, 0, 0);
	highlightedSquares[60] = Square(-4, -7, kingObj, 1, 1, 0);
	highlightedSquares[61] = Square(-5, -7, bishopObj, 1, 0, 0);
	highlightedSquares[62] = Square(-6, -7, knightObj, 1, 1, 0);
	highlightedSquares[63] = Square(-7, -7, rookObj, 1, 0, 0);
	highlightedSquares[64] = Square(-100, -100, 0, 0, 0, 0);
}

void keyboard(unsigned char key, int xIn, int yIn)
{
	switch (key)
	{
	//exit functionality
	case 'q':
	case 27:
		exit(0);
		break;

	//reset functionality
	case 'r':
		makeBoard();
		makeHighlightedBoard();
		camTrack = 1;
		playerTurn = 1;
		selected = 64;
		selectpiece = 0;
		intersect = false;
		break;
	}
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//create camera viewing transformations
	if (camTrack == 0)
	{
		gluLookAt(pl1Cam[0], pl1Cam[1], pl1Cam[2], -3.5, 0, -3.5, 0, 1, 0);
	}
	else if (camTrack == 1)
	{
		gluLookAt(pl2Cam[0], pl2Cam[1], pl2Cam[2], -3.5, 0, -3.5, 0, 1, 0);
	}

	
	glEnable(GL_COLOR_MATERIAL);

	for (int i = 0; i < 64; i++)
	{
		//color of highlighting, red for empty available space, green for space of enemy piece, if neither than normal coloured
		glPushMatrix();
		if (highlightedSquares[i].getHighlight() == 1)
		{
			glColor3f(1, 0, 0);
		}
		else if (highlightedSquares[i].getHighlight() == 2)
		{
			glColor3f(0, 1, 0);
		}
		else
		{
			glColor3f(squares[i].getColour(), squares[i].getColour(), squares[i].getColour());
		}
		//draws board
		glScalef(1, 0.40, 1);
		glTranslatef(squares[i].getX(), 0, squares[i].getZ());
		glutSolidCube(1);
		glPopMatrix();

		if (squares[i].getPiece() != 0)
		{
			// glDisable(GL_LIGHTING);
			// glDisable(GL_COLOR_MATERIAL);
			glPushMatrix();

			//piece colouring
			if (squares[i].getTeam() == 0)
			{
				glColor3f(0.3, 0.3, 0.3);
			}
			else
			{
				glColor3f(0.9, 0.9, 0.9);
			}
			glTranslatef(squares[i].getX(), 0.3, squares[i].getZ());
			glScalef(1.5, 1.5, 1.5);

			//rotates white pieces so they face correctly
			if (squares[i].getTeam() == 1)
			{
				glRotatef(180, 0, 1, 0);
			}
			//draws pieces
			glCallList(squares[i].getPiece());
			glPopMatrix();
		}
	}

	selectedObj(selected);

	//teapot rotation variables
	teapotangle++;
	teapot2angle++;
	teapot2angle++;

	//below handles all the teapot transformation and drawing
	glPushMatrix();
		glTranslatef(-3.5, 1, -3.5);
		glPushMatrix();
		glColor3f(0, 1, 0);
		glRotatef(teapotangle, 0, 1, 0);
		glTranslatef(8, 0, 0);
		glutSolidTeapot(1);
			glPushMatrix();
				glColor3f(0, 0, 1);
				glRotatef(teapot2angle, 0, 1, 0);
				glTranslatef(2, 0, 0);
				glPushMatrix();
					glRotatef(teapot2angle, 0, 1, 0);
					glScalef(0.4, 0.4, 0.4);
					glutSolidTeapot(1);
				glPopMatrix();
			glPopMatrix();
		// 3rd orbit, small yellow pot
			glPushMatrix();
			glColor3f(0.9, 0.8, 0.1);
			glRotatef(-teapot2angle, 0, 1, 0);
			glTranslatef(0, 0, 2);
				glPushMatrix();
				glRotatef(-teapot2angle, 0, 0, 1);
				glScalef(0.3, 0.3, 0.3);
				glutSolidTeapot(1);
				glPopMatrix();
			glPopMatrix();
		// 3rd orbit end
		glPopMatrix();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-3.5, 1, -3.5);
		glPushMatrix();
		glColor3f(1, 0, 0);
		glRotatef(teapotangle, 0, 1, 0);
		glTranslatef(-8, 0, 0);
		glutSolidTeapot(1);
			glPushMatrix();
			glColor3f(0.498, 1, 0.831);
			glRotatef(teapot2angle, 0, 1, 0);
			glTranslatef(2, 0, 0);
				glPushMatrix();
					glRotatef(teapot2angle, 0, 1, 0);
					glScalef(0.4, 0.4, 0.4);
					glutSolidTeapot(1);
				glPopMatrix();
			glPopMatrix();
		// 3rd orbit, small yellow pot
			glPushMatrix();
			glColor3f(0.5, 0, 0.7);
			glRotatef(-teapot2angle, 0, 1, 0);
			glTranslatef(0, 0, 2);
				glPushMatrix();
				glRotatef(-teapot2angle, 0, 0, 1);
				glScalef(0.3, 0.3, 0.3);
				glutSolidTeapot(1);
				glPopMatrix();
			glPopMatrix();
		// 3rd orbit end
		glPopMatrix();
	glPopMatrix();

	glDisable(GL_COLOR_MATERIAL);

	glutSwapBuffers();
}

void myInit(void)
{
	glClearColor(0.8, 0.8, 1, 1);
	glColor3f(1, 1, 1);

	//lighting properties
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 400);

	//obj loading
	kingObj = loadObject("kingblessnew.obj");	 //load the king.obj file
	pawnObj = loadObject("pawnblessnew.obj");	 //load the pawn.obj file
	bishopObj = loadObject("bishopblessnew.obj"); //load the king.obj file
	queenObj = loadObject("queenblessnew.obj");   //load the pawn.obj file
	rookObj = loadObject("rookblessnew.obj");	 //load the pawn.obj file
	knightObj = loadObject("knightblessnew.obj"); //load the pawn.obj file

	glFrontFace(GL_CCW);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv); //starts up GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 750);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("3D Chess"); //creates the window
	//below is print out to terminal with all info needed to use program
	printf("\n-------------------------------------------------------------------------------------------------------------------------\n");
	printf("\n				     	    WELCOME TO OUR 3D CHESS\n");
	printf("\n-------------------------------------------------------------------------------------------------------------------------\n");
	printf("\nElijah Wilding, 	wildinge, 	400013471\n");
	printf("Zack Palantzas, 	palantzz, 	400016191\n");
	printf("Corey Szeto, 		szetoc, 	400025728\n");
	printf("Aleem Haq, 		haqau4, 	1330201\n");
	printf("\n-------------------------------------------------------------------------------------------------------------------------\n");
	printf("\nHere is our final project implementation of 3D Chess. Please make sure to read this as it contains some important\n");
	printf("information. Firstly, we felt as if the graphics aspect of this program was much more important than the actual back end\n");
	printf("thus we focused more on the look and feel of the project then on actual features such as AI. Secondly, when a player\n");
	printf("takes their turn the camera flips to the other player's side. We just wanted to bring this up as it is a rather fast\n");
	printf("flip and did not want to cause confusion. Thirdly, the program has some bugs when it is run on Linux. Running on MacOs\n");
	printf("and Windows allows the program to run perfectly so it is definitely preferable. Fourthly, due to the extent of the\n");
	printf("implementation we didn't have time to implement checkmate but we did implement check as shown by printout to console\n");
	printf("when a certain team is in check. Another big plus of our project is that we created the chess pieces ourselves in\n");
	printf("Maya and Blender which were then loaded the with an obj loader. Also all of the chess logic was brainstormed, created,\n");
	printf("and implemented by us. Lastly, we tried to implement a text display overolay however when we tried it royally screwed\n");
	printf("up our ray casting and we decided a console text print out was better as it allowed our ray casting to function correctly.\n");
	printf("\nControls:\n");
	printf("left mouse click - selects object if correctly hovering over it\n");
	printf("q, esc - quits program\n");
	printf("r - resets board to start new game\n");
	printf("\nThanks for a great semester! We all loved this course.\n ");
	printf("-------------------------------------------------------------------------------------------------------------------------\n\n");
	glutDisplayFunc(display); //registers "display" as the display callback function
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutTimerFunc(0, FPS, 0);

	glEnable(GL_DEPTH_TEST);

	myInit();
	makeBoard();
	glutMainLoop(); //starts the event loop

	return (0); //return may not be necessary on all compilers
}