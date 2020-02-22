/* ========================================================================== */
/*                                                                            */
/*   _GenericSensor.h                                                         */
/*   Created on 24. �ervna 2018                                               */
/*   (c) Athor Ladislav Kotrba                                                */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

#ifndef _GENERICSENSOR_H
#define _GENERICSENSOR_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <vector>
#include "AlertSensor.h"
#include "KF_Filter.h"

#define IsReady_TimeOut 1000

//spolecny predek pro vsechny senzory

class _GenericSensor {
  private:
    float OFFSET;	    // used for tare weight
    float SCALE;	    // used to return weight in grams, kg, ounces, whatever

    static byte numOfAverage; // //default times to read average

    //  Alarm variables #### Vector for alarm objects store
    //Array to store all objects of class AlertSensor
    std::vector<AlertSensor> vectAlertSensorObj;

    std::vector<KF_Filter> vectKF_FilterObj;

    // Control variables
    static byte currentDev;   //0bYYYYXXXX,   XXXX = number of actual device YYYY = number of instances this class

  protected:
    // Control variables
    boolean Show_Flag;       //this device is actived => will be displayed and sended throught serial line



  public:

    _GenericSensor();

    virtual ~_GenericSensor() {}

    // waits for the chip to be ready and returns a reading
    virtual int32_t  read        () ; // {return 0;}
    // returns an average reading; times = how many times to read
    long  read_average(byte times = numOfAverage) ;
    // returns (read_average() - OFFSET), that is the current value without the tare weight; times = how many readings to do
    virtual float  get_value (byte times = numOfAverage) ;
    // returns get_value() divided by SCALE, that is the raw value divided by a value obtained via calibration
    // times = how many readings to do
    float get_units   (byte times = numOfAverage) ;
    // set the OFFSET value for tare weight; times = how many times to read the tare value
    void  tare        (byte times = numOfAverage);
    // set the SCALE value; this value is used to convert the raw data to "human readable" data (measure units)
    void  set_scale   (float scale = 1.f);
    // get the current SCALE
    float get_scale   () const ;
    // set OFFSET, the value that's subtracted from the actual reading (tare weight)
    void  set_offset  (float offset = 0.f);
    // get the current OFFSET
    float get_offset  () const ;
    // check if value of sensor is actual
    virtual bool  get_valDsIsActual() const {
      return HIGH;
    }
    // check if sensor is ready
    virtual bool  is_ready    () const {
      return HIGH;
    }
//    // check if all sensor is ready
//    virtual bool  get_allValDsIsActual() const {
//            return HIGH;
//    }
    // Alarm metods
    void addAllarmObject(float, float);
    void addKF_FilterObject(float, float);

    void   calc_ValAlarm_Flags();

    // wait check if sensor is ready
    void waitForDeviceIsReady() const ;

    // set the gain factor; takes effect only in some sensors. HX711 for example
    virtual void  set_gain    (byte ) {}

    // puts the chip into power down mode
    virtual void  power_down  () {}
    // puts the chip into power up mode
    virtual void  power_up    () {}

    boolean  get_Show_Flag() const ;
    void     set_Show_Flag(boolean ) ;


    //
    static void set_currentDev(byte device = 0);

    static byte get_currentDev();

    static byte get_numOfInstances();

    //
    static void set_numOfAverage(byte times = 10);

    static byte get_numOfAverage();




};

#endif /* _GENERICSENSOR_H */
