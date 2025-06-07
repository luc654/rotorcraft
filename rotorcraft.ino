#include <WiFi.h>
// #include <String.h>
#define YELLOW 26
#define WHITE 25

// -------------------- Wifi Setup --------------------
const char* ssid = "rotorcraft";
const char* pass = "password123";

WiFiServer server(80);
String header;
WiFiClient client;


void enableServer() {
  IPAddress local_ip(192, 168, 1, 1);
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 255, 0);

  WiFi.mode(WIFI_AP);
  Serial.println("[SERVER] WiFi.mode");
  WiFi.softAPConfig(local_ip, gateway, subnet);
  Serial.println("[SERVER] WiFi.softAPConfig");
  WiFi.softAP(ssid, pass);
  Serial.println("[SERVER] WiFi.softAP");
  IPAddress IP = WiFi.softAPIP();
  Serial.println("[SERVER] IPAddress IP");
  server.begin();
  Serial.println("[SERVER] server.begin"); 
}

bool waitConnection() {
  while (!client || !client.connected()) {
    client = server.available();
    delay(10);  // Yield a bit to prevent watchdog reset
  }

  Serial.println("Connection established");
  int sequence[] = {20, 50, 20, 50, 50, 50};
  flash(sequence, 6, YELLOW);
  return true;
}


// -------------------- Led Setup --------------------



void flash(int sequence[], int length, int line) {
  for (int i = 0; i < length; i = i + 2) {
    digitalWrite(line, HIGH);
    delay(sequence[i]);
    digitalWrite(line, LOW);
    delay(sequence[i+1]);
  }
}
void initLed(int t){
  pinMode(t, OUTPUT);
  int sequence[] = {100, 200, 100, 200};
  flash(sequence, 4, t);

}

// -------------------- Command Registry --------------------

int registerCommand() {
  if (!client || !client.connected()) {
    client = server.available(); 
    Serial.println("[REGISTER] server.available");
  }

  if (client && client.connected() && client.available()) {
    int command = client.read();
    Serial.printf("[REGISTER] %d \n", command);
    return command;
  }

  return 0;
}



void interpretCommand(int command){
  switch(command){

    case 1: {
      int sequence[] = {50, 50, 50, 50, 50, 50};
      flash(sequence, 4, WHITE);
    } break;

    case 49: {
      int sequence[] = {50, 50, 50, 50, 50, 50};
      flash(sequence, 4, YELLOW);
    } break;

  }
}



void setup() {
  Serial.begin(115200);
  initLed(26);
  initLed(25);
  enableServer();
  Serial.println("Server enabled");
  waitConnection();
  Serial.println("Connection established");
}

void loop() {
  int command = registerCommand();
  interpretCommand(command);
}


