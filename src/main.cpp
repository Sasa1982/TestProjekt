// #define APIKEY "b19cdd92-c65e-44a1-a310-1f6214afa931"
// #define SWVERSION "v@0.0.0.2"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <WiFi.h>
#include "FS.h"
#include "FFat.h"
#include "SD.h"
#include "device_FRAM.h"
#include "webserver.h"


#define I2C_SDA 19
#define I2C_SCL 20

#define FILESYSTEM SD

int freeSpace = 0, totalSpace = 0, usedSpace = 0;

const char *ssid = "SSID_EINGEBEN";
const char *password = "PW_EINGEBEN";

// const char *ssid = "yourssid";
// const char *password = "yourpasswd";
// String dataString =""; // holds the data to be written to the SD card

String humanReadableSizeUI(const size_t bytes)
{
  if (bytes < 1024)
    return String(bytes) + " B";
  else if (bytes < (1024 * 1024))
    return String(bytes / 1024.0) + " KB";
  else if (bytes < (1024 * 1024 * 1024))
    return String(bytes / 1024.0 / 1024.0) + " MB";
  else
    return String(bytes / 1024.0 / 1024.0 / 1024.0) + " GB";
}
///************++ New Touch********************



//###############################################
void createDir(fs::FS &fs, const char *path)
{
  if (!fs.exists(path))
  {
   Serial.print("Creating Dir: ");
   Serial.println(path);
    //Serial.printf("Creating Dir: %s\n", p);
    if (fs.mkdir(path))
    {
     Serial.println("Dir created");
    }
    else
    {
     Serial.println("mkdir failed");
    }
  }
}

void setup()
{

  Serial.begin(115200);
  delay(200);

  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(40000);

  init_fram();

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // scanI2Cdevices();

  Serial.print("Initialisiere SD..");
  if (!FILESYSTEM.begin(10)) // SD Card Fehler
  {
    Serial.println("Initialisierung fehlgeschlagen!");
    return;
  }
  else
    Serial.println("OK !");

  createDir(FILESYSTEM, "/assets");
  createDir(FILESYSTEM, "/assets/bootstrap");
  createDir(FILESYSTEM, "/assets/bootstrap/css");
  createDir(FILESYSTEM, "/assets/bootstrap/js");
  createDir(FILESYSTEM, "/assets/js");
  createDir(FILESYSTEM, "/report");
  createDir(FILESYSTEM, "/rezepte");
  createDir(FILESYSTEM, "/user");
  createDir(FILESYSTEM, "/products");

  Serial.println("**** Speicherplatz ****");
  Serial.print("Total space: ");
  totalSpace = FILESYSTEM.totalBytes();
  Serial.println(humanReadableSizeUI(totalSpace));

  Serial.print("used space: ");
  usedSpace = FILESYSTEM.usedBytes();
  Serial.println(humanReadableSizeUI(usedSpace));

  Serial.print("free space: ");
  freeSpace = totalSpace - usedSpace;
  Serial.println(humanReadableSizeUI(freeSpace));

  configureWebServer(); // Webserver starten
}

void loop()
  {



  }

  
  // test
//#####################################################################################################################
