#include <ArduinoJson.h>
#include <DHT.h>
#include <DallasTemperature.h>
#include <EEPROM.h>
#include <OneWire.h>
#include <WiFiNINA.h>
// DIGITAL PORTS PINS INFO
#define DHTPIN 2        // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define ONE_WIRE_BUS 3  // Digital pin for water temperature
#define SensorPin 0     // pH meter Analog output to Arduino Analog Input 0

// INTERNET CONNECTION INFO
#define PORT 5000
#define MAX_RETRY 5
IPAddress ip(192, 168, 1, 125);
IPAddress server(192, 168, 1, 36);
int status = WL_IDLE_STATUS;  // the Wifi radio's status
String ssid;
String pwd;
char api_key[] = "123456789";
const int capacity = JSON_OBJECT_SIZE(6);
StaticJsonDocument<capacity> doc;
WiFiClient client;
WiFiServer ap(80);

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

    clearEEPROM();
    WIFILoadInfo();

    // Check if network info is stored in EEPROM
    if ((ssid.length() == 0) || (pwd.length() == 0)) {
        Serial.println("No WIFI info in EEPROM");
        while (!createAP())
            ;
        //    WIFIGetInfo(); Fibracat_07171 9fac828a71
        WIFIStoreInfo(ssid, pwd);
        WIFILoadInfo();
    }

    Serial.print("Connecting to network SSID: ");
    Serial.println(ssid);
    Serial.print("Password: ");
    Serial.println(pwd);
    if (!connectNetwork(ssid, pwd)) {
        while (createAP())
            ;
    }
    Serial.println("You're connected to the network");
    WiFi.config(ip); 
    // printWifiData();
}

void loop() {
    data sensor_data;
    // Wait a few seconds between measurements.
    delay(2000);

    temp_hum result = readTempHum();

    // Check if any reads failed and exit early (to try again).
    if (isnan(result.temperature) || isnan(result.humidity)) {
        Serial.println(F("Failed to read from sensor!"));
        return;
    }

    sensor_data.temperature = result.temperature;
    sensor_data.humidity = result.humidity;
    sensor_data.water_temp = readWaterTemp();
    sensor_data.water_electrodes = 0;
    sensor_data.water_ph = readPH();

    sendData(sensor_data);
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
    doc["api_key"] = api_key;

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

void printWifiData() {
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your board's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
    Serial.println(WiFi.status());
}

bool createAP(void) {
    bool wait_for_cinnection = true;
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
    ap.begin();
    IPAddress ip;  // the IP address of your board
    ip = WiFi.localIP();
    Serial.println(ip);
    Serial.println("Waiting for client");
    delay(1000);
    while (wait_for_cinnection) {
        WiFiClient client = ap.available();  // listen for incoming clients
        if (client) {
            Serial.println(
                "new client");  // print a message out the serial port
            String currentLine =
                "";  // make a String to hold incoming data from the client
            while (client.connected()) {   // loop while the client's connected
                if (client.available()) {  // if there's bytes to read from the
                                           // client,
                    char c = client.read();  // read a byte, then
                    Serial.write(c);         // print it out the serial monitor
                    if (c == '\n') {
                        currentLine = "";
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
                        uint8_t start_pwd = currentLine.indexOf("?pwd=");
                        uint8_t end_pwd = currentLine.indexOf('!');
                        tmp_ssid = 
                            currentLine.substring(start_ssid + 6, end_ssid);
                        tmp_pwd =
                            currentLine.substring(start_pwd + 5, end_pwd);
                        Serial.print("SSID value = ");
                        Serial.println(tmp_ssid);
                        Serial.print("PWD value = ");
                        Serial.println(tmp_pwd);
                        client.stop();
                        WiFi.disconnect();
                        if (!connectNetwork(tmp_ssid,
                                            tmp_pwd)) {
                            return (false);
                        } else {
                            return (true);
                        }
                    }
                }
            }
        }
    }
}

// GET /?ssid=Testssid&pwd=Testpwd&submit=SSID+AND+PWD HTTP/1.1

/* TODO
char[] listNetworks() {
  // scan for nearby networks:
  Serial.println("** Scan Networks **");
  int numSsid = WiFi.scanNetworks();
  if (numSsid == -1) {
    Serial.println("Couldn't get a wifi connection");
    while (true);
  }

  // print the list of networks seen:
  Serial.print("number of available networks:");
  Serial.println(numSsid);

  char ssids [numSsid]:
  // print the network number and name for each network found:
  for (int thisNet = 0; thisNet < numSsid; thisNet++) {
    ssid[thisNet] = WiFi.SSID(thisNet));
  }
  return(ssids)
}*/