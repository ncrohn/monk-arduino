#include <SPI.h>
#include <WiFly.h>

char* ssid = "ssid";
char* pass = "pass";

char* server = "your-server-ip";
int port = 7001;
int temp1Pin = 3;
int filterCycles = 15;
float lastCycleTemp = 0.0;

WiFlyClient client;

// Calculation for temp in Celcius = (Vout in mV) / 10
// Conversion for temp to F = (Celcius * 9.0 / 5.0) + 32.0
// Vout mV = (reading from ADC) * (5000/1024) 

void setup() {
  // Setup the server
  Serial.begin(9600);
  WiFly.setUart(&Serial);
  WiFly.begin();
  WiFly.join(ssid, pass, true);
  client.connect(server, port);
}

void loop() {
  // Loop

  float tempC = readTemp();
  float tempF = (tempC * 9.0 / 5.0) + 32.0;

  if(client.connected()) {
    //Serial.println("Connected to client");

    // Sending
    // {
    //   "tempC": "50.0",
    //   "tempF": "150.0"
    // }

    if(lastCycleTemp != tempC) {
      Serial.print("{\"tempC\": \"");
      Serial.print(tempC);
      Serial.print("\", \"tempF\": \"");
      Serial.print(tempF);
      Serial.println("\"}");
    }
  }

  if(client.available()) {
    Serial.println("Client available");
  }

  lastCycleTemp = tempC;

  delay(1000);
}

float readTemp() {
  int read = analogRead(temp1Pin);
  float tempC = (read * 5.0);
  tempC = (tempC - 0.5) * 100;
  tempC /= 1024.0;

  return tempC;
}
