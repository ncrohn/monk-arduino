#include <SPI.h>
#include <WiFly.h>

char* ssid = "becherovka";
char* pass = "my5up3rDS";

char* server = "192.168.0.23";
int port = 7001;
int temp1Pin = 3;
int filterCycles = 15;
int filterCount = 0;
float filteredTempC = 0;
float lastCycleTemp = 0.0;
bool sendData = false;

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
  
  if(filterCount < filterCycles) {
    filteredTempC += tempC;
  } else {
    tempC = filteredTempC / 15.0;
    filteredTempC = 0;
    sendData = true;
  }

if(client.connected() && sendData == true) {

  float tempF = (tempC * 9.0 / 5.0) + 32.0;

  // Sending
  // {
  //   "tempC": "50.0",
  //   "tempF": "150.0"
  // }

  Serial.print("{\"tempC\": \"");
  Serial.print(tempC);
  Serial.print("\", \"tempF\": \"");
  Serial.print(tempF);
  Serial.println("\"}");
  
  sendData = false;
 }

  if(client.available()) {
    Serial.println("Client available");
  }

  delay(1000);
}

float readTemp() {
  int read = analogRead(temp1Pin);
  float tempC = (read * 5.0);
  tempC = (tempC - 0.5) * 100;
  tempC /= 1024.0;

  return tempC;
}
