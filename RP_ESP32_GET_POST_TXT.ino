//upload is working, 18.10.2021
//download is working, 19.10.2021

#include "SPIFFS.h"                       
#include "FS.h"
#include <WiFi.h>
#include <HTTPClient.h>


//Network
const char* ssid = "xxx";
const char* password = "xxx";


WiFiClient client;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //SPIFFS:
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
  }
  else{
    Serial.println("SPIFFS OK");
  }  
/*  
  if(!SPIFFS.format()){
    Serial.println("An Error has occurred while formatting SPIFFS");
  }
  else{
    Serial.println("SPIFFS formatted");
  }   
*/



  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  String filedata = get_txtFile_fromHTTP("http://www.xxx.de/filedown.txt");
  if(filedata.length() < 1){
    Serial.println("Download error!");
  }
  else{
    Serial.println("Download success!");
    //Serial.println(filedata);
    File file;
    file = SPIFFS.open("/filedown.txt", "w");
    if(!file){
      // File not found
      Serial.println("Failed to open file");
      } 
    else{
      file.println(filedata);
      file.close();
    }
  }
  
  sendFile("xxx.de", "/thaler1/upload.php", 80, "RPFile.txt", "/filedown.txt");
  
}

void loop() {
  // put your main code here, to run repeatedly:

}

//#####################################################################################################
String sendFile(String serverName, String serverPath, int serverPort, String filename, String fileDir ) {
  String getAll;
  String getBody;
  File root;
  //open file on sdcard to read
  root = SPIFFS.open(fileDir);
  if (!root) {
     Serial.println("can not open file!");
     return "SPIFFS_err";
  }
  
  Serial.println("Connecting to server: " + serverName);

  if (client.connect(serverName.c_str(), serverPort)) {
    Serial.println("Connection successful!");    
    String head = "--RalfFromThalerLader\r\nContent-Disposition: form-data; name=\"imageFile\"; filename=\"" + filename + "\"\r\nContent-Type: image/txt\r\n\r\n";
    String tail = "\r\n--RalfFromThalerLader--\r\n";

    size_t imageLen = root.size();
    Serial.print("File Size: ");
    Serial.println(imageLen);
    uint32_t extraLen = head.length() + tail.length();
    uint32_t totalLen = imageLen + extraLen;
  
    client.println("POST " + serverPath + " HTTP/1.1");
    client.println("Host: " + serverName);
    client.println("Content-Length: " + String(totalLen));
    client.println("Content-Type: multipart/form-data; boundary=RalfFromThalerLader");
    client.println();
    client.print(head);

    size_t fbLen = root.size();
    char Buffer[fbLen + 1];
    for(size_t n = 0; n < fbLen; n++){
      Buffer[n] = root.read();
      Buffer[n + 1] = '\0';
    }
    char *fbBuf = Buffer;
    
    //Serial.print("fbBuf: ");
    //Serial.println(fbBuf);
    
    for (size_t n=0; n<fbLen; n=n+1024) {
      if (n+1024 < fbLen) {
        client.write(fbBuf, 1024);
        fbBuf += 1024;
      }
      else if (fbLen%1024>0) {
        size_t remainder = fbLen%1024;
        client.write(fbBuf, remainder);
      }
    }   

    client.print(tail);
    
    int timoutTimer = 10000;
    long startTimer = millis();
    boolean state = false;
    
    while ((startTimer + timoutTimer) > millis()) {
      Serial.print(".");
      delay(100);      
      while (client.available()) {
        char c = client.read();
        if (c == '\n') {
          if (getAll.length()==0) { state=true; }
          getAll = "";
        }
        else if (c != '\r') { getAll += String(c); }
        if (state==true) { getBody += String(c); }
        startTimer = millis();
      }
      if (getBody.length()>0) { break; }
    }
    Serial.println();
    client.stop();
    Serial.println(getBody);
  }
  else {
    getBody = "Connection to " + serverName +  " failed.";
    Serial.println(getBody);
  }
  return getBody;
}

//#####################################################################################################
String get_txtFile_fromHTTP(String url){
  String payload = "";
  HTTPClient http;
  Serial.print("[HTTP] begin...\n");
  // configure traged server and url
  http.begin(url); //HTTP
  Serial.print("[HTTP] GET...\n");
  // start connection and send HTTP header
  int httpCode = http.GET();
  // httpCode will be negative on error
  if(httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.print("[HTTP] GET... code: ");
    Serial.println(httpCode);
    // file found at server
    if(httpCode == HTTP_CODE_OK) {
      payload = http.getString();
      //Serial.println(payload);
    }
  } 
  else {
  Serial.print("[HTTP] GET... failed, error: ");
  Serial.println(http.errorToString(httpCode).c_str());
  }
  http.end();
  return payload;
}
