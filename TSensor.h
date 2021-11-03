#ifndef _TSENSOR_H_
#define _TSENSOR_H_


#include <iostream>
#include <string>
#include "wiring_digital.c"
#include "wiring_analog.c"


#define ANALOG_MAX_READ 3
#define ASENSOR_TARGET1 0 
#define ASENSOR_TARGET2 1023

using namespace std;

bool EVENT_TRIGGER = true;

class Sensor{
private:
	string type;	//the type of sensor
	string op;	//operation type, analog or digital
	uint8_t num;	//sensor number (id)(arduino pin)
	bool dPass; 	//status of digital sensor, true(1) = work/pass false(0) = failed
	bool aPass[3];	//status of analog sensor, corresponding with the 3 ADC meaurements, true(1) = work/pass false(0) = failed
	bool tested; 	//status of sensor tested, true(1) = tested false(0) = not tested
	int analogValue[3]; //variable holds the values measured by ADC
public:	
	//CONSTRUCTORS

	Sensor(){
		type = " ";
		op = " ";
		num = 0;
		dPass = true;
		aPass = [true,true,true];
		tested = false;
		analogValue = [0,0,0];
	}
	Sensor(string type, string op, uint8_t num){
		this.type = type;
		this.op = op;
		this.num = num;
		dPass = true;
		aPass = [true,true,true];
		tested = false;
		analogValue= [0,0,0];
	}
	~Sensor(){
		delete type;
		delete op;
		delete aPass;
		delete analogValue;
	}
	
	//SETTERS
	void setType(string type){this.type = type;}
	void setOp(string op){this.op = op;}
	void setNum(uint8_t num){this.num = num;}
	void setDPass(bool dPass){this.dPass = dPass;}
	void setAPass(bool aPass, int index){this.aPass[index] = aPass;}
	void setTested(bool tested){this.tested = tested;}
	void setAnalogValue(int val, int index){this.analogValue[index] = val;}	

	//GETTERS
	string	getType(){return type;}
	string getOp(){return op;}
	uint8_t getNum(){return num;}
	bool getDPass(){return dPass;}
	bool getAPass(int index){return this.aPass[index];}
	bool getTested(){return tested;}
	int getAnalogValue(int index){return this.analogValue[index];}
	
	
};


class SensorTestReport{
private: 
public:
	string generateDSensorReport(Sensor * sensor);
	string generateASensorRepot(Sensor * sensor);
	string generateDSensorGroupReport(Sensor * sensor);
	string generateASensorGroupReport(Sensor * sensor);
	
	string parseReportForCSV(); //spreadsheet report
};

class SensorTestReportList{
public:
	SensorTestReport * pre;
	SensorTestReport * next;
};

class SensorTestRun{
public:
	SensorTestReport * runDigitalSensorTest(Sensor * sensor){
		SensorTestReport * report = new SensorTestReport();
		bool state;
		
		state = digitalReadSensor(sensor->getNumber());
		
		while(EVENT_TRIGGER); //variable must be changed to false to exit loop, (external trigger/interrupt)
		
		sensor->setDPass((state != digitalReadSensor(sensor))? true:false);
		sensor->setTested(true);
		
		EVENT_TRIGGER = true; //revert back to true awaiting the next event
		
		report->generateDSensorReport(sensor);
		return report;		
	}

	SensorTestReport * runAnalogSensorTest(Sensor * sensor){
		SensorTestReport * report = new SensorTestReport();
		int val;
			
		for(int i = 0; i < ANALOG_MAX_READ; i++ ){
			val = analogReadSensor(sensor);
			sensor->setAnalogValue(val,i);
			
			if(i == 0)sensor->setAPass((val==ASENSOR_TARGET1) ? true:false,i);
			if(i == 1 )sensor->setAPass((ASENSOR_TARGET1<val<ASENSOR_TARGET2) ? true:false,i);
			if(i == 2)sensor->setAPass((val==ASENSOR_TARGET2) ? true:false,i);

			if(i != 2) while(EVENT_TRIGGER); //only occurs on after all values are collected, must be changed to false to exit loop (external trigger/interrupt)
	
		}
		sensor->setTested(true);

		EVENT_TRIGGER = true;	//revert back to true awaiting the next event

		report->generateASensorReportsensor(sensor);
		return report;
	}
	
	SensorTestReport * runDigitalSensorGroupTest(Sensor * sensors, int SIZE){
		SensorTestReport * report = new SensorTestReport();
		bool state[SIZE];
		
		//collect all the state(output) of each sensors
		for (int i = 0; i < SIZE; i++){
			state[i] = digitalReadSensor(sensors[i]->getNumber());
		} 
		
		while(EVENT_TRIGGER); //variable must be changed to false to exit loop, (external trigger/interrupt)
		
		//compare each sensor's current state to previous state 
		for(int i = 0; i < SIZE; i++){
			sensors[i]->setDPass((state[i] != digitalReadSensor(sensors[i]->getNumber()))? true:false);
			sensors[i]->setTested(true);
		}
		
		EVENT_TRIGGER = true; // revert back to true awaiting the next event
		
		report->generateDSensorGroupReport(sensors);
		return report;
	}
	
	SensorTestReport * runAnalogSensorGroupTest(Sensor * sensor, int SIZE){
		SensorTestReport * report  = new SensorTestReport();
		int val, i = 0, j = 0;
		
		for(j = 0; j < ANALOG_MAX_READ; j++){
			for(i = 0; i < SIZE; i++){	
				val = analogReadSensor(sensors[i]);
				sensors[i]->setAnalogValue(val,j);	
				
				if(j == 0)sensors[i]->setAPass((val == ASENSOR_TARGET1) ? true:false , j);
				if(j == 1)sensors[i]->setAPass((ASENSOR_TARGET1 < val < ASENSOR_TARGET2) ? true:false , j);
				if(j == 2)sensor[i]->setAPass((val == ASENSOR_TARGET2) ? true:false , j);
				
				if (j == 2) sensors[i]->setTested(true); //on the final run, change test state of each sensor		
			}
			if(j != 2 ) while(EVENT_TRIGGER); //occurs after each sensor get their values except final set of values 
		}
		
		EVENT_TRIGGER = true; //revert back to true awaiting the next event

		report->generateASensorGroupReport(sensors);
		return report;
		
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
