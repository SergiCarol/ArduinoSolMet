#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <avr/wdt.h>
#include <DallasTemperature.h>
#include <EEPROM.h>
#include <OneWire.h>
#include <WiFiNINA.h>
#include <stdbool.h>
// DIGITAL PORTS PINS INFO
#define DHTPIN 2        // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define ONE_WIRE_BUS 3  // Digital pin for water temperature
#define SensorPin 0     // pH meter Analog output to Arduino Analog Input 0
#define ElectrodesPin 1 // Electrodes Analog input of arduino
#define VREF 5.0
#define water_pump_1 4
#define air_pump 5
#define fan 6
#define extractor 7
#define water_heater 8
#define lights 9

// INTERNET CONNECTION INFO
#define PORT 5000
#define MAX_RETRY 1
#define UID_LENGTH 16

IPAddress ip(192, 168, 1, 125);
IPAddress server(192, 168, 43, 109);
int status = WL_IDLE_STATUS;  // the Wifi radio's status
String ssid;
String pwd;
String api_key = "123456789";
const int capacity = JSON_OBJECT_SIZE(7);
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
String readUID(void);

void reboot() {
  wdt_disable();
  wdt_enable(WDTO_15MS);
  while (1) {}
}


bool connectNetwork(String network_ssid, String network_pwd) {
    for (uint8_t retry = 0; retry < MAX_RETRY; retry++) {
        char ssid_[32];
        char pwd_[64];
        Serial.print("Attempting to connect to WEP network, SSID: ");
        Serial.println(network_ssid);
        network_ssid.toCharArray(ssid_, 32);
        network_pwd.toCharArray(pwd_, 64);
        status = WiFi.begin(ssid_, pwd_);
        delay(10000);
        Serial.print("Status: ");
        Serial.println(status);
        if (status == WL_CONNECTED) {
            ssid = network_ssid;
            pwd = network_pwd;
            Serial.println("Connected!");
            return (true);
        }
    }
    return (false);
}

void clearEEPROM(void) {
    for (int i = 0; i < EEPROM.length(); i++) {
        EEPROM.write(i, 0);
    }
}

void sendData(data sensor_data) {
    // Set data to be sent
    doc["temperature"] = sensor_data.temperature;
    doc["humidity"] = sensor_data.humidity;
    doc["water_temperature"] = sensor_data.water_temp;
    doc["water_electrodes"] = sensor_data.water_electrodes;
    doc["water_ph"] = sensor_data.water_ph;
    doc["api_key"] = readUID();

    // Begin connection
    Serial.println("Attempting to establish connection");
    WiFiClient wifi;
    HttpClient client = HttpClient(wifi, server, PORT);
    String contentType = "application/json";
    String postData = "";
    serializeJson(doc, postData);
    Serial.println(postData);
    client.post("/upload", contentType, postData);
    int statusCode = client.responseStatusCode();
    String response = client.responseBody();

    Serial.print("Status code: ");
    Serial.println(statusCode);
    Serial.print("Response: ");
    Serial.println(response);
    
    readResponse(response);
}

void readResponse(String response){
    DynamicJsonDocument services(2048);
    deserializeJson(services, response);

    digitalWrite(water_pump_1, !services["water_pump_1"] | true);
    digitalWrite(air_pump, !services["air_pump"] | true);
    digitalWrite(fan, !services["fan"] | true);
    digitalWrite(extractor, !services["extractor"] | true);
    digitalWrite(water_heater, !services["water_heater"] | true);
    digitalWrite(lights, !services["lights"] | true);

}

float readElectrodes(void) {
    int analogBuffer[10], tmp;
    unsigned long int
        avgValue;  // Store the average value of the sensor feedback
    float b;
    float averageVoltage = 0,tdsValue = 0,temperature = 25;

    for (int i = 0; i < 10; i++) {
        analogBuffer[i] = analogRead(ElectrodesPin);
        delay(10);
    }
    avgValue = 0;
    for (int i = 0; i < 10; i++) {
        avgValue += analogBuffer[i];
    }
    avgValue = avgValue / 10;
    averageVoltage =  avgValue * (float)VREF/ 1024.0;
    tdsValue=(133.42*averageVoltage*averageVoltage*averageVoltage - 255.86*averageVoltage*averageVoltage + 857.39*averageVoltage)*0.5;
    Serial.print("TDS Value:");
    Serial.print(tdsValue,0);
    Serial.println("ppm");
    return(tdsValue);
}

temp_hum readTempHum(void) {
    temp_hum result;

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    result.temperature = t;
    result.humidity = h;
    return (result);
}

float readWaterTemp(void) {
    sensors.requestTemperatures();
    return (sensors.getTempCByIndex(0));
}

float readPH(void) {
    unsigned long int
        avgValue;  // Store the average value of the sensor feedback
    float b;
    int buf[10], temp;

    for (int i = 0; i < 10; i++) {
        buf[i] = analogRead(SensorPin);
        delay(10);
    }
    for (int i = 0; i < 9; i++)  // sort the analog from small to large
    {
        for (int j = i + 1; j < 10; j++) {
            if (buf[i] > buf[j]) {
                temp = buf[i];
                buf[i] = buf[j];
                buf[j] = temp;
            }
        }
    }
    avgValue = 0;
    for (int i = 2; i < 8; i++)  // take the average value of 6 center sample
        avgValue += buf[i];
    float phValue =
        (float)avgValue * 5.0 / 1024 / 6;  // convert the analog into millivolt
    return (3.5 * phValue);  // convert the millivolt into pH value
}


bool createAP(void) {
    String tmp_ssid;
    String tmp_pwd;

    status = WiFi.beginAP("ArduinoAP");
    if (status != WL_AP_LISTENING) {
        Serial.println("Creating access point failed");
        // don't continue
        while (true)
            ;
    }

    // wait 10 seconds for connection:
    delay(10000);

    // start the web server on port 80
    WiFiServer ap(80);

    ap.begin();
    IPAddress ip;  // the IP address of your board
    ip = WiFi.localIP();
    Serial.println(ip);
    Serial.println("Waiting for client");
    delay(1000);
    while (true) {
        WiFiClient client = ap.available();  // listen for incoming clients
        client.flush();
        if (client.connected()) {
            Serial.println(
                "new client");  // print a message out the serial port
            String currentLine = "";  // make a String to hold incoming data from the client
            Serial.println(client.status());
            while (client.connected()) {   // loop while the client's connected
                if (client.available()) {  // if there's bytes to read from the
                                           // client,
                    char c = client.read();  // read a byte, then
                    Serial.write(c);         // print it out the serial monitor
                    if (c == '\n') {
                        if (currentLine.length() == 0) {
                        Serial.println("End of communication");
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println();
                        client.println();
                        delay(500);
                        client.flush(); 
                        client.stop();

                        WiFi.disconnect();
                        if (!connectNetwork(tmp_ssid,
                                            tmp_pwd)) {
                            return (false);
                        } else {
                            writeUID(api_key);
                            return (true);
                        }
                    }
                    else {
                        currentLine = "";
                    }
                    } else if (c != '\r') {  // if you got anything else but a
                                             // carriage return character,
                        
                        currentLine +=
                            c;  // add it to the end of the currentLine
                    }
                    // http://192.168.1.4/?ssid=NOM_SSID&?pwd=NOM_PWD!
                    if (currentLine.startsWith("GET /?ssid=") &&
                        currentLine.endsWith("HTTP/1.1")) {
                        Serial.println(currentLine);
                        uint8_t start_ssid = currentLine.indexOf("?ssid=");
                        uint8_t end_ssid = currentLine.indexOf('&');
                        tmp_ssid = 
                            currentLine.substring(start_ssid + 6, end_ssid);
                        currentLine = currentLine.substring(end_ssid + 1);
                        
                        Serial.println(currentLine);

                        uint8_t start_pwd = currentLine.indexOf("?pwd=");
                        uint8_t end_pwd = currentLine.indexOf('&');
                        tmp_pwd =
                            currentLine.substring(start_pwd + 5, end_pwd);
                        currentLine = currentLine.substring(end_pwd + 1);
                        Serial.println(currentLine);
                        uint8_t start_api = currentLine.indexOf("?api=");
                        uint8_t end_api = currentLine.indexOf('!');
                        api_key =
                            currentLine.substring(start_pwd + 5, end_pwd);

                        Serial.print("SSID value = ");
                        Serial.println(tmp_ssid);
                        Serial.print("PWD value = ");
                        Serial.println(tmp_pwd);
                        Serial.print("API KEY value = ");
                        Serial.println(api_key);
                        }
                    
                }
            }
        }
    }
}

void WIFILoadInfo(void) {
    uint8_t ssid_length = EEPROM.read(1);
    uint8_t pwd_length = EEPROM.read(32);

    for (uint8_t addr = 2; addr < ssid_length + 2; addr++) {
        ssid[addr - 2] = EEPROM.read(addr);
    }

    for (uint8_t addr = 33; addr < pwd_length + 33; addr++) {
        pwd[addr - 33] = EEPROM.read(addr);
    }
}

void WIFIStoreInfo(String ssid_, String pwd_) {
    uint8_t ssid_length = ssid_.length();
    uint8_t pwd_length = pwd_.length();

    EEPROM.write(1, ssid_length);
    for (uint8_t addr = 2; addr < ssid_length + 2; addr++) {
        EEPROM.write(addr, ssid_[addr - 2]);
    }

    EEPROM.write(32, pwd_length);
    for (uint8_t addr = 33; addr < pwd_length + 33; addr++) {
        EEPROM.write(addr, pwd_[addr - 33]);
    }
}

String readUID(void){
  String tmp_uid;
  for (uint8_t addr = 100; addr < 100 + UID_LENGTH; addr++) {
        tmp_uid[addr - 100] = EEPROM.read(addr);
    }
    Serial.print("Reading api key: ");
    Serial.println(tmp_uid);
    return(tmp_uid);
}

void writeUID(String UID){
    Serial.print("Writting api key: ");
    Serial.println(UID);
  for (uint8_t addr = 100; addr < 100 + UID_LENGTH + 2; addr++) {
        EEPROM.write(addr, UID[addr - 100]);
    }
}


void setup() {
    Serial.begin(9600);
    // Start sensor modules
    dht.begin();
    sensors.begin();
    pinMode(SensorPin, INPUT);
    pinMode(ElectrodesPin, INPUT);
    pinMode(ElectrodesPin, INPUT);
    pinMode(water_pump_1, OUTPUT);
    pinMode(air_pump, OUTPUT);
    pinMode(fan, OUTPUT);
    pinMode(extractor, OUTPUT);
    pinMode(water_heater, OUTPUT);
    pinMode(lights, OUTPUT);
    //clearEEPROM();
    //WIFILoadInfo();

    digitalWrite(water_pump_1,  true);
    digitalWrite(air_pump,  true);
    digitalWrite(fan,  true);
    digitalWrite(extractor,  true);
    digitalWrite(water_heater,  true);
    digitalWrite(lights,  true);

    //  Check if network info is stored in EEPROM
    /*
    if ((ssid.length() == 0) || (pwd.length() == 0)) {
        Serial.println("No WIFI info in EEPROM");
        while (!createAP());
        WIFIStoreInfo(ssid, pwd);
        WIFILoadInfo();
    }
    else{
        Serial.print("Connecting to network SSID: ");
        Serial.println(ssid);
        Serial.print("Password: ");
        Serial.println(pwd);
        if (!connectNetwork(ssid, pwd)) {
            while (createAP())
                ;
        }
    }
    Serial.println("You're connected to the network");
    WiFi.config(ip); 
    // printWifiData();
    */
}

void loop() {
    data sensor_data;
    // Wait a few seconds between measurements.
    uint8_t d = Serial.read();

    if (d == 97){
        digitalWrite(water_pump_1,  false);
    }
    if (d == 98){
        digitalWrite(air_pump,  false);
    }
    if (d == 99){
        digitalWrite(fan,  false);
    }
    if (d == 100){
        digitalWrite(extractor,  false);
    }
    if (d == 101){
        digitalWrite(water_heater,  false);
    }
    if (d == 102){
        digitalWrite(lights,  false);
    }
    if (d == 103) {
            digitalWrite(water_pump_1,  true);
        digitalWrite(air_pump,  true);
        digitalWrite(fan,  true);
        digitalWrite(extractor,  true);
        digitalWrite(water_heater,  true);
        digitalWrite(lights,  true);
    }
}
