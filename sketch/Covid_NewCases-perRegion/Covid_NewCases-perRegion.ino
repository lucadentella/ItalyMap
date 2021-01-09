#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <Adafruit_NeoPixel.h>

#define NUM_REGIONS 20
int pixelRegione[NUM_REGIONS] = {14, 15, 16, 17, 18, 19, 13, 12, 10, 9, 11, 7, 6, 5, 3, 4, 2, 1, 0, 8};

const char* ssid     = "MySSID";
const char* password = "MyPWD";
const char* csvHost  = "raw.githubusercontent.com";
const int csvPort    = 443;
const char* csvURL   = "/pcm-dpc/COVID-19/master/dati-regioni/dpc-covid19-ita-regioni-latest.csv";

#define SIGNAL_PIN  14
Adafruit_NeoPixel strip(NUM_REGIONS, SIGNAL_PIN, NEO_GRB + NEO_KHZ800);

#define RED_TH      1000
#define ORANGE_TH   600
#define YELLOW_TH   300

uint32_t red = strip.Color(255, 0, 0);
uint32_t orange = strip.Color(255, 60, 0);
uint32_t yellow = strip.Color(255, 230, 0);
uint32_t green = strip.Color(0, 255, 0);

#define UPDATE_INTERVAL 600
unsigned long lastUpdateTime = 0;

void setPixelColor(int region, int cases) {

  // array starts at 0
  region = region - 1;
  
  if(cases > RED_TH) strip.setPixelColor(pixelRegione[region], red);
  else if(cases > ORANGE_TH) strip.setPixelColor(pixelRegione[region], orange);
  else if(cases > YELLOW_TH) strip.setPixelColor(pixelRegione[region], yellow);
  else strip.setPixelColor(pixelRegione[region], green); 
}

void updateMap() {

  Serial.println();
  Serial.println("------------ Updating Map ------------");

  WiFiClientSecure httpsClient;
  httpsClient.setInsecure();

  // file contains two values for Trentino Alto Adige, code 04 (21 = P.A. Bolzano and 22 = P.A. Trento)
  int trentinoSum = 0;
  
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
    int regionCode;
    int regionNewCases;
    
    // extract region code and number of new cases
    while ((str = strtok_r(p, ",", &p)) != NULL) {
      tokenCount++;
      if(tokenCount == 3) regionCode = atoi(str);
      else if(tokenCount == 13) regionNewCases = atoi(str); 
    }

    // sum P.A. Bolzano and P.A. Trento
    if(regionCode == 21) {
      trentinoSum += regionNewCases;
      continue;
    }
    else if(regionCode == 22) {
      trentinoSum += regionNewCases;
      regionNewCases = trentinoSum;
      regionCode = 4;
    }
    Serial.print("Region: ");
    Serial.print(regionCode);
    Serial.print(", new cases: ");
    Serial.println(regionNewCases);

    setPixelColor(regionCode, regionNewCases);
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
