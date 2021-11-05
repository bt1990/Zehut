#ifndef _TBOARD_H_
#define _TBOARD_H_

#include "TSensor.h"

class Board{
private:
	int ID;
	string version;
public: 
	//CONSTRUCTOR & DESTRUCTOR
	Board(){
		ID = 0;
		version = "";
	}
	Board(int ID, string version){
		this->ID = ID;
		this->version = version;
	}

	//SETTERS
	
	void setID(int ID){
		this->ID = ID;
	}
	void setVersion(string version){
		this->version = version;
	}

	//GETTERS
	
	int getID(){
		return this->ID;
	}	
	string getVersion(){
		return this->version;
	}
};

class BoardsList{
private:
public: 
	Board * next;
	Board * pre;
};

class BoardControl{
private:
public: 
	Board * createBoard(int ID, string version){
		Board * board = new Board(ID,version);
		return board;
	}

	Board * createBoardGroup(){

	}

	void setBoardID(Board * board, int ID){
		board->setID(ID);
	}

	void setBoardVersion(Board * board, string version){
		board->setVersion(version);
	}
};

#endif _TBOARD_H_
