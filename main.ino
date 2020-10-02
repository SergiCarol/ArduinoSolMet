#include <DHT.h>
#include <EEPROM.h>
#include <WiFiNINA.h>
#include <ArduinoJson.h>
#include <OneWire.h>
#include <DallasTemperature.h>
// DIGITAL PORTS PINS INFO
#define DHTPIN 2       // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22  // DHT 22  (AM2302), AM2321
#define ONE_WIRE_BUS 3 // Digital pin for water temperature
#define SensorPin 0    //pH meter Analog output to Arduino Analog Input 0

// INTERNET CONNECTION INFO
#define PORT 5000
IPAddress gateway(192, 168, 1, 1);
IPAddress server(192, 168, 1, 36);
int status = WL_IDLE_STATUS; // the Wifi radio's status
char ssid[32];
char pwd[64];
char api_key[] = "123456789";
const int capacity = JSON_OBJECT_SIZE(6);
StaticJsonDocument<capacity> doc;
WiFiClient client;
WiFiServer ap(80);


// Define struct for temperature and humidity readings
typedef struct
{
  float temperature;
  float humidity;
} temp_hum;

typedef struct
{
  float temperature;
  float humidity;
  float water_temp;
  float water_ph;
  float water_electrodes;
} data;

DHT dht(DHTPIN, DHTTYPE);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup()
{

  Serial.begin(9600);
  // Start sensor modules
  dht.begin();
  sensors.begin();

  clearEEPROM();
  WIFILoadInfo();

  // Check if network info is stored in EEPROM
  if ((strlen(ssid) == 0) || (pwd == NULL))
  {
    Serial.println("No WIFI info in EEPROM");
    createAP();
    //    WIFIGetInfo();
    WIFIStoreInfo("Fibracat_07171", "9fac828a71");
    WIFILoadInfo();
  }

  Serial.print("Connecting to network SSID: ");
  Serial.println(ssid);
  Serial.print("Password: ");
  Serial.println(pwd);
  while (status != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to WEP network, SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pwd);
    delay(10000);
  }
  Serial.println("You're connected to the network");

  //printWifiData();
}

void loop()
{
  data sensor_data;
  // Wait a few seconds between measurements.
  delay(2000);

  temp_hum result = readTempHum();

  // Check if any reads failed and exit early (to try again).
  if (isnan(result.temperature) || isnan(result.humidity))
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  sensor_data.temperature = result.temperature;
  sensor_data.humidity = result.humidity;
  sensor_data.water_temp = readWaterTemp();
  sensor_data.water_electrodes = 0;
  sensor_data.water_ph = readPH();

  sendData(sensor_data);
}

void clearEEPROM(void)
{
  for (int i = 0; i < EEPROM.length(); i++)
  {
    EEPROM.write(i, 0);
  }
}

void sendData(data sensor_data)
{

  // Set data to be sent
  doc["temperature"] = sensor_data.temperature;
  doc["humidity"] = sensor_data.humidity;
  doc["water_temperature"] = sensor_data.water_temp;
  doc["water_electrodes"] = sensor_data.water_electrodes;
  doc["water_ph"] = sensor_data.water_ph;
  doc["api_key"] = api_key;

  // Begin connection
  Serial.println("Attempting to establish connection");

  if (client.connect(server, PORT))
  {
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

temp_hum readTempHum(void)
{

  temp_hum result;

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  result.temperature = t;
  result.humidity = h;
  return (result);
}

float readWaterTemp(void)
{
  sensors.requestTemperatures();
  return (sensors.getTempCByIndex(0));
}

float readPH(void)
{
  unsigned long int avgValue; //Store the average value of the sensor feedback
  float b;
  int buf[10], temp;

  for (int i = 0; i < 10; i++)
  {
    buf[i] = analogRead(SensorPin);
    delay(10);
  }
  for (int i = 0; i < 9; i++) //sort the analog from small to large
  {
    for (int j = i + 1; j < 10; j++)
    {
      if (buf[i] > buf[j])
      {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }
  avgValue = 0;
  for (int i = 2; i < 8; i++) //take the average value of 6 center sample
    avgValue += buf[i];
  float phValue = (float)avgValue * 5.0 / 1024 / 6; //convert the analog into millivolt
  return (3.5 * phValue);                           //convert the millivolt into pH value
}

void WIFILoadInfo(void)
{
  uint8_t ssid_length = EEPROM.read(1);
  uint8_t pwd_length = EEPROM.read(32);

  for (uint8_t addr = 2; addr < ssid_length + 2; addr++)
  {
    ssid[addr - 2] = EEPROM.read(addr);
  }

  for (uint8_t addr = 33; addr < pwd_length + 33; addr++)
  {
    pwd[addr - 33] = EEPROM.read(addr);
  }
}

void WIFIStoreInfo(char *ssid_, char *pwd_)
{
  uint8_t ssid_length = strlen(ssid_);
  uint8_t pwd_length = strlen(pwd_);

  EEPROM.write(1, ssid_length);
  for (uint8_t addr = 2; addr < ssid_length + 2; addr++)
  {
    EEPROM.write(addr, ssid_[addr - 2]);
  }

  EEPROM.write(32, pwd_length);
  for (uint8_t addr = 33; addr < pwd_length + 33; addr++)
  {
    EEPROM.write(addr, pwd_[addr - 33]);
  }
}

void printWifiData()
{
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(WiFi.status());
}

void createAP(void)
{
  bool wait_for_cinnection = true;

  status = WiFi.beginAP("Arduino AP", "12345");
  if (status != WL_AP_LISTENING)
  {
    Serial.println("Creating access point failed");
    // don't continue
    while (true)
      ;
  }

  // wait 10 seconds for connection:
  delay(10000);

  // start the web server on port 80
  ap.begin();

  while (wait_for_cinnection)
  {
    WiFiClient client = ap.available(); // listen for incoming clients

    if (client)
    {
      Serial.println("new client"); // print a message out the serial port
      String currentLine = "";      // make a String to hold incoming data from the client
      while (client.connected())
      { // loop while the client's connected
        if (client.available())
        {                         // if there's bytes to read from the client,
          char c = client.read(); // read a byte, then
          Serial.write(c);        // print it out the serial monitor
          if (c == '\n')
          { // if the byte is a newline character

            // if the current line is blank, you got two newline characters in a row.
            // that's the end of the client HTTP request, so send a response:
            if (currentLine.length() == 0)
            {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println();
              client.println("<HTML>");
              client.println("<HEAD>");
              client.println("<TITLE>Arduino GET test page</TITLE>");
              client.println("</HEAD>");
              client.println("<BODY>");

              client.println("<H1>HTML form GET example</H1>");

              client.println("<FORM ACTION='/' method=get >"); //uses IP/port of web page

              // the content of the HTTP response follows the header:
              client.print("<label for=\"ssid\">SSID:</label>");
              client.print("<input type=\"text\" id=\"ssid\" name=\"ssid\"><br><br>");
              client.print("<label for=\"pwd\">Pasword:</label>");
              client.print("<input type=\"text\" id=\"pwd\" name=\"pwd\"><br><br>");
              client.println("<INPUT TYPE=SUBMIT NAME='submit' VALUE='SSID AND PWD'>");

              client.println("</FORM>");

              client.println("<BR>");

              client.println("</BODY>");
              client.println("</HTML>");
              // The HTTP response ends with another blank line:
              client.println();
              // break out of the while loop:
              break;
            }
            else
            { // if you got a newline, then clear currentLine:
              currentLine = "";
            }
          }
          else if (c != '\r')
          {                   // if you got anything else but a carriage return character,
            currentLine += c; // add it to the end of the currentLine
          }
        }
      }
      client.stop();
      Serial.println(currentLine);
    }
  }
}

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