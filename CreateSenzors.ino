#include <iostream>
#include "HX711MultiRead.h"
#include "freertos/timers.h"

typedef struct HX711_pins {
  byte DOUT;      // HX711.DOUT   - pin (4) #A2 #A0 #A4
  byte PD_SCK;    // HX711.PD_SCK - pin (3) #A3 #A1 #A5
} HX711_Pins;

const HX711_Pins HX711s_pins[] PROGMEM = {
  { 26, 25},  //pin2 - Data, pin 3 - Hodiny => on SensoShield 5.0
  { 17, 16},  //pin4 - Data, pin 5 - Hodiny => on SensoShield 5.0
  { 27, 14}   //pin6 - Data, pin 7 - Hodiny => on SensoShield 5.0
};

#define numOfHX711Cells            (sizeof(HX711s_pins)/sizeof(HX711s_pins[0]))    //number of using cells withOut LCD Display can be this value 4 with Display is 4 max


TimerHandle_t tmr;
int id = 1;
int interval = 10;
std::vector<_GenericSensor*> vSensors;

_GenericSensor* aSensors[numOfHX711Cells];

//HX711MultiRead l(26, 25), m(17, 16), n(27, 14);

void setup() {
  Serial.begin(115200);
  for (byte i = 0; i < numOfHX711Cells; i++) {
    std::cout << "DOUT\t" << (int)HX711s_pins[i].DOUT << "\tPD_SCK\t" << (int)HX711s_pins[i].PD_SCK  << std::endl;
    vSensors.push_back(new HX711MultiRead(HX711s_pins[i].DOUT, HX711s_pins[i].PD_SCK));
    std::cout << "New size: " << vSensors.size() << std::endl;
  }
  for (_GenericSensor* sensor : vSensors) {
    std::cout << "\t" << sensor << std::endl;
  }
  std::cout <<  std::endl;
 
    for (byte i = 0; i < numOfHX711Cells; i++) {

      //      aSensors[firstHX711Dev + i] = new HX711GenSensor (pgm_read_byte_near(&HX711s_pins[i].DOUT), pgm_read_byte_near(&HX711s_pins[i].PD_SCK), 32); // parameter "gain" is ommited; the default value 128 is used by the library
      aSensors[i] = new HX711MultiRead (pgm_read_byte_near(&HX711s_pins[i].DOUT), pgm_read_byte_near(&HX711s_pins[i].PD_SCK));     // parameter "gain" is ommited; the default value 128 is used by the library
    }
 
/*
  for (int i = 0; i < HX711MultiRead::getAllObjects().size(); i++) {
    //Serial.print (F("0x")); Serial.print ((uint32_t)HX711MultiRead::getAllObjects()[i],HEX);
    std::cout << "\t" << HX711MultiRead::getAllObjects()[i] << std::endl;
    HX711MultiRead::getAllObjects()[i]->set_gain(128);
  }
*/
  //  m.set_gain(128);
  //  std::cout << m.read() << std::endl;

  tmr = xTimerCreate("MyTimer", interval, pdTRUE, ( void * )id, vTimerCallback);
  if ( xTimerStart(tmr, 1 ) != pdPASS ) {
    printf("Timer start error");
  }
}

void vTimerCallback( TimerHandle_t pxTimer ) {
  //  std::cout << "Timer vyvolal CalBack funkci" << std::endl;
  HX711MultiRead::readAll();

  if ( xTimerStart(tmr, 1 ) != pdPASS ) {
    printf("Timer start error");
  }

}


#define INTERVAL 1000
int count = 0;
uint32_t myTime = millis() + INTERVAL;

//bool isAllAalDsIsActual() {
//  bool outputVal = HIGH;
//  for (byte i = 0; i < numOfHX711Cells; i++) {
//    if (!aSensors[i]->get_valDsIsActual()) {
//      outputVal = LOW;
//      break;
//    }
//  }
//  return outputVal;
//}

void loop() {
  while (myTime > millis()) {
    if (HX711MultiRead::get_allValDsIsActual()) {
            for (byte i = 0; i < numOfHX711Cells; i++) {
              std::cout << aSensors[i]->read() << "\t";
            }
      /*
      for (_GenericSensor* sensor : vSensors) {
        std::cout << sensor->read() << "\t";
      }*/
      std::cout << std::endl;
      count++;
    }
  }
  Serial.print("count = "); Serial.println(count);
        delay(1000);
  myTime = millis() + INTERVAL;
  count = 0;
}
