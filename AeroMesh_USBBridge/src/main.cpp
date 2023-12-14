#include "painlessMesh.h"
#include "secrets.h"

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

void sendMessage(String &msg, uint32_t dst = 0) {
  Serial.printf("src:%u dst:%u, msg:%s\n", mesh.getNodeId(), dst, msg.c_str());
  if (dst > 0) {
    mesh.sendSingle(dst, msg);
  } else {
    mesh.sendBroadcast(msg);
  }
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("src:%u msg:%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("newNode:%u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("connChanged\n"); //TODO: add status infos
}

void setup() {
  Serial.begin(115200);

  //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
}