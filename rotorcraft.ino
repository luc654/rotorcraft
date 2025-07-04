#include <WiFi.h>
// #include <String.h>
#define YELLOW 26
#define WHITE 25
#define GREEN 33

#define BUZZER 23

#define TRIGGER 5
#define ECHO 18

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

void ledOn(int line){
  digitalWrite(line,HIGH);
}
void ledOff(int line){
  digitalWrite(line,LOW);
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

// -------------------- Ultrasonic Sensor Setup, Control & Algorithmic features --------------------

int thresHold = 100;
int margin = 10;
void initSensor(){
  pinMode(TRIGGER, OUTPUT); // Sets the trigPin as an Output
  pinMode(ECHO, INPUT);
}

float sensorDistance(){
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);
  
  float duration = pulseIn(ECHO, HIGH);
  return duration * 0.034/2;
}

void sensorPing(){
  float cm = sensorDistance();
  bool running = true;
  bool flag = false;
  int count = 0;
  while(running){
  Serial.printf("[PING] Distance %.4f \n", cm);
  if(cm + 10 < thresHold || cm - 10 > thresHold){
    count++;
    delay(50);
  } else {
    running = false;
  }
  if(count > 5){
    running = false;
    flag = true;
  }
  }
  if(flag){
    if(enableBuzzer){
      buzz();
    }
  }
}

void setThreshold(){
  int cm = sensorDistance();
  thresHold = cm;
  margin = thresHold / 100 * 10;
  if(margin < 1){
    margin = 1;
  }
  Serial.printf("[THRESHOLD] Distance %d\n", cm);
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
      setThreshold();
      int sequence[] = {500, 0};
      flash(sequence, 2, GREEN);
    } break;
    case 4: {
      buzz();
    } break;
  }
}



void setup() {
  Serial.begin(115200);
  initLed(WHITE);
  initLed(YELLOW);
  initLed(GREEN);
  initSensor();

  setThreshold();
  enableServer();
  waitConnection();
}

void loop() {
  int command = registerCommand();
  interpretCommand(command);


  delay(100);
    if(enableBuzzer){
      sensorPing();
  }

}


