#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

// Wi-Fi credentials
const char* ssid = "X";
const char* password = "@MR26148#011008";

// API endpoint
const String API_URL = "https://core-fit-production.up.railway.app/reservations/verify-password";

// GPIO pins on ESP8266 for status LEDs
const int correctPin = D7;   // GPIO13
const int wrongPin = D5;     // GPIO14
const int checkingPin = D6;  // GPIO12

WiFiClientSecure client;

void setup() {
  Serial.begin(9600);

  pinMode(correctPin, OUTPUT);
  pinMode(wrongPin, OUTPUT);
  pinMode(checkingPin, OUTPUT);

  reset_pins();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting...");
  }

  Serial.println("WiFi Connected");
  client.setInsecure();  // For development only
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    // Only proceed if it's a valid 6-digit password
    if (input.length() == 6) {
      reset_pins();                     // Clear previous state
      digitalWrite(checkingPin, HIGH);  // Turn on checking LED
      if (input == "000000") {
        digitalWrite(checkingPin, LOW);  // Turn off checking LED after response
        Serial.println("true");
        digitalWrite(correctPin, HIGH);
        delay(10000);  // Keep result LED on for 10 seconds

      } else {
        HTTPClient http;
        http.begin(client, API_URL);
        http.addHeader("Content-Type", "application/json");

        String payload = "{\"password\":\"" + input + "\", \"playgroundId\": 1}";
        int httpCode = http.POST(payload);
        String response = http.getString();
        response.trim();


        digitalWrite(checkingPin, LOW);  // Turn off checking LED after response

        if (httpCode > 0) {
          if (response.indexOf("true") >= 0) {
            Serial.println("true");
            digitalWrite(correctPin, HIGH);
            delay(10000);  // Keep result LED on for 10 seconds
          } else {
            Serial.println("false");
            digitalWrite(wrongPin, HIGH);
            delay(3000);  // Keep result LED on for 3 seconds
          }
        } else {
          Serial.println("error");
          digitalWrite(wrongPin, HIGH);
          delay(3000);  // Keep result LED on for 3 seconds
        }
        http.end();
      }
      reset_pins();
    }
  }
}

void reset_pins() {
  digitalWrite(correctPin, LOW);
  digitalWrite(wrongPin, LOW);
  digitalWrite(checkingPin, LOW);
}
