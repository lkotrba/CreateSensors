/* ========================================================================== */
/*                                                                            */
/*   _GenericSensor.cpp                                                       */
/*   Created on 24. �ervna 2018                                               */
/*   (c) Athor Ladislav Kotrba                                                */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

#include "_GenericSensor.h"

std::vector<AlertSensor> vectAlertSensorObj;
std::vector<KF_Filter> vectKF_FilterObj;

byte _GenericSensor::numOfAverage = 10;  //default times to read average
byte _GenericSensor::currentDev = 0;     //0bYYYYXXXX,   XXXX = number of actual device YYYY = number of instances this class


_GenericSensor::_GenericSensor() {
  //  numOfInstances + 1   this i in higer half of currentDev variable
  this->currentDev = ((this->currentDev & 0b00001111) | ((((this->currentDev & 0b11110000) >> 4) + 1) << 4)) ;
}

int32_t  _GenericSensor::read        ()  {
  //Serial.print (F("\t _GenericSensor::read")); //Serial.print (times);
  return 0;
}


long _GenericSensor::read_average(byte times)  {
  long sum = 0;
  for (byte i = 0; i < times; i++) {
    sum += read();
  }
  return (sum / times);
}


float _GenericSensor::get_value(byte times)  {
  //  Serial.print("GenericSensor::get_value.");
  return static_cast<float> (read_average(times) - OFFSET);
}

float _GenericSensor::get_units(byte times) {

  float immediateValueOfRead = get_value(times) / SCALE;
  return (immediateValueOfRead);
}

void _GenericSensor::tare(byte times) {
  //    unsigned long readValue = read();         // tenzometrical sensor gets negative value
  //    unsigned long sum = readValue;
  //    unsigned long T_minValue = readValue;
  //    unsigned long T_maxValue = readValue;
  long readValue = read();
  long sum = readValue;
  long T_minValue = readValue;
  long T_maxValue = readValue;

  //     Serial.print(F("\nreadValue: ")); Serial.print(readValue);
  //     Serial.print(F("\tsum: ")); Serial.println(sum);

  if (times > 1) {
    for (byte i = 0; i < (times - 1); i++) {
      sum += readValue = read();
      //     Serial.print(F("readValue: ")); Serial.print(readValue);
      //     Serial.print(F("\tsum: ")); Serial.println(sum);

      if (readValue < T_minValue) T_minValue = readValue;
      if (readValue > T_maxValue) T_maxValue = readValue;
    }
  }
  set_offset(sum / times);
  for (KF_Filter objKF_Filter : vectKF_FilterObj) {
    objKF_Filter.setMaxDivergence(T_maxValue - T_minValue);   //maxDivergence is in RAW units
  }
  //    Serial.print(F("maxKFDivedgence: ")); Serial.println(T_maxValue - T_minValue);
}

void _GenericSensor::set_scale(float scale) {
  SCALE = scale;
}

float _GenericSensor::get_scale() const {
  return SCALE;
}

void _GenericSensor::set_offset(float offset) {
  OFFSET = offset;
}

float _GenericSensor::get_offset() const {
  return OFFSET;
}


// Alarm metods
void addAllarmObject(float MinVal, float MaxVal ) {
  vectAlertSensorObj.push_back(AlertSensor(MinVal, MaxVal));
}

void  _GenericSensor::calc_ValAlarm_Flags() {
  for (AlertSensor objMinMAx : vectAlertSensorObj) {
    if (!Show_Flag) {
      if (!is_ready()) {
        power_up();
      }
      waitForDeviceIsReady();
      if (is_ready()) {
        objMinMAx.Check_MinMaxValAlarm(get_units());
      }
      power_down();
    }
  }
}


// Auxiary
void _GenericSensor::waitForDeviceIsReady() const {

  unsigned long time = millis();
  while (!(is_ready()) && (millis() < (time + IsReady_TimeOut)));
}

boolean  _GenericSensor::get_Show_Flag() const {
  return this->Show_Flag;
}

void _GenericSensor::set_Show_Flag(boolean OnOff) {
  if ( OnOff && !Show_Flag) {
    Show_Flag = HIGH;
    power_up ();
  } else if (!OnOff && Show_Flag) {
    Show_Flag = LOW;
    power_down ();
  }
}

void _GenericSensor::set_currentDev(byte device) {
  //this->currentDev = (device >= numOfDevices) ? 0 : device;
  currentDev &= 0b11110000;
  currentDev |= constrain(device, 0, (_GenericSensor::get_numOfInstances() - 1));
}

byte _GenericSensor::get_currentDev() {
  return (currentDev & 0b00001111);
}

byte _GenericSensor::get_numOfInstances() {
  return ((currentDev & 0b11110000) >> 4);
}

void _GenericSensor::set_numOfAverage(byte times) {
  numOfAverage = times;
}

byte _GenericSensor::get_numOfAverage() {
  return numOfAverage;
}
