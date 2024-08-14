#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <HttpClient.h>

//SSID
char ssid[] = "";
//password
char pass[] = "";
// API Server
char server[] = "postman-echo.com";  // API endpoint
// WiFi client
WiFiClient client;
// Set up HTTP client
HttpClient http(client, server, 80);
// Set up screen
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, pass);
  Wire.begin(5, 4);
  // Set up display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
}

void loop() {
  // Set cursor, clear display and print WiFi status
  Serial.println(WiFi.status());
  display.clearDisplay();
  display.setCursor(0, 0);
  // Set WiFi Status and make HTTP request
  if (WiFi.status() == 3) {
    display.println("WIFI:");
    display.println("Connected");
    display.setTextSize(1);
    display.println(WiFi.localIP());
    display.setTextSize(2);

    httpRequest();
  } else if (WiFi.status() == 6) {
    display.println("WIFI:");
    display.println("Disconnected");
  } else if (WiFi.status() == 0) {
    display.println("WIFI:");
    display.println("IDLE");
  }
  display.display();
  delay(1000);
}


void httpRequest() {
  Serial.println("Sending HTTP GET request...");
  http.get("/get");
  
  // Check the HTTP status code
  int statusCode = http.responseStatusCode();
  Serial.print("HTTP Status Code: ");
  Serial.println(statusCode);

  // Check if the request was successful
  if (statusCode >= 200 && statusCode < 300) {
    String response = http.responseBody();
    // Print response
    Serial.println("Response:");
    Serial.println(response);
  } else {
    Serial.println("Error in HTTP GET request");
  }
}
