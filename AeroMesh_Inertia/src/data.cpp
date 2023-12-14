#include "data.h"

DataList<Data::Vec3> DataHelper::accelList;
DataList<Data::Vec3> DataHelper::gyroList;
DataList<Data::Vec3> DataHelper::linAccelList;
DataList<Data::Quaternion> DataHelper::rotList;

void DataHelper::addAccelData(float x, float y, float z) {
    addData(&accelList, x, y, z);
}

void DataHelper::addGyroData(float x, float y, float z) {
    addData(&gyroList, x, y, z);
}

void DataHelper::addLinAccelData(float x, float y, float z) {
    addData(&linAccelList, x, y, z);
}

void DataHelper::addRotationData(float i, float j, float k, float real) {
    addData(&rotList, i, j, k, real);
}

Data::Vec3 DataHelper::getAccelData() {
    return getMesureAvg(&accelList);
}

Data::Vec3 DataHelper::getGyroData() {
    return getMesureAvg(&gyroList);
}

Data::Vec3 DataHelper::getlinAccelData() {
    return getMesureAvg(&linAccelList);
}

Data::Quaternion DataHelper::getRotationData() {
    return getMesureAvg(&rotList);
}

Data::Status DataHelper::getStatusData() {
    // TODO: read battery properly
    Data::Status status = {0.0f, 0.0f};
    return status;
}

void DataHelper::addData(DataList<Data::Vec3> *list, float x, float y, float z) {
    list->list[list->pos].x = x;
    list->list[list->pos].y = y;
    list->list[list->pos].z = z;

    list->pos += 1;
    if (list->pos >= DATALISTMAX){ list->pos = 0; }
}

void DataHelper::addData(DataList<Data::Quaternion> *list, float i, float j, float k, float real) {
    list->list[list->pos].i = i;
    list->list[list->pos].j = j;
    list->list[list->pos].k = k;
    list->list[list->pos].real = real;

    list->pos += 1;
    if (list->pos >= DATALISTMAX){ list->pos = 0; }
}

Data::Vec3 DataHelper::getMesureAvg(DataList<Data::Vec3> *dataList) {    
    Data::Vec3 data = {0, 0, 0};
    // sum
    for (int i = 0; i < DATALISTMAX; i++){
        data.x += dataList->list[i].x;
        data.y += dataList->list[i].y;
        data.z += dataList->list[i].z;
    }

    // avg
    data.x = data.x/DATALISTMAX;
    data.y = data.y/DATALISTMAX;
    data.z = data.z/DATALISTMAX;

    return data;
}

Data::Quaternion DataHelper::getMesureAvg(DataList<Data::Quaternion> *dataList) {
    Data::Quaternion data = {0, 0, 0, 0};
    // sum
    for (int i = 0; i < DATALISTMAX; i++){
        data.i += dataList->list[i].i;
        data.j += dataList->list[i].j;
        data.k += dataList->list[i].k;
        data.real += dataList->list[i].real;
    }

    // avg
    data.i = data.i/DATALISTMAX;
    data.j = data.j/DATALISTMAX;
    data.k = data.k/DATALISTMAX;
    data.real = data.real/DATALISTMAX;

    return data;
}