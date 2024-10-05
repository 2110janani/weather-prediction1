#include <ESP8266WiFi.h> 
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

// Pin definitions
const int redLED = 5;    
const int yellowLED = 4; 
const int greenLED = 3;  
const int buzzer = 2;    

// Wi-Fi credentials
const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

// OpenWeatherMap API details
String apiKey = "af9f9102b5e0379c3f0a669b6f24453a
";
String city = "YOUR_CITY_NAME";
String country = "YOUR_COUNTRY_CODE";
String server = "api.openweathermap.org";

void setup() {
  Serial.begin(115200);
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(buzzer, OUTPUT);

  connectToWiFi();
}

void loop() {
  String weather = getWeatherData();
  
  if (weather == "Clear") {
    signalSunny();
  } else if (weather == "Clouds") {
    signalCloudy();
  } else if (weather == "Rain") {
    signalRain();
  } else {
    Serial.println("Weather data unavailable.");
  }
  
  delay(60000);  // Fetch data every 60 seconds
}

void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  
  Serial.println("Connected to WiFi");
}

String getWeatherData() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    String url = "http://" + server + "/data/2.5/weather?q=" + city + "," + country + "&appid=" + apiKey;
    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload);
      
      // Simplified: parse the weather type from the JSON response
      if (payload.indexOf("Clear") >= 0) {
        return "Clear";
      } else if (payload.indexOf("Clouds") >= 0) {
        return "Clouds";
      } else if (payload.indexOf("Rain") >= 0) {
        return "Rain";
      }
    } else {
      Serial.println("Error in HTTP request");
    }
    
    http.end();
  }
  return "";
}

void signalSunny() {
  digitalWrite(greenLED, HIGH);
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, LOW);
  noTone(buzzer);  // No sound
  Serial.println("Sunny: Green light ON");
}

void signalCloudy() {
  digitalWrite(yellowLED, HIGH);
  digitalWrite(greenLED, LOW);
  digitalWrite(redLED, LOW);
  noTone(buzzer);  // No sound
  Serial.println("Cloudy: Yellow light ON");
}

void signalRain() {
  digitalWrite(redLED, HIGH);
  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED, LOW);
  tone(buzzer, 1000);  // Buzzer sound
  Serial.println("Rain: Red light ON, Buzzer ON");
}
