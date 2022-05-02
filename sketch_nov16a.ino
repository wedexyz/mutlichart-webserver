#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>



String dataIn;
String dt[10];
int i;
boolean parsing=false;


const char* htmlfile = "/index.html";
IPAddress    apIP(42, 42, 42, 42);                              
const char *ssid = "WEDEX";
//const char *password = "12345678";
const char* host = "esp8266fs";
ESP8266WebServer server(80);





/*
void handleADC(){
  int a = analogRead(A0);
  a = map(a,0,1023,0,100);
  String adc = String(a);
  server.send(200, "text/plane",adc);
}
*/
void parsingData()
{
      int j=0;
      dt[j]="";
      for(i=1;i<dataIn.length();i++)
      {
             //pengecekan tiap karakter dengan karakter (#) dan (,)
             if ((dataIn[i] == '#') || (dataIn[i] == ','))
             {
                   //increment variabel j, digunakan untuk merubah index array penampung
                   j++;
                  dt[j]=""; //inisialisasi variabel array dt[j]
             }
             else
             {
                  //proses tampung data saat pengecekan karakter selesai.
                  dt[j] = dt[j] + dataIn[i];
             }
      }
      Serial.print("data 2 : ");
      Serial.print(dt[1]);
      Serial.print("\n");
      Serial.print("data 3 : ");
      Serial.print(dt[2]);
      Serial.print("\n");
      Serial.print("data 4 : ");
      Serial.print(dt[3]);
      Serial.print("\n");
   
     //String adc = String(dt[1].toInt());
     //server.send(200, "text/plane",adc);
     
  String data = "{\"ADC\":\""+String(dt[1].toInt())+"\", \"Temperature\":\""+ String(dt[2]) +"\",\"Humidity\":\""+ String(dt[3]) +"\"}";
  server.send(200, "text/plane", data);
}



void handleRoot(){
  server.sendHeader("Location", "/index.html",true);  
  server.send(302, "text/plane","");

}

void handleWebRequests(){
  if(loadFromSpiffs(server.uri())) return;
  String message = "File Not Detected\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " NAME:"+server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  Serial.println(message);
}

void setup() {

  Serial.begin(115200);
  dataIn="";
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));   
  WiFi.softAP(ssid
  //, password
  );
 
  //Initialize File System
  SPIFFS.begin();
  Serial.println("File System Initialized");

  
  IPAddress myIP = WiFi.softAPIP();
  //Initialize Webserver
  server.on("/",handleRoot);
  server.on("/readADC",parsingData); 

  server.onNotFound(handleWebRequests);
 
  server.begin();  
}

void loop() {
 server.handleClient();
 if(Serial.available()){
           char inChar = (char)Serial.read();
           dataIn += inChar;
           if (inChar == '\n') {parsing = true;}
     }
if(parsing){
           parsingData();
   
           parsing=false;
           dataIn="";
     }
}

bool loadFromSpiffs(String path){
  String dataType = "text/plain";
  if(path.endsWith("/")) path += "index.htm";

  if(path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
  else if(path.endsWith(".html")) dataType = "text/html";
  else if(path.endsWith(".htm")) dataType = "text/html";
  else if(path.endsWith(".css")) dataType = "text/css";
  else if(path.endsWith(".js")) dataType = "application/javascript";
  else if(path.endsWith(".png")) dataType = "image/png";
  else if(path.endsWith(".gif")) dataType = "image/gif";
  else if(path.endsWith(".jpg")) dataType = "image/jpeg";
  else if(path.endsWith(".ico")) dataType = "image/x-icon";
  else if(path.endsWith(".xml")) dataType = "text/xml";
  else if(path.endsWith(".pdf")) dataType = "application/pdf";
  else if(path.endsWith(".zip")) dataType = "application/zip";
  File dataFile = SPIFFS.open(path.c_str(), "r");
  if (server.hasArg("download")) dataType = "application/octet-stream";
  if (server.streamFile(dataFile, dataType) != dataFile.size()) {
  }

  dataFile.close();
  return true;
}
