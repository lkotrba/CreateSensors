/* ========================================================================== */
/*                                                                            */
/*   HX711MultiRead.h                                                         */
/*   Created on 28. January 2020                                              */
/*   (c) Athor Ladislav Kotrba                                                */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */
#define SCK_DISABLE_INTERRUPTS (1)
#define SCK_DELAY (1)
#define SCK_TIMEOUT (2000)

#ifndef _HX711MULTIREAD_H
#define _HX711MULTIREAD_H 1

#include <vector>
#include "_GenericSensor.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class HX711MultiRead : public _GenericSensor{
  private:
    static std::vector<HX711MultiRead*> PointersToObjects;   //Array to store all objects of class HX711MultiRead

    byte  PD_SCK;	// Power Down and Serial Clock Input Pin
    byte  DOUT;		// Serial Data Output Pin
    byte  GAIN;		// amplification factor

    int32_t  dataSample; // data set, make if interrupt is used, make it volatile
    bool valDsIsActual;
    bool cellIsPowerOn;
    
  public:
    // define clock and data pin, channel, and gain factor
    // channel selection is made by passing the appropriate gain: 128 or 64 for channel A, 32 for channel B
    // gain: 128 or 64 for channel A; channel B works with 32 gain factor only
    HX711MultiRead(byte dout, byte pd_sck, byte gain = 128);

    virtual ~HX711MultiRead();

    // set the gain factor; takes effect only after a call to read()
    // channel A can be set for a 128 or 64 gain; channel B has a fixed 32 gain
    // depending on the parameter, the channel is also set to either A or B
    void set_gain(byte gain = 128);
    bool is_ready();

    //Passes pointer arrays on all  objects of class HX711MultiRead
    static std::vector<HX711MultiRead*> getAllObjects();
    int32_t read();
    static void readAll();

    bool get_valDsIsActual() const ;

    static bool get_allValDsIsActual() ;
    
    bool waitReady(unsigned long delay_ms = 0);

    void powerDown();
    void powerUp();
};

#endif /* _HX711MULTIREAD_H */
