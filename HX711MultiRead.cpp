/* ========================================================================== */
/*                                                                            */
/*   HX711MultiRead.cpp                                                       */
/*   Created on 28. January 2020                                              */
/*   (c) Athor Ladislav Kotrba                                                */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

#include <iostream>

#include "HX711MultiRead.h"
//#define DEBUG
#include "DEBUG_PRINT.h"

std::vector<HX711MultiRead*> HX711MultiRead::PointersToObjects;

HX711MultiRead::HX711MultiRead(byte dout, byte pd_sck, byte gain) {
  PointersToObjects.push_back(this);
  PD_SCK 	= pd_sck;
  DOUT 	= dout;
  valDsIsActual = LOW;
  cellIsPowerOn = HIGH;

  pinMode(PD_SCK, OUTPUT);
  pinMode(DOUT, INPUT);

  set_gain(gain);
}

HX711MultiRead::~HX711MultiRead() {

}

void HX711MultiRead::set_gain(byte gain) {
  switch (gain) {
    case 128:		// channel A, gain factor 128
      GAIN = 1;
      break;
    case 64:		// channel A, gain factor 64
      GAIN = 3;
      break;
    case 32:		// channel B, gain factor 32
      GAIN = 2;
      break;
  }
  
//  std::cout << "DOUT\t" << (int)DOUT << "\tPD_SCK\t" << (int)PD_SCK  << std::endl;

  digitalWrite(PD_SCK, LOW);
  read();
}

bool HX711MultiRead::is_ready() {
  return digitalRead(DOUT) == LOW;
}


std::vector<HX711MultiRead*> HX711MultiRead::getAllObjects() {
  return PointersToObjects;
}

int32_t HX711MultiRead::read() { //read 24 bit data and start the next conversion
  if (!valDsIsActual) {        // valDsIsActual -> dataSample was actualized by proc ReadAll()
    //                         //!valDsIsActual -> start the next conversion
    Serial.println("Single read");
    digitalWrite(PD_SCK, LOW);
    dataSample = 0;
    if (!waitReady()) {

      if (SCK_DISABLE_INTERRUPTS) {
        noInterrupts();
      }
      for (uint8_t j = 0; j < 24; j++) { //read 24 bit data + set gain and start next conversion
#ifdef SCK_DELAY
        if (SCK_DELAY) delayMicroseconds(1); // could be required for faster mcu's, set value in config.h
#endif
        digitalWrite(PD_SCK, HIGH);
#ifdef SCK_DELAY
        if (SCK_DELAY) delayMicroseconds(1); // could be required for faster mcu's, set value in config.h
#endif
        dataSample = static_cast<int32_t>(static_cast<uint32_t>(dataSample) << 1);
        if (digitalRead(DOUT)) {
          dataSample = static_cast<int32_t>(static_cast<uint32_t>(dataSample) + 1);
        }
        digitalWrite(PD_SCK, LOW);
      }

      for (byte j = 0; j < GAIN; j++) {
#ifdef SCK_DELAY
        if (SCK_DELAY) delayMicroseconds(1); // could be required for faster mcu's, set value in config.h
#endif
        digitalWrite(PD_SCK, HIGH);
#ifdef SCK_DELAY
        if (SCK_DELAY) delayMicroseconds(1); // could be required for faster mcu's, set value in config.h
#endif
        digitalWrite(PD_SCK, LOW);
      }

      if (SCK_DISABLE_INTERRUPTS) {
        interrupts();
      }
      dataSample = static_cast<int32_t>(static_cast<uint32_t>(dataSample) ^ 0x800000); // if out of range (min), change to 0
      return dataSample;
    }
  }
  valDsIsActual = LOW;
  return dataSample;
}

void HX711MultiRead::readAll() { //read 24 bit data and start the next conversion
  for (HX711MultiRead* PTO : PointersToObjects) {
    if (!PTO->is_ready()) {
      return;
    }
  }
  byte numOfObjects = PointersToObjects.size();
  byte gainSet[numOfObjects];

  //  for (auto actualObject: PointersToObjects){
  for (byte i = 0; i < numOfObjects; i++) {
    //    digitalWrite(actualObject->PD_SCK, LOW);
    //    actualObject->dataSample = 0;
    //    gainSet[i] = actualObject->GAIN;
    //    actualObject->waitReady();
    //    i++;

    digitalWrite(PointersToObjects[i]->PD_SCK, LOW);
    PointersToObjects[i]->dataSample = 0;
    gainSet[i] = PointersToObjects[i]->GAIN;
    PointersToObjects[i]->waitReady();
  }
  //  convRslt = 0;
  if (SCK_DISABLE_INTERRUPTS) {
    noInterrupts();
  }
  for (uint8_t j = 0; j < 24; j++) { //read 24 bit data + set gain and start next conversion
#ifdef SCK_DELAY
    if (SCK_DELAY) delayMicroseconds(1); // could be required for faster mcu's, set value in config.h
#endif
    for (byte i = 0; i < numOfObjects; i++) {
      digitalWrite(PointersToObjects[i]->PD_SCK, HIGH);
    }
#ifdef SCK_DELAY
    if (SCK_DELAY) delayMicroseconds(1); // could be required for faster mcu's, set value in config.h
#endif
    for (byte i = 0; i < numOfObjects; i++) {
      PointersToObjects[i]->dataSample = static_cast<int32_t>(static_cast<uint32_t>(PointersToObjects[i]->dataSample) << 1);
      if (digitalRead(PointersToObjects[i]->DOUT))
      {
        PointersToObjects[i]->dataSample = static_cast<int32_t>(static_cast<uint32_t>(PointersToObjects[i]->dataSample) + 1);
      }
    }
    for (byte i = 0; i < numOfObjects; i++) {
      digitalWrite(PointersToObjects[i]->PD_SCK, LOW);
    }
  }
  for (byte j = 0; j < 3; j++) {
    for (byte i = 0; i < numOfObjects; i++) {
      if (gainSet[i] > 0) {
#ifdef SCK_DELAY
        if (SCK_DELAY) delayMicroseconds(1); // could be required for faster mcu's, set value in config.h
#endif
        digitalWrite(PointersToObjects[i]->PD_SCK, HIGH);
#ifdef SCK_DELAY
        if (SCK_DELAY) delayMicroseconds(1); // could be required for faster mcu's, set value in config.h
#endif
        digitalWrite(PointersToObjects[i]->PD_SCK, LOW);
        gainSet[i]--;
      }
    }
  }

  if (SCK_DISABLE_INTERRUPTS) {
    interrupts();
  }
  for (byte i = 0; i < numOfObjects; i++) {
    PointersToObjects[i]->dataSample = static_cast<int32_t>(static_cast<uint32_t>(PointersToObjects[i]->dataSample) ^ 0x800000); // if out of range (min), change to 0
    PointersToObjects[i]->valDsIsActual = HIGH;
  }
}

bool HX711MultiRead::get_valDsIsActual() const {
  return valDsIsActual;
}

bool HX711MultiRead::get_allValDsIsActual() { 
  for (HX711MultiRead* PTO : PointersToObjects) {
    if (!PTO->valDsIsActual) {
      return LOW;
    }
  }
  return HIGH;
}


bool HX711MultiRead::waitReady(unsigned long delay_ms) {
  // Wait for the chip to become ready.
  // This is a blocking implementation and will
  // halt the sketch until a load cell is connected.
  uint32_t time = millis() + SCK_TIMEOUT;
  int count = 0;
  while (!is_ready() && time > millis()) {
    // Probably will do no harm on AVR but will feed the Watchdog Timer (WDT) on ESP.
    // https://github.com/bogde/HX711/issues/73
    delay(delay_ms);
    count++;
  }
  if (time < millis()) {
    powerDown();
    return HIGH;
  } else {
    uint32_t WaitTime = millis() + SCK_TIMEOUT - time;
    //    Serial.print("WaitTime = "); Serial.println(WaitTime);
    //    Serial.print("Wait time = "); Serial.println(millis() + SCK_TIMEOUT - time);
    //    Serial.print("Count = "); Serial.println(count);

    return LOW;
  }
}
void HX711MultiRead::powerDown() {
  digitalWrite(PD_SCK, LOW);
  digitalWrite(PD_SCK, HIGH);
  for (byte i = 0; i < PointersToObjects.size(); i++) {
    if (this == PointersToObjects[i]) {
      HX711MultiRead::PointersToObjects.erase(PointersToObjects.begin() + i);
      break;
    }
  }
  cellIsPowerOn = LOW;
}

void HX711MultiRead::powerUp() {
  digitalWrite(PD_SCK, LOW);
  for (byte i = 0; i < PointersToObjects.size(); i++) {
    if (this == PointersToObjects[i]) return;
  }
  PointersToObjects.push_back(this);
  cellIsPowerOn = HIGH;
}
