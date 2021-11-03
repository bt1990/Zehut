#ifndef _TSENSOR_H_
#define _TSENSOR_H_


#include <iostream>
#include <string>

#define ASENSOR_

using namespace std;

class Sensor{
private:
	string type;	//the type of sensor
	string op;	//operation type, analog or digital
	uint8_t num;	//sensor number (id)
	bool pass; 	//status of sensor, true(1) = work/pass false(0) = failed
	bool tested; 	//status of sensor tested, true(1) = tested false(0) = not tested
	int analogValue[3]; //variable holds the values measured by ADC
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

class SensorTestReportList{
public:
	SensorTestReport * pre;
	SensorTestReport * next;
};

class SensorTestRun{
public:
	SensorTestReport * runDigitalSensorTest(){
	}
	SensorTestReport * runAnalogSensorTest(){
	}
	SensorTestReport * runDigitalSensorGroupTest(){
	}
	SensorTestReport * runAnalogSensorGroupTest(){
	}	
};

class SensorControl{
private:
	const string sensorTypes[] = {};
	const string operationType[] = {"Analog","Digital"};
	uint8_t numberOfSensors;
	Sensor sensors[];

public:
	 Sensor * createSensor(string type, string op,uint8_t num){
		Sensor *temp = new Sensor(type,op,num);
		return temp;
	}
	
	Sensor * createSensorGroup(string type, string op, uint8_t num, uint8_t numberOfSensor){
		Sensor sensors[numberOfSensor];
		for(int i = 0; i < numberOfSensor; i++){
			sensors[i](type,op,num);
		}
		return sensors;
	}

	bool digitalReadSensor(Sensor * sensor){
		//check the operation type is correct
		if(sensor->getType() == "Digital"){
			digitalRead(sensor->getNumber());
			return true;
		}else{
			//Serial.print("");
			return false;
		}	
		
	}
	
	int analogReadSensor(Sensor * sensor){
		if(sensor->type == "Analog"){
			return analogRead(sensor->getNumber());
		}else{
			return -1;
		}
	}
};

#endif _TSENSOR_H_
