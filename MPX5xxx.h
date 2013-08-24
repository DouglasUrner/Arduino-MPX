/*
** Driver for the Freescale MPX5xxx family of pressure sensors.
**
** http://www.freescale.com/
*/
#ifndef MPX5xxx_h
#define MPX5xxx_h

#include <Arduino.h>

#include "../Common/DEBUG.h"

#ifndef A2D_FULL_SCALE
#define A2D_FULL_SCALE 1024
#endif // A2D_FULL_SCALE
#ifndef VSS
#define VSS 5.0
#endif // VSS

#define MPX5010 0
#define MPX5100 1
#define MPX5700 2

/*
** For the through hole, dual-port versions (6 in-line pins) the reference side
** of the part is the one on which the device number is printed. Pin one is
** usually on the left (but see the data sheet for some of the other parts it is
** on the right and the surface mount parts have a different pin layout). There
** appears to be a notch in pin one on many (all?) of the packages.
**
** Pin assignments:
** - pin 1: Vout
** - pin 2: GND
** - pin 3: Vss
** - pin 4-6: no connection
**
** Transfer functions and error calcualtions are described in MPX5xxx.cpp.
*/

class MPX5xxx {

 public:
  MPX5xxx(byte devType, byte pin);
  MPX5xxx(byte devType, byte pin, byte refType);
  MPX5xxx(byte devType, byte pin, byte refType, float aRefVolts);
  float autoCalibrate();	// Automatically determine and set offset.
  void  calibrate(float counts);	// Manually set the offset.
  float read();
  float pointAverage(byte samples, int msDelay);
  float rollingAverage(byte samples);
  float error();

 private:
  /*
   * Full scale output voltage is nominally 4.7 V, but it may go as high
   * as 4.925 V. So to support the full range of an MPX5xxx family device
   * an analog reference voltage of 5.0 V is required. However if the
   * application does not require the full range of the device a lower
   * AREF voltage can be used with the benefit of having more resolution
   * in the active range.
   */
  float _aRefVolts;
  byte  _devType;		// Device id, should be enum.
  float _Voffset;		// The voltage offset when a zero reading is expected.
  byte  _pin;
  float _transferConstant;	// Device specific constant for transfer function.

  float convert(float reading);
  float supplyVoltage();	// Get the value of Vss
};

#endif // MPX5xxx_h
