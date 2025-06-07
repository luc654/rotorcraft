#include <WiFi.h>
#define yellow 26
#define white 25

// -------------------- Wifi Setup --------------------
const char* ssid = "rotorcraft";
const char* pass = "password123";

WiFiServer server(80);
String header;


void enableServer() {
  IPAddress local_ip(192, 168, 1, 1);
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 255, 0);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(ssid, pass);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  server.begin(); 
}

bool waitConnection(){
  bool conn = false;
  while (!conn){
    if(WiFiClient client = server.available()){
      conn = true;
    }
  }
  int sequence[] = {20, 50, 20, 50, 50, 50};
  flash(sequence, 6, yellow);
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




void setup() {
  Serial.begin(115200);
  initLed(26);
  initLed(25);
  enableServer();
  waitConnection();
}

void loop() {
  
}


