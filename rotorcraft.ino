#include <WiFi.h>
// #include <String.h>
#define YELLOW 26
#define WHITE 25
#define BUZZER 23

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
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(ssid, pass);
  IPAddress IP = WiFi.softAPIP();
  server.begin();
  Serial.println("[SERVER] Server enabled");
}

bool waitConnection() {
  while (!client || !client.connected()) {
    client = server.available();
    delay(10); 
  }

  int sequence[] = {20, 50, 20, 50, 50, 50};
  flash(sequence, 6, YELLOW);
  Serial.println("[SERVER] Connection established");
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

// -------------------- Buzzer Setup & Control --------------------


bool enableBuzzer = false;
void initBuzzer(){
  pinMode(BUZZER, OUTPUT);
}

void buzz(){
  tone(BUZZER, 1000); 
  delay(1000);        
  noTone(BUZZER);     
}

void toggleBuzzing(){
  enableBuzzer = !enableBuzzer;
}


// -------------------- Command Registry --------------------

int registerCommand() {
  if (!client || !client.connected()) {
    client = server.available(); 
    Serial.println("[REGISTER] server.available");
  }

  if (client && client.connected() && client.available()) {
    // Minus 48 because of ASCII enumeration.
    int command = client.read() - 48;
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
    case 2: {
      toggleBuzzing();
      
    } break;
    case 3: {
      buzz();
    } break;
  }
}



void setup() {
  Serial.begin(115200);
  initLed(26);
  initLed(25);
  buzz();
  enableServer();
  waitConnection();
}

void loop() {
  int command = registerCommand();
  interpretCommand(command);
  delay(100);
    if(enableBuzzer){
  buzz();
  }

}


