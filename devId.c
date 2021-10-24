#include EEPROM.h

String serDat;
byte deviceId;

void setup() 
{
  Serial.begin(115200);
  deviceId=EEPROM.read(1);
}

void loop() 
{
  assignDeviceId();
}

void assignDeviceId()
{
  if (Serial.available()  0) 
  {
    serDat = Serial.readString();
    Serial.println(serDat);
   }
   
   if (serDat.substring(0,7) == SET_ID)
   {
    deviceId = serDat.substring(7).toInt();
    EEPROM.put(1,deviceId);
    EEPROM.get(1,deviceId);
    Serial.println(deviceId);
    serDat = ;
    deviceId=0;
   }
   
  if (serDat.substring(0,7)==GET_ID)
  {
   EEPROM.get(1,deviceId);
   Serial.println(deviceId);
   serDat = ;
   deviceId=0;
   }
}
