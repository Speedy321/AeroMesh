#ifndef AEROMESH_MESSAGES_HELPER
#define AEROMESH_MESSAGES_HELPER
#include "painlessMesh.h"

/*
Message formats are NMEA inspired string objects

Base template is 

<msg_type>;<msg_subtype|0>;<msg_data>

-- Status --
status;std;<bat_%>;<bat_volt>
status;err;<err_no>;<err_msg>
status;ack;<ack_no>;<acc_msg|0>

-- Commands --
// These commands should generally be adressed directly at devices. 
// Broadcasting them has high risk of causing unintended actions on the mesh.
comm;info
comm;restart
comm;changemesh;<new_mesh_name>;<new_mesh_passwd>;<permanent[1|0]>
comm;chname;<new_name>;<permanent[1|0]>

-- Info --
info;0;<name>;<ID>;<mesh_name>

-- Data --
- GPS -
gps;pos;<long>;<long_dir>;<lat>;<lat_dir>
gps;spd;<spd>;<spd_unit>;<dir>;<dir_unit>

- IMU -
imu;acc;<x>;<y>;<z>;<unit>
imu;linacc;<x>;<y>;<z>;<unit>
imu;rot;<w>;<x>;<y>;<z> 
imu;gyro;<x>;<y>;<z>;<unit>

- Strenght/Load -
load;0;<val>;<unit>

*/

void send_cmd_status_std(painlessMesh* mesh, float bat_pct, float bat_volt){
    char *c_msg = new char[100];
    sprintf(c_msg, "status;std;%f;%f", bat_pct, bat_volt);
    mesh->sendBroadcast(c_msg);
    delete[] c_msg;
}
void send_cmd_status_err(painlessMesh* mesh, int8_t err_no, const char* err_msg){
    char *c_msg = new char[strlen(err_msg) + 20];
    sprintf(c_msg, "status;err;%i;%s", err_no, err_msg);
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

void send_gps_pos(painlessMesh* mesh, float longitude, char long_pos, float lattitude, char lat_pos){
    char *c_msg = new char[100];

    sprintf(c_msg, "gps;pos;%f;%c;%f;%c", longitude, long_pos, lattitude, lat_pos);
    mesh->sendBroadcast(c_msg);
    delete[] c_msg;
}
void send_gps_spd(painlessMesh* mesh, float spd, const char* spd_units, float dir, const char* dir_units){
    char *c_msg = new char[100 + strlen(spd_units) + strlen(dir_units)];

    sprintf(c_msg, "gps;spd;%f;%s;%f;%s", spd, spd_units, dir, dir_units);
    mesh->sendBroadcast(c_msg);
    delete[] c_msg;
}

void send_imu_accel(painlessMesh* mesh, float x, float y, float z, const char* units){
    char *c_msg = new char[100 + strlen(units)];

    sprintf(c_msg, "imu;acc;%f;%f;%f;%s", x, y, z, units);
    mesh->sendBroadcast(c_msg);
    delete[] c_msg;
}
void send_imu_lin_accel(painlessMesh* mesh, float x, float y, float z, const char* units){
    char *c_msg = new char[100 + strlen(units)];

    sprintf(c_msg, "imu;linacc;%f;%f;%f;%s", x, y, z, units);
    mesh->sendBroadcast(c_msg);
    delete[] c_msg;
}
void send_imu_rotation(painlessMesh* mesh, float w, float x, float y, float z){
    char *c_msg = new char[100];

    sprintf(c_msg, "imu;rot;%f;%f;%f;%f", w, x, y, z);
    mesh->sendBroadcast(c_msg);
    delete[] c_msg;
}
void send_imu_gyro(painlessMesh* mesh, float x, float y, float z, const char* units){
    char *c_msg = new char[100 + strlen(units)];

    sprintf(c_msg, "imu;gyro;%f;%f;%f;%s", x, y, z, units);
    mesh->sendBroadcast(c_msg);
    delete[] c_msg;
}

void send_loadcell(painlessMesh* mesh, float val, const char* units){
    char *c_msg = new char[100 + strlen(units)];

    sprintf(c_msg, "load;0;%f;%s", val, units);
    mesh->sendBroadcast(c_msg);
    delete[] c_msg;
}

#endif