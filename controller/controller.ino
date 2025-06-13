#include <ESP8266WiFi.h>

// -------------------- Button setup --------------------

#define BUTTON_A 13
#define BUTTON_B 12
#define BUTTON_C 14

unsigned long lastButtonTime = 0;
const unsigned long buttonCooldown = 300; 


void initBtns(){
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);
}

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
  int index = 0;
  while (WiFi.status() != WL_CONNECTED) { 
    delay(1000);
    Serial.printf("[SERVER] Attempting to connect. (%d) \n", index);
    index++;
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
  initBtns();
  connectServer();
}

// -------------------- Command sending --------------------

void transmit(int command){
  client.print(command);
  Serial.printf("[Transmit] %d \n", command);
}

void handleButtons() {
  if (millis() - lastButtonTime < buttonCooldown) return;

  if(digitalRead(BUTTON_A) == LOW && digitalRead(BUTTON_C) == LOW){
    Serial.println("[BTN] A & C");

  } else
   if (digitalRead(BUTTON_A) == LOW) {
    Serial.println("[BTN] A");
    transmit(1);
    lastButtonTime = millis();

    
  } else if (digitalRead(BUTTON_B) == LOW) {
    Serial.println("[BTN] B");
    lastButtonTime = millis();


  } else if (digitalRead(BUTTON_C) == LOW) {
    Serial.println("[BTN] C");
    lastButtonTime = millis();
  }
}

void loop(){
  delay(100);
  handleButtons();
}