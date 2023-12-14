#ifndef AEROMESH_MESSAGES_HELPER
#define AEROMESH_MESSAGES_HELPER
#include "painlessMesh.h"
#include "data.h"

/*
Message formats are NMEA inspired string objects

Base template is 

<msg_type>;<msg_subtype|0>;<msg_data>

-- Status --
status;std;<bat_%>;<bat_volt>
status;err;<err_no>;<err_msg>
status;info;<msg>
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

void send_cmd_status_std(painlessMesh* mesh, Data::Status data);
void send_cmd_status_err(painlessMesh* mesh, int8_t err_no, const char* err_msg);
void send_cmd_status_info(painlessMesh* mesh, const char* err_msg);
void send_cmd_status_ack(painlessMesh* mesh, int8_t ack_no, const char* ack_msg);

void send_cmd_command_info(painlessMesh* mesh);
void send_cmd_command_restart(painlessMesh* mesh);
void send_cmd_command_change_mesh(painlessMesh* mesh, const char* new_mesh_name, const char* new_mesh_pass, bool permanent);
void send_cmd_command_change_name(painlessMesh* mesh, const char* new_name, bool permanent);

void send_cmd_info(painlessMesh* mesh, const char* name, const char* ssid);

void send_gps_pos(painlessMesh* mesh, Data::GPSPosition data);
void send_gps_spd(painlessMesh* mesh, Data::GPSSpeed data);

void send_imu_accel(painlessMesh* mesh, Data::Acceleration data);
void send_imu_lin_accel(painlessMesh* mesh, Data::Acceleration data);
void send_imu_rotation(painlessMesh* mesh, Data::Rotation data);
void send_imu_gyro(painlessMesh* mesh, Data::Gyro data);

void send_loadcell(painlessMesh* mesh, Data::Load data);

#endif