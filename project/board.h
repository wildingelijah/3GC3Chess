#ifndef __BOARD_H__	//guard against cyclic dependancy
#define __BOARD_H__

class Square{
public:
	Square();
	Square(int x, int z, int piece, int team, int colour, int highlight);
	int getX();
	int getZ();
	int getPiece();
	int getTeam();
	int getHighlight();
	int getColour();
	void setX(int value);
	void setZ(int value);
	void setPiece(int value);
	void setTeam(int value);
	void setHighlight(int value);
	void setColour(int value);

	int x;
	int z;
	int piece;
	//0 = none
	//1 = pawn
	//2 = rook
	//3 = knight
	//4 = bishop
	//5 = queen
	//6 = king
	int colour;
	int team;
	int highlight;
};





class Board{
public:
	Board();	//constructor
	void drawBoard();
};



#endif