#ifndef IMU_H
#define IMU_H

#include <SparkFun_BNO08x_Arduino_Library.h>  // CTRL+Click here to get the library: http://librarymanager/All#SparkFun_BNO08x
#include "logging.h"
#include "data.h"

// For reliable interaction with the SHTP bus, we need
// to use hardware reset control, and monitor the H_INT pin
// The H_INT pin will go low when its okay to talk on the SHTP bus.
// Note, these can be other GPIO if you like.
// Do not define (or set to -1) to not user these features.
#define BNO08X_INT  14
//#define BNO08X_INT  -1
//#define BNO08X_RST  16
#define BNO08X_RST  -1

#define BNO08X_ADDR 0x4B  // SparkFun BNO08x Breakout (Qwiic) defaults to 0x4B
//#define BNO08X_ADDR 0x4A // Alternate address if ADR jumper is closed
/*
static const char *radUnit = "rad";
static const char *degUnit = "def";
static const char *msecUnit = "m/s";
static const char *radsecUnit = "rad/s";
static const char *msec2Unit = "m/s^2";
*/

static BNO08x imu;

void initIMU(int i2c_address, int interrupt_pin, int reset_pin);
void taskReadIMU();
bool isIMUValid();

bool setReports(void);
String resetReasonName(byte resetReasonNumber);
void resetIMU(void);

#endif