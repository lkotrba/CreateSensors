/* ========================================================================== */
/*                                                                            */
/*   KF_Filter.h                                                              */
/*   Created on 08. February 2020                                             */
/*   (c) Athor Ladislav Kotrba                                                */
/*   Description: class for implementation Kalman filter                      */
/*                                                                            */
/* ========================================================================== */

#ifndef _KF_FILTER_H
#define _KF_FILTER_H 1

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class KF_Filter {
  private:

    //    boolean KalmanFilter_OnOff;
    float sum;             //Sum of Kalman fiter samples. Used for average counting
    byte  times;            //Number samples for Kalman filter average
    word  maxDivergence;    //Max value in RAW units when KF_sum is reset by readUnits() value.

  public:

    KF_Filter();

    virtual ~KF_Filter() {}

    // setTimes value. Set number samples for average kalman filter
    void setTimes( byte times = 10);

    // getTimes value. Get number samples for average kalman filter
    byte getTimes();

    // setMaxDivergence value. Set max value in RAW units. When Read() is more than AVERAGE then KF_sum = read().
    void setMaxDivergence( word times = 100);

    // getMaxDivergence value. Get max value in RAW units. When Read() is more than AVERAGE then KF_sum = read().
    word getMaxDivergence();

    float Update(float, float);
};


#endif /* _ALERTSENSOR_H */
