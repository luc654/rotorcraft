#include <ESP8266WiFi.h>

// -------------------- Wifi Setup --------------------
const char* ssid = "rotorcraft";
const char* pass = "password123";

WiFiServer server(80);
String header;


void connectServer() {
  WiFi.begin(ssid, pass);             // Connect to the network
  Serial.print(ssid); Serial.println(" ...");

  while (WiFi.status() != WL_CONNECTED) { 
    delay(1000);
  }

    WiFiClient client;
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
  initLed();
  connectServer();
}

void loop(){

}