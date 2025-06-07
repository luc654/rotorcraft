#include <ESP8266WiFi.h>

// -------------------- Wifi Setup --------------------
const char* ssid = "rotorcraft";
const char* pass = "password123";

WiFiServer server(80);
String header;
WiFiClient client;


void connectServer() {
  WiFi.begin(ssid, pass);
  Serial.println("[SERVER] WiFi.begin");             
  Serial.print(ssid);
  Serial.println("[SERVER] Serial.print"); 

  while (WiFi.status() != WL_CONNECTED) { 
    delay(1000);
    Serial.println("[SERVER] delay(1000");
  }

    if (client.connect("192.168.1.1", 80)) {
    int sequence[] = {20, 50, 20, 50, 50, 50};
    flash(sequence, 6);

    };


}


// -------------------- Led Setup --------------------

int line = 2;


void flash(int sequence[], int length) {
  for (int i = 0; i < length; i = i + 2) {
    digitalWrite(line, HIGH);
    delay(sequence[i]);
    digitalWrite(line, LOW);
    delay(sequence[i+1]);
  }
}
void initLed(){
  pinMode(line, OUTPUT);
  int sequence[] = {100, 200, 100, 200};
  flash(sequence, 4);

}


void setup(){
  Serial.begin(115200);
  initLed();
  connectServer();
}

// -------------------- Command sending --------------------

void transmit(int command){
  client.print(command);
  Serial.printf("[Transmit] %d \n", command);
}


void loop(){
  transmit(1);
  delay(500);
}