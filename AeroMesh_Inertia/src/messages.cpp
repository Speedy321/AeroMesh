#include "messages.h"

void send_cmd_status_std(painlessMesh* mesh, Data::Status data){
    char *c_msg = new char[100];
    sprintf(c_msg, "status;std;%f;%f", data.bat_pct, data.bat_volt);
    mesh->sendBroadcast(c_msg);
    delete[] c_msg;
}
void send_cmd_status_err(painlessMesh* mesh, int8_t err_no, const char* err_msg){
    char *c_msg = new char[strlen(err_msg) + 20];
    sprintf(c_msg, "status;err;%i;%s", err_no, err_msg);
    mesh->sendBroadcast(c_msg);
    delete[] c_msg;
}
void send_cmd_status_info(painlessMesh* mesh, const char* err_msg){
    char *c_msg = new char[strlen(err_msg) + 20];
    sprintf(c_msg, "status;info;%s", err_msg);
    mesh->sendBroadcast(c_msg);
    delete[] c_msg;
}
void send_cmd_status_ack(painlessMesh* mesh, int8_t ack_no, const char* ack_msg){
    char *c_msg = new char[strlen(ack_msg) + 20];
    sprintf(c_msg, "status;ack;%i;%s", ack_no, ack_msg);
    mesh->sendBroadcast(c_msg);
    delete[] c_msg;
}

void send_cmd_command_info(painlessMesh* mesh){
    // THIS IS FOR DEBUG ONLY! Do not broadcast this command unless you know what you're doing.
    mesh->sendBroadcast("comm;info");
}
void send_cmd_command_restart(painlessMesh* mesh){
    // THIS IS FOR DEBUG ONLY! Do not broadcast this command unless you know what you're doing.
    mesh->sendBroadcast("comm;restart");
}
void send_cmd_command_change_mesh(painlessMesh* mesh, const char* new_mesh_name, const char* new_mesh_pass, bool permanent){
    // THIS IS FOR DEBUG ONLY! Do not broadcast this command unless you know what you're doing.
    char *c_msg = new char[strlen(new_mesh_name) + strlen(new_mesh_pass) + 25];
    sprintf(c_msg, "comm;changemesh;%s;%s;%i", new_mesh_name, new_mesh_pass, (int)permanent);
    mesh->sendBroadcast(String(c_msg));
    delete[] c_msg;
}
void send_cmd_command_change_name(painlessMesh* mesh, const char* new_name, bool permanent){
    // THIS IS FOR DEBUG ONLY! Do not broadcast this command unless you know what you're doing.
    char *c_msg = new char[strlen(new_name) + 25];
    sprintf(c_msg, "comm;chname;%s;%i", new_name, (int)permanent);
    mesh->sendBroadcast(String(c_msg));
    delete[] c_msg;
}

void send_cmd_info(painlessMesh* mesh, const char* name, const char* ssid){
    uint32_t id = mesh->getNodeId();
    char *c_msg = new char[strlen(name) + strlen(ssid) + 25];

    sprintf(c_msg, "info;0;%s;%u;%s", name, id, ssid);
    mesh->sendBroadcast(c_msg);
    delete[] c_msg;
}

void send_gps_pos(painlessMesh* mesh, Data::GPSPosition data){
    char *c_msg = new char[100];

    sprintf(c_msg, "gps;pos;%f;%c;%f;%c", data.longitude, data.long_position, data.latitude, data.lat_position);
    mesh->sendBroadcast(c_msg);
    delete[] c_msg;
}
void send_gps_spd(painlessMesh* mesh, Data::GPSSpeed data){
    char *c_msg = new char[100 + strlen(data.speed_unit) + strlen(data.dir_unit)];

    sprintf(c_msg, "gps;spd;%f;%s;%f;%s", data.speed, data.speed_unit, data.direction, data.dir_unit);
    mesh->sendBroadcast(c_msg);
    delete[] c_msg;
}

void send_imu_accel(painlessMesh* mesh, Data::Acceleration data){
    char *c_msg = new char[100];

    sprintf(c_msg, "imu;acc;%f;%f;%f;m/s^2", data.x, data.y, data.z);
    mesh->sendBroadcast(c_msg);
    delete[] c_msg;
}
void send_imu_lin_accel(painlessMesh* mesh, Data::Acceleration data){
    char *c_msg = new char[100];

    sprintf(c_msg, "imu;linacc;%f;%f;%f;m/s^2", data.x, data.y, data.z);
    mesh->sendBroadcast(c_msg);
    delete[] c_msg;
}
void send_imu_rotation(painlessMesh* mesh, Data::Rotation data){
    char *c_msg = new char[100];

    sprintf(c_msg, "imu;rot;%f;%f;%f;%f", data.i, data.j, data.k, data.real);
    mesh->sendBroadcast(c_msg);
    delete[] c_msg;
}
void send_imu_gyro(painlessMesh* mesh, Data::Gyro data){
    char *c_msg = new char[100];

    sprintf(c_msg, "imu;gyro;%f;%f;%f;rad/s", data.x, data.y, data.z);
    mesh->sendBroadcast(c_msg);
    delete[] c_msg;
}

void send_loadcell(painlessMesh* mesh, Data::Load data){
    char *c_msg = new char[100];

    sprintf(c_msg, "load;0;%f;kg", data.value);
    mesh->sendBroadcast(c_msg);
    delete[] c_msg;
}