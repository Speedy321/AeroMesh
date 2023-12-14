#include "imu.h"

bool imu_valid = false;

void initIMU(int i2c_address, int interrupt_pin, int reset_pin){
    Wire.flush();
    Wire.begin(SDA, SCL);
    Wire.setClockStretchLimit(150000);

    String wireStatus = "Wire status: ";
    wireStatus += Wire.status();
    Logger::debug(wireStatus);
    Wire.beginTransmission(i2c_address);
    String wireError = "Wire Error: ";
    wireError += Wire.endTransmission();
    Logger::debug(wireError);

    //imu_valid = imu.begin(i2c_address, Wire, interrupt_pin, reset_pin);
    imu_valid = imu.begin(i2c_address, Wire);

    if(!imu_valid) {
        Logger::error("BNO08x not detected at default I2C address.");
    } else {
        String msg = "BNO08x found!, last reset: ";
        msg += resetReasonName(imu.getResetReason());
        Logger::info(msg);
        
        imu_valid = setReports();

        Logger::info("BNO08x configured.");
    }
}

// This task should only be started after check if "imu_valid" is true.
void taskReadIMU(){
    imu.serviceBus();

    // One of these will appear at the very start because of the power on reset.
    // Use getResetReason() for the difference between different resets.
    if (imu.wasReset()) {
        String msg = "BNO08x reset: ";
        msg += resetReasonName(imu.getResetReason());
        Logger::info(msg);
        setReports();  // We'll need to re-enable reports after any reset.
    }

    // Has a new event come in on the Sensor Hub Bus?
    if (imu.getSensorEvent() == true) {

        // is it the correct sensor data we want?
        switch (imu.getSensorEventID()) {
        //standard data
        case SENSOR_REPORTID_ACCELEROMETER:
            //imu.getAccel()
            DataHelper::addAccelData(imu.getAccelX(), imu.getAccelY(), imu.getAccelZ());
            break;
/*
        case SENSOR_REPORTID_GAME_ROTATION_VECTOR:
            //imu.getQuat()
            // TODO: implement if needed
            break;
        case SENSOR_REPORTID_GRAVITY:
            //imu.getGravity()
            // TODO: implement if needed
            break;
*/
        case SENSOR_REPORTID_GYROSCOPE_CALIBRATED:
            //imu.getGyro()
            DataHelper::addGyroData(imu.getGyroX(), imu.getGyroY(), imu.getGyroZ());
            break;
/*
        case SENSOR_REPORTID_GYRO_INTEGRATED_ROTATION_VECTOR:
            //imu.getGyroIntegrated...()
            // TODO: implement if needed
            break;
*/
        case SENSOR_REPORTID_LINEAR_ACCELERATION:
            //imu.getLinAccel()
            DataHelper::addLinAccelData(imu.getLinAccelX(), imu.getLinAccelY(), imu.getLinAccelZ());
            break;
/*      
        case SENSOR_REPORTID_MAGNETIC_FIELD:
            //imu.getMag()
            // TODO: implement if needed
            break;
*/
        case SENSOR_REPORTID_ROTATION_VECTOR:
            //imu.getQuat()
            DataHelper::addRotationData(imu.getQuatI(), imu.getQuatJ(), imu.getQuatK(), imu.getQuatReal());
            break;
/*
        // raw data
        case SENSOR_REPORTID_RAW_ACCELEROMETER:
            // TODO: implement if needed
            break;
        case SENSOR_REPORTID_RAW_GYROSCOPE:
            // TODO: implement if needed
            break;
        case SENSOR_REPORTID_RAW_MAGNETOMETER:
            // TODO: implement if needed
            break;
        case SENSOR_REPORTID_UNCALIBRATED_GYRO:
            // TODO: implement if needed
            break;

        // post processed data
        case SENSOR_REPORTID_PERSONAL_ACTIVITY_CLASSIFIER:
            // TODO: implement if needed
            break;
        case SENSOR_REPORTID_STABILITY_CLASSIFIER:
            // TODO: implement if needed
            break;
        case SENSOR_REPORTID_AR_VR_STABILIZED_GAME_ROTATION_VECTOR:
            // TODO: implement if needed
            break;
        case SENSOR_REPORTID_AR_VR_STABILIZED_ROTATION_VECTOR:
            // TODO: implement if needed
            break;
        case SENSOR_REPORTID_STEP_COUNTER:
            // TODO: implement if needed
            break;
        case SENSOR_REPORTID_TAP_DETECTOR:
            // TODO: implement if needed
            break;
*/
        }
    }
}

bool isIMUValid() {
    return imu_valid;
}

// Here is where you define the sensor outputs you want to receive
bool setReports(void) {
    bool success = false;
    int fail_count = 0;

    /* standard data */
    success = imu.enableAccelerometer();
    if (!success) {
        Logger::warn("Failed to enable Accelerometer.");
        fail_count++;
    }
    //success = imu.enableGameRotationVector();
    //if (!success) Logger::warn("Failed to enable Game Rotation Vector.");
    //success = imu.enableGravity();
    //if (!success) Logger::warn("Failed to enable Gravity.");
    success = imu.enableGyro();
    if (!success){
        Logger::warn("Failed to enable Gyro.");
        fail_count++;
    } 
    //success = imu.enableGyroIntegratedRotationVector();
    //if (!success) Logger::warn("Failed to enable Gyro enabled high rate Rotation Vector.");
    success = imu.enableLinearAccelerometer();
    if (!success) {
        Logger::warn("Failed to enable Linear Accelerometer.");
        fail_count++;
    }
    //success = imu.enableMagnetometer();
    //if (!success) Logger::warn("Failed to enable Magnetometer.");
    success = imu.enableRotationVector();
    if (!success) {
        Logger::warn("Failed to enable Rotation Vector.");
        fail_count++;
    }

    /* raw data */
    //imu.enableRawAccelerometer();
    //imu.enableRawGyro();
    //imu.enableRawMagnetometer();
    //imu.enableUncalibratedGyro();

    /* post processed data */
    //imu.enableActivityClassifier(<timeBetweenReports>, <activities>);
    //imu.enableStabilityClassifier();
    //imu.enableARVRStabilizedGameRotationVector(<timeBetweenReports>);
    //imu.enableARVRStabilizedRotationVector(<timeBetweenReports>);
    //imu.enableStepCounter();
    //imu.enableTapDetector(<timeBetweenReports>);

    /* others */
    //imu.enableReport(<args>);
    //imu.enableDebugging();

    if (fail_count > 0) return false;
    else return true;
}

//Given a number between 0 and 5, print the name of the reset reason
//1 = POR, 2 = Internal reset, 3 = Watchdog, 4 = External reset, 5 = Other
String resetReasonName(byte resetReasonNumber)
{
    switch (resetReasonNumber) {
    case 1:
        return "POR";
    case 2:
        return "Internal reset";
    case 3:
        return "Watchdog";
    case 4:
        return "External reset";
    default:
        return "Other";
    }
}

void resetIMU(void){
    if (imu.softReset() == false){
        imu.hardwareReset();
    }
}