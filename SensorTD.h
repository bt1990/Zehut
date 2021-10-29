#ifndef _SENSORTD_H_
#define _SENSORTD_H_


#include <iostream>
#include <string>

using namespace std;

class Sensor{
private:
	string type;	//the type of sensor
	string op;	//operation type, analog or digital
	uint8_t num;	//sensor number (id)
	bool stat; 	//status of sensor, true(1) = work/pass false(0) = failed
	bool tested; 	//status of sensor tested, true(1) = tested false(0) = not tested
public:	
	//CONSTRUCTORS

	Sensor(){
		type = " ";
		op = " ";
		num = 0;
		stat = true;
		tested = false;
	}
	Sensor(string type, string op, uint8_t num){
		this.type = type;
		this.op = op;
		this.num = num;
		stat = true;
		tested = false;
	}
	~Sensor(){
		delete type;
		delete op;
	}
	
	//SETTERS
	void setType(string type){this.type = type;}
	void setOp(string op){this.op = op;}
	void setNum(uint8_t num){this.num = num;}
	void setStat(bool stat){this.stat = stat;}
	void setTested(bool tested){this.tested = tested;}
	
	//GETTERS
	string	getType(){return type;}
	string getOp(){return op;}
	uint8_t getNum(){return num;}
	bool getStat(){return stat;}
	bool getTested(){return tested;}
	
};


class SensorTestReport{

};

class SensorTestRun{

};

class SensorControl{
private:
	const string sensorTypes[] = {};
	const string operationType[] = {"Analog","Digital","Both"};
	uint8_t numberOfSensors;

public:
	 
};

#endif _SENSORTD_H_
