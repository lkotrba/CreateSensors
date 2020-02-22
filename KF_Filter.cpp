/* ========================================================================== */
/*                                                                            */
/*   KF_Filter.cpp                                                            */
/*   Created on 08. February 2020                                             */
/*   (c) Athor Ladislav Kotrba                                                */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

#include "KF_Filter.h"


KF_Filter::KF_Filter() {
  sum = 0;
}

void KF_Filter::setTimes(byte times) {
  times = times;
  //   Serial.print(F("times = ")); Serial.println(times);
}

byte KF_Filter::getTimes(void) {
  return times;
}
void KF_Filter::setMaxDivergence(word times) {
  maxDivergence = times;
}

word KF_Filter::getMaxDivergence(void) {
  return maxDivergence;
}

float KF_Filter::Update(float immediateValueOfRead, float SCALE) {
  if (times) {  // (times != 0) => Kalman filter is ON
    float averageVal =  sum / times;
    float immediateDivergence = abs(immediateValueOfRead - averageVal);  //immediateDivergence is in physical units
    //Serial.print (F("\t immediateDivergence: ")); Serial.print (immediateDivergence); Serial.print (F("\t maxDivergence: ")); Serial.println (maxDivergence);
    if (immediateDivergence > static_cast<float> (maxDivergence / SCALE)) {
      sum = immediateValueOfRead * times;
    } else {
      sum += immediateValueOfRead - averageVal;
      immediateValueOfRead = averageVal;
    }
    //Serial.print (F("\t immediateValueOfRead: ")); Serial.print (immediateValueOfRead);
  }
  //Serial.print (F("\t immediateValueOfRead: ")); Serial.print (immediateValueOfRead);
  //T    Check_MinMaxValAlarm(immediateValueOfRead);
  return (immediateValueOfRead);
}
