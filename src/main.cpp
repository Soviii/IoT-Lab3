
#include <WiFi.h>
#include <HttpClient.h>
#include <DHT20.h>
#include <string.h>
// #include <sstream>

#define SCL_PIN GPIO_NUM_21 // white header pin
#define SDA_PIN GPIO_NUM_22 // blue header pin

float temperature_val = 0; // stores temperature
float humidity_val = 0; // stores humidity

DHT20 dht20; // DHT object for reading temperature and sensor

// This example downloads the URL "http://arduino.cc/"
char ssid[] = "Sovi";    // your network SSID (name) 
char pass[] = "iVoS7769"; // your network password (use for WPA, or use as key for WEP)

// Name of the server we want to connect to
const char hostname[] = "52.53.207.138";
// Path to download (this is the bit after the hostname in the URL
// that you want to download
char path[100] = "/?temperature=00.00&humidity=00.00";

const uint16_t port = 5000;

// Number of milliseconds to wait without receiving any data before we give up
const int kNetworkTimeout = 30*1000;
// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 1000;

/* changing the path variable before connecting to webpage */
void SetupPathForWebPage(){
  String temp_str = String(temperature_val);
  String hum_str = String(humidity_val);
  // Serial.print("temp_str: ");
  // Serial.println(temp_str);
  
  // Serial.print("hum_str: ");
  // Serial.println(hum_str);

  // 15 16 18 19
  // 30 31 33 34
  // total length: 34 (33 last index)
  for(int i = 0; i < 5; i++){
    if(i == 2) continue; // no need to change period in path variable
    
    path[14+i] = temp_str[i];
    path[29+i] = hum_str[i];
  }


}

/* source code from Part A; connecting to webpage using HTTP */
void ConnectToFlaskWebPage(){
  WiFiClient c;
  HttpClient http(c);
  
  int err = 0; // initializing error status variable

  SetupPathForWebPage();

  //"52.53.207.138:5000/?...",
  err = http.get(hostname, port, path);
  if (err == 0){
    Serial.println("connected to webpage");

    err = http.responseStatusCode();
    if (err >= 0){
      Serial.print("Got status code: ");
      Serial.println(err);

      // Usually you'd check that the response code is 200 or a
      // similar "success" code (200-299) before carrying on,
      // but we'll print out whatever response we get

      err = http.skipResponseHeaders();
      if (err >= 0){
        Serial.println("Success!\n");
      }
      else {
        Serial.print("Failed to skip response headers: ");
        Serial.println(err);
      }
    }
    else {    
      Serial.print("Getting response failed: ");
      Serial.println(err);
    }
  }
  else {
    Serial.print("Connect failed: ");
    Serial.println(err);
  }
  http.stop();

  Serial.println("");
}

/* connecting to wifi source; only need to do once */
void ConnectToWifi(){
  // We start by connecting to a WiFi network
  delay(1000);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  // while the TTGO has not been connected to the wifi source
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println("WiFi connected");
}


void GatherTempAndHumidityValues(){
    dht20.read(); // called for receiving new values
    temperature_val = dht20.getTemperature(); // gets temperature in Celsius
    humidity_val = dht20.getHumidity(); // gets humidity in RH (relative humidity; relationship between moisture in air)
    Serial.print("Temperature: ");
    Serial.print(temperature_val);
    Serial.print("C, Humidity: ");
    Serial.print(humidity_val);
    Serial.println("%");
}

void setup() {
  Serial.begin(9600);
  
  dht20.begin(SDA_PIN, SCL_PIN); // connecting TTGO to DHT20 sensor (for temperature and humidity) through I2C connection

  ConnectToWifi();
}

void loop() {
  GatherTempAndHumidityValues();

  ConnectToFlaskWebPage();

  delay(2000);
}
