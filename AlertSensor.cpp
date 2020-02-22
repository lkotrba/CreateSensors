/* ========================================================================== */
/*                                                                            */
/*   AlertSensor.cpp                                                          */
/*   Created on 08. February 2020                                             */
/*   (c) Athor Ladislav Kotrba                                                */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

#include "AlertSensor.h"


word AlertSensor::millisToOneTick = 0;        //time when will change "timeInAlarm" variable
boolean isAnyAllarm_Flag = 0;                 //Allarm some device is active
unsigned long countMillisDevWithoutAlarm = 0; //time of one tick "timeInAlarm" variable. Zero value means that no time is recorded

// Alarm metods
AlertSensor::AlertSensor(float MinVal, float MaxVal ) {
  this->MinValue = MinVal;
  this->MaxValue = MaxVal;

  MinValAlarm_Flag = LOW;
  MaxValAlarm_Flag = LOW;

  countMillisDevInAlarm = millis();
}

// Min
void AlertSensor::set_MinValue(float MinValue) {
  this->MinValue = MinValue;
}

float AlertSensor::get_MinValue() const {
  return this->MinValue;
}

void  AlertSensor::set_MinValAlarm_Flag(boolean MVA_flag) {
  this->MinValAlarm_Flag = MVA_flag;
}

boolean  AlertSensor::get_MinValAlarm_Flag() const {
  return this->MinValAlarm_Flag;
}

void AlertSensor::set_MinValReal(float MinValReal) {
  this->MinValReal = MinValReal;
}

float AlertSensor::get_MinValReal() const {
  return  this->MinValReal;
}

// Max
void AlertSensor::set_MaxValue(float MaxValue) {
  this->MaxValue = MaxValue;
}

float AlertSensor::get_MaxValue() const {
  return this->MaxValue;
}

void  AlertSensor::set_MaxValAlarm_Flag(boolean MVA_flag) {
  this->MaxValAlarm_Flag = MVA_flag;
}

boolean  AlertSensor::get_MaxValAlarm_Flag() const {
  return this->MaxValAlarm_Flag;
}

void AlertSensor::set_MaxValReal(float MaxValReal) {
  this->MaxValReal = MaxValReal;
}

float AlertSensor::get_MaxValReal() const {
  return  this->MaxValReal;
}
// Alarm metod calculate
// Private metod
void  AlertSensor::Check_MinMaxValAlarm(float actualValue) {
  boolean minFlag = LOW, maxFlag = LOW;
  if ((actualValue <= MinValue) && ((MinValue >= 1E-9) || (MinValue <= -1E-9))) { //Check MinVal underground
    // set min alarm
    minFlag = HIGH;
    if (actualValue <= MinValReal) {        //MinValue is overload
      MinValReal =  actualValue;
    }
  } else if ((actualValue >= MaxValue) && ((MaxValue >= 1E-9) || (MaxValue <= -1E-9))) { //Check MaxVal overflow
    // set max alarm
    maxFlag = HIGH;
    if (actualValue >= MaxValReal) {         //MaxValue is overload
      MaxValReal =  actualValue;
    }
  }
  MinValAlarm_Flag = minFlag;
  MaxValAlarm_Flag = maxFlag;
  if (millisToOneTick != 0) {
    if (get_MinMaxValAlarm_Flag()) {
      if ((countMillisDevInAlarm + millisToOneTick) > millis()) {
        allarmTimeOverload = HIGH;
      }
    } else {
      countMillisDevInAlarm = millis();
    }
  }
}

boolean  AlertSensor::get_MinMaxValAlarm_Flag() const {
  return this->MinValAlarm_Flag || this->MaxValAlarm_Flag;
}
