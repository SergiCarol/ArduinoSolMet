#include <DHT.h>
#include <EEPROM.h>
#include <WiFiNINA.h>
#include <ArduinoJson.h>
#include <OneWire.h> 
#include <DallasTemperature.h>

// DIGITAL PORTS PINS INFO
#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define ONE_WIRE_BUS 3 // Digital pin for water temperature 

// INTERNET CONNECTION INFO
#define PORT 5000
IPAddress gateway(192,168,1, 1);
IPAddress server(192,168,1,36);
int status = WL_IDLE_STATUS;     // the Wifi radio's status
char ssid[32]; 
char pwd[64];
char user_id[] = "123456789";
const int capacity = JSON_OBJECT_SIZE(6);
StaticJsonDocument<capacity> doc;
WiFiClient client;

// Define struct for temperature and humidity readings
typedef struct {
  float temperature;
  float humidity;
} temp_hum;

typedef struct {
  float temperature;
  float humidity;
  float water_temp;
  float water_ph;
  float water_electrodes;
} data;

DHT dht(DHTPIN, DHTTYPE);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  
  Serial.begin(9600);
  // Start sensor modules
  dht.begin();
  sensors.begin();

  //clearEEPROM();
  WIFILoadInfo();

  // Check if network info is stored in EEPROM
  if ((strlen(ssid) == 0) || (pwd == NULL)){
    Serial.println("No WIFI info in EEPROM");
    // If no client information is stored proced to boot up BLE
    //    BLE.begin();
    //    WIFIGetInfo();
    WIFIStoreInfo("Fibracat_07171", "9fac828a71");
    WIFILoadInfo();
  }

  Serial.print("Connecting to network SSID: ");
  Serial.println(ssid);
  Serial.print("Password: ");
  Serial.println(pwd);
   while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WEP network, SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pwd);
    delay(10000);
  }
  Serial.println("You're connected to the network");

  //printWifiData();
}

void loop() {
  data sensor_data;
  // Wait a few seconds between measurements.
  delay(2000);

  temp_hum result = readTempHum();

  // Check if any reads failed and exit early (to try again).
  if (isnan(result.temperature) || isnan(result.humidity)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  sensor_data.temperature = result.temperature;
  sensor_data.humidity = result.humidity;
  sensor_data.water_temp = readWaterTemp();
  sensor_data.water_electrodes = 0;
  sensor_data.water_ph = 0;

  sendData(sensor_data);
  
}

void clearEEPROM(void){
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
}

void sendData(data sensor_data){

  // Set data to be sent
  doc["temperature"] = sensor_data.temperature;
  doc["humidity"] = sensor_data.humidity;
  doc["water_temperature"] = sensor_data.water_temp;
  doc["water_electrodes"] = sensor_data.water_electrodes;
  doc["water_ph"] = sensor_data.water_ph;
  doc["user_id"] = user_id;

  // Begin connection
  Serial.println("Attempting to establish connection");

  if (client.connect(server, PORT)) {
    Serial.println("connected");
    client.println("POST / HTTP/1.1");
    client.println("Connection: close");
    client.print("Content-Length: ");
    client.println(measureJson(doc));
    client.println("Content-Type: application/json");
    // Terminate headers with a blank line
    client.println();
    // Send JSON document in body
    serializeJson(doc, client);
    serializeJson(doc, Serial);
  }
}

temp_hum readTempHum(void){

    temp_hum result;

    float h = dht.readHumidity();
    float t = dht.readTemperature();
    
    result.temperature = t;
    result.humidity = h;
    return(result);
}

float readWaterTemp(void){
  sensors.requestTemperatures();
  return(sensors.getTempCByIndex(0));  
}

void WIFILoadInfo(void){
  uint8_t ssid_length =  EEPROM.read(1);
  uint8_t pwd_length =  EEPROM.read(32);

  for (uint8_t addr=2; addr < ssid_length + 2;  addr++){
    ssid[addr-2] = EEPROM.read(addr);
  }

  for (uint8_t addr=33; addr < pwd_length + 33; addr++){
    pwd[addr-33] = EEPROM.read(addr);
  }
}

void WIFIStoreInfo(char *ssid_, char *pwd_){
  uint8_t ssid_length = strlen(ssid_);
  uint8_t pwd_length = strlen(pwd_);

  EEPROM.write(1, ssid_length);
  for (uint8_t addr=2; addr < ssid_length + 2; addr++){
    EEPROM.write(addr, ssid_[addr-2]);
  }

  EEPROM.write(32, pwd_length);
  for (uint8_t addr=33; addr < pwd_length + 33; addr++){
    EEPROM.write(addr, pwd_[addr-33]);
  }
}

void printWifiData() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(WiFi.status());
}
