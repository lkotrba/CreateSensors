/* ========================================================================== */
/*                                                                            */
/*   AlertSensor.h                                                            */
/*   Created on 08. February 2020                                             */
/*   (c) Athor Ladislav Kotrba                                                */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

#ifndef _ALERTSENSOR_H
#define _ALERTSENSOR_H 1

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class AlertSensor {
  private:
    //  Alarm variables
    //  Min
    float MinValue;           //If value in readUnits is less than this value => set min alarm flag
    //When is this walue = (-1E-9 < value < 1E-9) => alarm signal is OFF
    float MinValReal;         //Actual exceedance of the minimum value

    boolean MinValAlarm_Flag; //LOW => no Allarm, HIGH => is allarm
    //  Max
    float MaxValue;           //If value in readUnits is more than this value => set max alarm flag
    //When is this walue = (-1E-9 < value < 1E-9) => alarm signal is OFF
    float MaxValReal;         //Actual exceedance of the maximum value

    boolean MaxValAlarm_Flag; //LOW => no Allarm, HIGH => is allarm

    //  Time
    //    unsigned long timeInAlarm; //time which device spend in Alarm. Will be only in flash and local variable

    unsigned long countMillisDevInAlarm; //time when will change "timeInAlarm" variable

    boolean allarmTimeOverload;    //Allarm spend long time flag. We would recorded (increased) it to EEPROM memory

    static word millisToOneTick; //time of one tick "timeInAlarm" variable. Zero value means that no time is recorded

    static boolean isAnyAllarm_Flag; //Allarm some device is active

    static unsigned long countMillisDevWithoutAlarm; //time when will change "timeInAlarm" variable. Time for which the device is working correctly

  public:
    // Alarm metods
    //### Constructor
    AlertSensor(float, float);

    //### Destructor
    virtual ~AlertSensor() {}

    //### Min
    void set_MinValue(float MaxValue = 0.0);

    float get_MinValue() const ;


    void set_MinValAlarm_Flag(boolean MVA_flag = LOW);

    boolean  get_MinValAlarm_Flag() const ;

    //
    void set_MinValReal(float MaxValReal = 0.0);

    float get_MinValReal() const ;
    //#### Max
    void set_MaxValue(float MaxValue = 0.0);

    float get_MaxValue() const ;

    // Internal function for check whether has been alarm value overload which sets MaxValAlarm_Flag
    void Check_MinMaxValAlarm(float);

    void set_MaxValAlarm_Flag(boolean MVA_flag = LOW);

    boolean  get_MaxValAlarm_Flag() const ;

    //
    void set_MaxValReal(float MaxValReal = 0.0);

    float get_MaxValReal() const ;
    //

    boolean  get_MinMaxValAlarm_Flag() const ;
};

#endif /* _ALERTSENSOR_H */
