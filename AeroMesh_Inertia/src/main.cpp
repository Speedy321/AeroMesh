#include "logging.h"
#include "painlessMesh.h"
#include "secrets.h"
#include "imu.h"
#include "messages.h"
#include "data.h"

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

void sendMessages() {

  Data::Acceleration accelData = DataHelper::getAccelData();
  Data::Gyro gyroData = DataHelper::getGyroData();
  Data::Acceleration linAccelData = DataHelper::getlinAccelData();
  Data::Rotation rotData = DataHelper::getRotationData();

  Data::Status statusData = DataHelper::getStatusData();

  send_cmd_status_std(&mesh, statusData);
  if(isIMUValid()){
    send_imu_accel(&mesh, accelData);
    send_imu_lin_accel(&mesh, linAccelData);
    send_imu_rotation(&mesh, rotData);
    send_imu_gyro(&mesh, gyroData);
  }
}

Task sendMessageTask(TASK_MILLISECOND * 1000, TASK_FOREVER, &sendMessages);
Task readIMUTask(TASK_MILLISECOND * 10, TASK_FOREVER, &taskReadIMU);

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  String ms = "src:" + from;
  ms += " msg:";
  ms += msg;
  Logger::debug(ms);

  // TODO: answer commands

  /*
  send_cmd_status_err(&mesh, 123, "Test!");
  send_cmd_status_ack(&mesh, 123, "Ack test!");

  send_cmd_command_info(&mesh);
  send_cmd_command_restart(&mesh);
  send_cmd_command_change_mesh(&mesh, "no_mesh", "no_pass", false);
  send_cmd_command_change_name(&mesh, "new_name", true);

  send_cmd_info(&mesh, "DebugNode", MESH_PREFIX);
  */
  //Serial.printf("src:%u msg:%s\n", millis(), from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
  String msg = "New node: ";
  msg += nodeId;
  Logger::debug(msg);
  //Serial.printf("newNode:%u\n", nodeId);
}

void changedConnectionCallback() {
  String msg = "Connection Changed";
  Logger::debug(msg);
  //Serial.printf("connChanged\n"); //TODO: add status infos
}

void setup() {
  Serial.begin(115200);
  Logger::set_level(LoggingLevel::DEBUG);

  initIMU(BNO08X_ADDR, BNO08X_INT, BNO08X_RST);

  //mesh.setDebugMsgTypes( painlessmesh::logger::LogLevel::ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( painlessmesh::logger::LogLevel::ERROR | STARTUP | MESH_STATUS | CONNECTION);  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);

  send_cmd_status_info(&mesh, "Inertia sensor started.");

  if(isIMUValid()){
    userScheduler.addTask( readIMUTask );
    readIMUTask.enable();
  } else {
    Logger::error("Not starting IMU task.");
  }
  userScheduler.addTask( sendMessageTask );
  sendMessageTask.enable();
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
}