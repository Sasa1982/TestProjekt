// function defaults
//String listFiles(bool ishtml = false);
// list all of the files, if ishtml=true, return html rather than simple text
#include "Arduino.h"


#include "FS.h"
#include "FFat.h"
#include "SD.h"
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include "webserver.h"


AsyncWebServer server(80);
bool shouldReboot = false;    
const String default_httpuser = "admin";
const String default_httppassword = "admin";
// Make size of files human readable
// source: https://github.com/CelliesProjects/minimalUploadAuthESP32



//************** PLATZHALTER BZW KoNSTATEN FÜR WEBSERVER */

int kategorie_anzahl_1=12;
int kategorie_anzahl_2=22;
int kategorie_anzahl_3=13;
int kategorie_anzahl_4=6;
int kategorie_anzahl_5=8;
int kategorie_anzahl_6=88;



//#############################################################

String humanReadableSize(const size_t bytes)
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

String processor(const String& var) {
  if (var == "SWVERSION") {
    return "v1.1.1";
  }

  if (var == "CHIPID") {
    return  "123456";
  }
  if (var == "FREEFFAT") {
    return humanReadableSize((SD.totalBytes() - SD.usedBytes()));
  }

  if (var == "USEDFFAT") {
    return humanReadableSize(SD.usedBytes());
  }

  if (var == "TOTALFFAT") {
    return humanReadableSize(SD.totalBytes());
  }
}

// String listFiles(bool ishtml,const char *path)
// {
//   String returnText = "";
//  Serial.print("Listing files stored on FFat");
//   File root = SD.open(path);
//   File foundfile = root.openNextFile();
//   if (ishtml)
//   {
//     returnText += "<table><tr><th align='left'>Name</th><th align='left'>Size</th><th></th><th></th></tr>";
//   }
//   while (foundfile)
//   {
//     if (ishtml)
//     {
//       returnText += "<tr align='left'><td>" + String(foundfile.name()) + "</td><td>" + humanReadableSize(foundfile.size()) + "</td>";
//       returnText += "<td><button onclick=\"downloadDeleteButton(\'" + String(path)+ "/"+ String(foundfile.name()) + "\', \'download\')\">Download</button>";
//       returnText += "<td><button onclick=\"downloadDeleteButton(\'" + String(path)+ "/"+ String(foundfile.name()) + "\', \'delete\')\">Delete</button></tr>";
//     }
//     else
//     {
//       returnText += "File: " + String(foundfile.name()) + " Size: " + humanReadableSize(foundfile.size()) + "\n";
//     }
//     foundfile = root.openNextFile();
//   }
//   if (ishtml)
//   {
//     returnText += "</table>";
//   }
//   root.close();
//   foundfile.close();
//   return returnText;
// }


String pathUpload ="/report/";

String listFiles(bool ishtml,const char *path) // Boostarp
{
  String returnText = "";
 Serial.print("Listing files stored on FFat");
  File root = SD.open(path);
  File foundfile = root.openNextFile();
  if (ishtml)
  {
    returnText += "<table class='table table-striped'><thead><tr><th scope='col'>Name</th><th scope='col'>Size</th>  <th scope='col'></th>  <th scope='col'></th> </tr></thead>";
  }
  while (foundfile)
  {
    if (ishtml)
    {
      returnText += "<tr align='left'><td>" + String(foundfile.name()) + "</td><td>" + humanReadableSize(foundfile.size()) + "</td>";
      returnText += "<td><button type='button' class='btn btn-primary'onclick=\"downloadDeleteButton(\'" + String(path)+ "/"+ String(foundfile.name()) + "\', \'download\')\">Download</button>";
      returnText += "<td><button type='button' class='btn btn-primary'onclick=\"downloadDeleteButton(\'" + String(path)+ "/"+ String(foundfile.name()) + "\', \'delete\')\">Delete</button></tr>";
    }
    else
    {
      returnText += "File: " + String(foundfile.name()) + " Size: " + humanReadableSize(foundfile.size()) + "\n";
    }
    // else
    // {
    // returnText += "<div class='alert alert-primary' role='alert'>" + String(foundfile.name()) +"</div>";
    // }
        foundfile = root.openNextFile();
  }
  if (ishtml)
  {
    returnText += "</table>";
  }
  root.close();
  foundfile.close();
  return returnText;
}


// handles uploads to the filserver
void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
  // make sure authenticated before allowing upload
//   if (checkUserWebAuth(request))
//   {
    String logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();
   Serial.print(logmessage);

    if (!index)
    {
      logmessage = "Upload Start: " + String(filename);
      // open the file on first call and store the file handle in the request object
      request->_tempFile = SD.open(pathUpload  + filename, "w");
     Serial.print(logmessage);
    }

    if (len)
    {
      // stream the incoming chunk to the opened file
      request->_tempFile.write(data, len);
      logmessage = "Writing file: " + String(filename) + " index=" + String(index) + " len=" + String(len);
     Serial.print(logmessage);
    }

    if (final)
    {
      logmessage = "Upload Complete: " + String(filename) + ",size: " + String(index + len);
      // close the file handle as the upload is now done
      request->_tempFile.close();
     Serial.print(logmessage);
      request->redirect("/");
    }
//   }
//   else
//   {
//    Serial.print("Auth: Failed");
//     return request->requestAuthentication();
//   }
}

bool checkUserWebAuth(AsyncWebServerRequest *request)
{
  bool isAuthenticated = false;

  if (request->authenticate(default_httpuser.c_str(),  default_httpuser.c_str()))
  {
   Serial.print("is authenticated via username and password");
    isAuthenticated = true;
  }
  return isAuthenticated;
}

void notFound(AsyncWebServerRequest *request)
{
  String logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();
 Serial.print(logmessage);
  request->send(404, "text/plain", "<div class='alert alert-danger' role='alert'>'Keine Dateien'</div>");
}

void configureWebServer()
{
  // configure web server

  // if url isn't found
  server.onNotFound(notFound);

  // run handleUpload function when any file is uploaded
  server.onFileUpload(handleUpload);

  server.on("/assets/bootstrap/css/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SD, "/assets/bootstrap/css/bootstrap.min.css", "text/css"); });
            
 
  server.on("/assets/js/chart.min.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SD, "/assets/js/chart.min.js", "text/javascript"); }); //chart
  server.on("/assets/js/bs-init.js", HTTP_GET, [](AsyncWebServerRequest *request)     
            { request->send(SD, "/assets/js/bs-init.js", "text/javascript"); });//chart
  server.on("/assets/bootstrap/js/bootstrap.min.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SD, "/assets/bootstrap/js/bootstrap.min.js", "text/javascript"); });

  server.on("/assets/js/auswertung.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SD, "/assets/js/auswertung.js", "text/javascript"); });

  server.on("/assets/js/jquery.min.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SD, "/assets/js/jquery.min.js", "text/javascript"); });

  server.on("/assets/js/index.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SD, "/assets/js/index.js", "text/javascript"); });

  server.on("/assets/js/theme.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SD, "/assets/js/theme.js", "text/javascript"); });
  // visiting this page will cause you to be logged out
  server.on("/logout", HTTP_GET, [](AsyncWebServerRequest *request)
             {
    request->requestAuthentication();
    request->send(401); });
char buf[8];

server.on("/anzahlCocktails", HTTP_GET, [](AsyncWebServerRequest *request)
          { request->send_P(200, "text/plain", String(kategorie_anzahl_1).c_str()); });

server.on("/anzahlAlkoholfrei", HTTP_GET, [](AsyncWebServerRequest *request)
          { request->send_P(200, "text/plain", String(kategorie_anzahl_2).c_str()); });

server.on("/anzahlLongdrinks", HTTP_GET, [](AsyncWebServerRequest *request)
          { request->send_P(200, "text/plain", String(kategorie_anzahl_3).c_str()); });

server.on("/anzahlShots", HTTP_GET, [](AsyncWebServerRequest *request)
          { request->send_P(200, "text/plain", String(kategorie_anzahl_4).c_str()); });

server.on("/anzahlSaft", HTTP_GET, [](AsyncWebServerRequest *request)
          { request->send_P(200, "text/plain", String(kategorie_anzahl_5).c_str()); });

server.on("/anzahlSonstige", HTTP_GET, [](AsyncWebServerRequest *request)
          { request->send_P(200, "text/plain", String(kategorie_anzahl_6).c_str()); });

server.on("/dashboard.html", HTTP_GET, [](AsyncWebServerRequest *request)
          {
    String logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();

    if (checkUserWebAuth(request)) {
      //request->send(200, "text/html", reboot_html);
      request->send(SD, "/dashboard.html", "text/html", false, processor);
      logmessage += " Auth: Success";
     Serial.print(logmessage);
      shouldReboot = true;
    } else {
      logmessage += " Auth: Failed";
     Serial.print(logmessage);
      return request->requestAuthentication();
    } });

server.on("/files.html", HTTP_GET, [](AsyncWebServerRequest *request)
          {
    String logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();

    if (checkUserWebAuth(request)) {
      //request->send(200, "text/html", reboot_html);
      request->send(SD, "/files.html", "text/html", false, processor);
      logmessage += " Auth: Success";
     Serial.print(logmessage);
      shouldReboot = true;
    } else {
      logmessage += " Auth: Failed";
     Serial.print(logmessage);
      return request->requestAuthentication();
    } });
// presents a "you are now logged out webpage
server.on("/logged-out", HTTP_GET, [](AsyncWebServerRequest *request)
          {
    String logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();
   Serial.print(logmessage);
   //request->send_P(401, "text/html", logout_html, processor);
    request->send(SD, "/logout.html", "text/html", false, processor); });

server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
          {
               String logmessage = "Client:" + request->client()->remoteIP().toString() + +" " + request->url();

               if (checkUserWebAuth(request))
               {
                 logmessage += " Auth: Success";
                Serial.print(logmessage);
                 //request->send_P(200, "text/html", index.html, processor);
                 request->send(SD, "/index.html", "text/html", false, processor);
               }
               else
               {
                 logmessage += " Auth: Failed";
                Serial.print(logmessage);
                 return request->requestAuthentication();
               } });

server.on("/reboot", HTTP_GET, [](AsyncWebServerRequest *request)
          {
    String logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();

    if (checkUserWebAuth(request)) {
      //request->send(200, "text/html", reboot_html);
      request->send(SD, "/reboot.html", "text/html", false, processor);
      logmessage += " Auth: Success";
     Serial.print(logmessage);
      shouldReboot = true;
    } else {
      logmessage += " Auth: Failed";
     Serial.print(logmessage);
      return request->requestAuthentication();
    } });

server.on("/listfilesRezepte", HTTP_GET, [](AsyncWebServerRequest *request)
          {
            pathUpload ="/rezepte/";
            String logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();
    if (checkUserWebAuth(request)) 
    {
      logmessage += " Auth: Success";
     Serial.print(logmessage);
      request->send(200, "text/plain", listFiles(true,"/rezepte"));
    } 
    else 
    {
      logmessage += " Auth: Failed";
     Serial.print(logmessage);
      return request->requestAuthentication();
    } });

server.on("/listfilesProducts", HTTP_GET, [](AsyncWebServerRequest *request)
          {
      pathUpload ="/products/";
      String logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();
      if (checkUserWebAuth(request)) {
        logmessage += " Auth: Success";
     Serial.print(logmessage);
      request->send(200, "text/plain", listFiles(true,"/products"));
    } 
    else 
    {
      logmessage += " Auth: Failed";
     Serial.print(logmessage);
      return request->requestAuthentication();
    } });

server.on("/listfilesReport", HTTP_GET, [](AsyncWebServerRequest *request)
          {
              pathUpload = "/report/";
              String logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();
              if (checkUserWebAuth(request))
              {
                logmessage += " Auth: Success";
               Serial.print(logmessage);
                request->send(200, "text/plain", listFiles(true, "/report"));
              }
              else
              {
                logmessage += " Auth: Failed";
               Serial.print(logmessage);
                return request->requestAuthentication();
              } });

server.on("/file", HTTP_GET, [](AsyncWebServerRequest *request)
          {
    String logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();
    if (checkUserWebAuth(request)) {
      logmessage += " Auth: Success";
     Serial.print(logmessage);

      if (request->hasParam("name") && request->hasParam("action")) {
        const char *fileName = request->getParam("name")->value().c_str();
        const char *fileAction = request->getParam("action")->value().c_str();

        logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url() + "?name=" + String(fileName) + "&action=" + String(fileAction);

        String filepath = "/" + String(fileName);
       Serial.print("***********Filename:");
       Serial.print(filepath);
        if (!SD.exists(filepath))
        {
         Serial.print(logmessage + " ERROR: file does not exist");
          request->send(400, "text/plain", "ERROR: file does not exist");
        }
        else
        {
         Serial.print(logmessage + " file exists");
          if (strcmp(fileAction, "download") == 0)
          {
            logmessage += " downloaded";
            request->send(SD, filepath, "application/octet-stream");
          }
          else if (strcmp(fileAction, "delete") == 0)
          {
            logmessage += " deleted";
            SD.remove(filepath);
            request->send(200, "text/plain", "<div class='alert alert-warning' role='alert'>"+String(filepath)+ "</div>"); 
            // Add Reload Buttons HM
          }
          else
          {
            logmessage += " ERROR: invalid action param supplied";
            request->send(400, "text/plain", "ERROR: invalid action param supplied");
          }
         Serial.print(logmessage);
        }
      } else {
        request->send(400, "text/plain", "ERROR: name and action params required");
      }
    } else {
      logmessage += " Auth: Failed";
     Serial.print(logmessage);
      return request->requestAuthentication();
    } });
server.begin();
}
