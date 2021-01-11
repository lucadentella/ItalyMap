#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <Adafruit_NeoPixel.h>

#define NUM_REGIONS 20
String pixelRegione[NUM_REGIONS] = {"SIC", "CAL", "BAS", "CAM", "PUG", "MOL", "ABR", "LAZ", "SAR", "UMB", "TOS", "MAR", "EMR", "LIG", "PIE", "VDA", "LOM", "TAA", "VEN", "FVG"};

const char* ssid     = "MySSID";
const char* password = "MyPwd";
const char* csvHost  = "raw.githubusercontent.com";
const int csvPort    = 443;
const char* csvURL   = "/italia/covid19-opendata-vaccini/master/dati/vaccini-summary-latest.csv";

#define SIGNAL_PIN  14
Adafruit_NeoPixel strip(NUM_REGIONS, SIGNAL_PIN, NEO_GRB + NEO_KHZ800);

#define GREEN_TH    80.0
#define YELLOW_TH   60.0
#define ORANGE_TH   50.0

uint32_t red = strip.Color(255, 0, 0);
uint32_t orange = strip.Color(255, 60, 0);
uint32_t yellow = strip.Color(255, 230, 0);
uint32_t green = strip.Color(0, 255, 0);

#define UPDATE_INTERVAL 600
unsigned long lastUpdateTime = 0;

void setPixelColor(String region, int percentage) {

  // get the led corresponding to the region
  int ledNum = -1;
  for(int i = 0; i < NUM_REGIONS; i++)
    if(region == pixelRegione[i]) {
      ledNum = i;
      break;
    }
  
  if(percentage > GREEN_TH) strip.setPixelColor(ledNum, green);
  else if(percentage > YELLOW_TH) strip.setPixelColor(ledNum, yellow);  
  else if(percentage > ORANGE_TH) strip.setPixelColor(ledNum, orange);
  else strip.setPixelColor(ledNum, red); 
}

void updateMap() {

  Serial.println();
  Serial.println("------------ Updating Map ------------");

  WiFiClientSecure httpsClient;
  httpsClient.setInsecure();

  // file contains two values for Trentino Alto Adige
  float trentinoAvg = 0;
  
  if(!httpsClient.connect(csvHost, csvPort)) {
    Serial.println("Unable to connect to Github :(");
    return;
  }
  Serial.println("Connected to Github");
  httpsClient.print(String("GET ") + csvURL + " HTTP/1.1\r\n" +
    "Host: " + csvHost + "\r\n" +
    "Connection: close\r\n\r\n");
  
  // download all the headers
  while (httpsClient.connected()) {
    String line = httpsClient.readStringUntil('\n');
    if (line == "\r") break;
  }

  // download first line (column headers)
  String line = httpsClient.readStringUntil('\n');

  // download all the lines
  while(httpsClient.available()) {
    
    // get line and convert into char array (for strtok)
    String line = httpsClient.readStringUntil('\n');
    char buf[line.length()];
    line.toCharArray(buf, sizeof(buf));

    // divide the line into tokens (comma-separated)
    char *p = buf;
    char *str;
    int tokenCount = 0;
    String regionCode;
    float regionPercentage;
    
    // extract region code and number of new cases
    while ((str = strtok_r(p, ",", &p)) != NULL) {
      tokenCount++;
      if(tokenCount == 1) regionCode = String(str);
      else if(tokenCount == 4) regionPercentage = atof(str); 
    }

    // average P.A. Bolzano and P.A. Trento
    if(regionCode == "PAB") {
      trentinoAvg = regionPercentage;
      continue;
    }
    else if(regionCode == "PAT") {
      trentinoAvg = (trentinoAvg + regionPercentage) / 2;
      regionPercentage = trentinoAvg;
      regionCode = "TAA";
    }
    Serial.print("Region: ");
    Serial.print(regionCode);
    Serial.print(", new cases: ");
    Serial.println(regionPercentage);

    setPixelColor(regionCode, regionPercentage);
  }
  strip.show();
  Serial.println("Pixel colors updated");
}


void setup() {

  Serial.begin(115200);
  Serial.println("ItalyMap - Covid New Cases per Region");
  Serial.println();
  
  strip.begin();
  strip.setBrightness(5);
  strip.clear();
  Serial.println("NeoPixel library init DONE");
 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("WiFi init DONE");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
  Serial.print("Connected to ");
  Serial.println(ssid);

  updateMap();
}

void loop() {

  if(millis() > lastUpdateTime + (UPDATE_INTERVAL * 1000)) {
    updateMap();
    lastUpdateTime = millis();
  }
}
