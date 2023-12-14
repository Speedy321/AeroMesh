#ifndef DATA_H
#define DATA_H
#include <Arduino.h>

#define DATALISTMAX 10

namespace Data{
    // status data
    typedef struct Status{
        float bat_pct;
        float bat_volt;
    } Status;
    // No need for structs for the other status (error and ack)

    //sensor data
    typedef struct GPSPosition{
        float longitude;
        char long_position;
        float latitude;
        char lat_position;
    } GPSPosition;
    typedef struct GPSSpeed{
        float speed;
        const char *speed_unit;
        float direction;
        const char *dir_unit;
    } GPSSpeed;

    // Used for IMU (linear-)acceleration, gyro, and other x,y,z + units data.
    typedef struct Vec3{
        float x;
        float y;
        float z;
    } Vec3, Acceleration, Gyro;
    // Used for rotation (quaternion)
    typedef struct Quaternion{
        float i;
        float j;
        float k;
        float real;
    } Quaternion, Rotation;

    typedef struct Vec1{
        float value;
    } Vec1, Load;
}

template <typename T> struct DataList {
    T list[DATALISTMAX];
    int pos;
    int lastReadPos;
};

class DataHelper{
public:
    static void addAccelData(float x, float y, float z);
    static void addGyroData(float x, float y, float z);
    static void addLinAccelData(float x, float y, float z);
    static void addRotationData(float i, float j, float k, float real);

    static Data::Vec3 getAccelData();
    static Data::Vec3 getGyroData();
    static Data::Vec3 getlinAccelData();
    static Data::Quaternion getRotationData();

    static Data::Status getStatusData();

private:
    DataHelper();
    static DataList<Data::Vec3> accelList;
    static DataList<Data::Vec3> gyroList;
    static DataList<Data::Vec3> linAccelList;
    static DataList<Data::Quaternion> rotList;

    static void addData(DataList<Data::Vec3> *list, float x, float y, float z);
    static void addData(DataList<Data::Quaternion> *list, float i, float j, float k, float real);
    static Data::Vec3 getMesureAvg(DataList<Data::Vec3> *dataList);
    static Data::Quaternion getMesureAvg(DataList<Data::Quaternion> *dataList);
};

#endif