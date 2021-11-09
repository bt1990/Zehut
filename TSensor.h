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
	int num;	//sensor number (id)(arduino pin)
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
	void setNum(int num){this.num = num;}
	void setDPass(bool dPass){this.dPass = dPass;}
	void setAPass(bool aPass, int index){this.aPass[index] = aPass;}
	void setTested(bool tested){this.tested = tested;}
	void setAnalogValue(int val, int index){this.analogValue[index] = val;}	

	//GETTERS
	string	getType(){return type;}
	string getOp(){return op;}
	int getNum(){return num;}
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
	
	/**
	* Run digital test procedure on 'sensor' object
	* 
	* @param 'sensor' object whose test is being performed on 
	* @return an object of 'SensorTestReport' class
	*/
	SensorTestReport * runDigitalSensorTest(Sensor * sensor){
		SensorTestReport * report = new SensorTestReport();
		SensorControl controller = SensorControl();
		bool state;
		
		state = controller.digitalReadSensor(sensor->getNumber());
		
		while(EVENT_TRIGGER); //variable must be changed to false to exit loop, (external trigger/interrupt)
		
		sensor->setDPass((state != controller.digitalReadSensor(sensor))? true:false);
		sensor->setTested(true);
		
		EVENT_TRIGGER = true; //revert back to true awaiting the next event
		
		report->generateDSensorReport(sensor);
		return report;		
	}
	
	/**
	* Run analog test procedure on 'sensor' object 
	* 
	* @param 'sensor' object whose test is being performed on 
	* @return an object of 'SensorTestReport' class
	*/
	SensorTestReport * runAnalogSensorTest(Sensor * sensor){
		SensorTestReport * report = new SensorTestReport();
		SensorControl controller = SensorControl();
		int val;
			
		for(int i = 0; i < ANALOG_MAX_READ; i++ ){
			val = controller.analogReadSensor(sensor);
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
	
	/**
	* Run digital test procedure on group of 'Sensor' object passed as the parameter
	*
	* @param 'sensor' array of objects whose test is being performed on 
	* @param 'SIZE' holds the number of objects to be tested
	* @return an object of 'SensorTestReport' class
	*/
	SensorTestReport * runDigitalSensorGroupTest(Sensor * sensors, int SIZE){
		SensorTestReport * report = new SensorTestReport();
		SensorControl controller = SensorControl();
		bool state[SIZE];
		
		//collect all the state(output) of each sensors
		for (int i = 0; i < SIZE; i++){
			state[i] = controller.digitalReadSensor(sensors[i]->getNumber());
		} 
		
		while(EVENT_TRIGGER); //variable must be changed to false to exit loop, (external trigger/interrupt)
		
		//compare each sensor's current state to previous state 
		for(int i = 0; i < SIZE; i++){
			sensors[i]->setDPass((state[i] != controller.digitalReadSensor(sensors[i]->getNumber()))? true:false);
			sensors[i]->setTested(true);
		}
		
		EVENT_TRIGGER = true; // revert back to true awaiting the next event
		
		report->generateDSensorGroupReport(sensors);
		return report;
	}
	
	/**
	* Run analog test procedure on group of 'Sensor' object passed as the parameter
	*
	* @param 'sensor' array of objects whose test is being performed on
	* @param 'SIZE' holds the number of objects to be tested  
	* @return an object of 'SensorTestReport' class
	*/
	SensorTestReport * runAnalogSensorGroupTest(Sensor * sensor, int SIZE){
		SensorTestReport * report  = new SensorTestReport();
		SensorControl controller = SensorControl();
		int val, i = 0, j = 0;
		
		for(j = 0; j < ANALOG_MAX_READ; j++){
			for(i = 0; i < SIZE; i++){	
				val = controller.analogReadSensor(sensors[i]);
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
	const string operationTypes[] = {"Analog","Digital"};
	int numberOfSensors;
	Sensor sensors[];

public:

	SensorControl(){
		numberOfSensors = 0;
					
	}

	~SensorControl(){
		delete sensorTypes;
		delete operationTypes;
		delete sensors;
	}

	///Creates a new sensor object
	Sensor * createSensor(string type, string op,int num){
		Sensor *temp = new Sensor(type,op,num);
		return temp;
	}
	
	///Creates a group of new sensor objects with the same type and operation
	Sensor * createSensorGroup(string type, string op, int [] num, int numberOfSensors){
		Sensor * sensors[numberOfSensors]; //declare pointer to array of sensor data type
		
		//using a loop to instantiate and the define the sensor object
		for(int i = 0; i < numberOfSensors; i++){
			sensors[i] = new Sensor(type,op,num[i]);
		}
		return sensors;
	}

	///Reads the input of the digital sensor signal
	bool digitalReadSensor(Sensor * sensor){
		//check the operational type is correct
		if(sensor->getType() == "Digital"){
			digitalRead(sensor->getNumber()); //uses the sensor's number(arduino pin) to read value
			return true; //whether sensor output HIGH or LOW return true
		}else{
			//Serial.println("System is attempting to perform a digital read on an analog type sensor. If perform result maybe inconclusive. ");
			return false; //return false if the wrong operational type
		}	
		
	}
	
	///Reads the input of the analog sensor signal
	int analogReadSensor(Sensor * sensor){
	
		//check the operational type is correct
		if(sensor->type == "Analog"){
			return analogRead(sensor->getNumber()); //return the analog value outputed by the sensor
		}else{	
			//Serial.println("System is attempting to perform a analog read on an digital type sensor. If perform result maybe inconclusive. ");
			return -1; //otherwise return a negative value, which sensor's output typically don't return
		}
	}
};

#endif _TSENSOR_H_
