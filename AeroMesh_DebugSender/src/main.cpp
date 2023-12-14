#include "painlessMesh.h"
#include "secrets.h"
#include "messages.h"

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

void sendMessages() {
  send_cmd_status_std(&mesh, 0.45f, 3.725f);
  send_cmd_status_err(&mesh, 123, "Test!");
  send_cmd_status_ack(&mesh, 123, "Ack test!");

  send_cmd_command_info(&mesh);
  send_cmd_command_restart(&mesh);
  send_cmd_command_change_mesh(&mesh, "no_mesh", "no_pass", false);
  send_cmd_command_change_name(&mesh, "new_name", true);

  send_cmd_info(&mesh, "DebugNode", MESH_PREFIX);
  
  send_gps_pos(&mesh, 1.2f, 'N', 458.4f, 'E');
  send_gps_spd(&mesh, 12.5f, "kts", 182.45f, "deg");

  send_imu_accel(&mesh, 0.1f, 0.2f, 0.3f, "m/s^2");
  send_imu_lin_accel(&mesh, 0.11f, 0.22f, 0.33f, "m/s^2");
  send_imu_rotation(&mesh, 0.111f, 0.222f, 0.333f, 0.444f);
  send_imu_gyro(&mesh, 0.1111f, 0.2222f, 0.3333f, "rad/s");

  send_loadcell(&mesh, 1254.0f, "kg");

  String data = "";
  for(int i = 0; i < 60; i++){
    data += i;
    mesh.sendBroadcast(data);
  }
}

Task sendMessageTask( TASK_SECOND * 30 , TASK_FOREVER, &sendMessages);


// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("src:%u msg:%s\n", millis(), from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("newNode:%u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("connChanged\n"); //TODO: add status infos
}

void setup() {
  Serial.begin(115200);

  mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  //mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);

  userScheduler.addTask( sendMessageTask );
  sendMessageTask.enable();
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
}