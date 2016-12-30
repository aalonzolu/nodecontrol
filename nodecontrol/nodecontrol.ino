#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// Nombre de este nodo
const char* NOMBRE = "NODE 1";

//WIFI
const char* ssid = "Casa Alonzo";  // nombre Wifi
const char* password = "LCDCA2016"; // clave Wifi

// PINES
const int pines[] = {
  D2,
  D7,
  // D3,
  // D4,
  }; 
// NOMBRE DE PINES 
const char* pinNombres[] = {
  "Led 1",
  "Led 2",
  "Led 3",
  "Led 4",
  "Led 5",
  }; 
 // Cantidad de pines habilitados
const int pinCount = 2;


// ALERTA!
// NO MODIFICAR NADA A PARTIR DE ESTA LINEA A MENOS QUE SEAS PRO!

String nodeIP="";
ESP8266WebServer server(80);
void handleRoot() {
  digitalWrite(LED_BUILTIN, 1);
  server.send(200, "text/plain", "Hola Mundo!");
  digitalWrite(LED_BUILTIN, 0);
}

void handleInfo(){
  String res = "["; 
  for(int i=0; i<pinCount;i++){
    res = res+"{'name':'"+pinNombres[i]+"','id':'"+i+"'}";
    if(i<(pinCount-1)){
      res=res+",";
    }
  }
  res = res+"]";
  //nodeIP
  res = "{'items':"+res+",'name':'"+NOMBRE+"','ip':'"+nodeIP+"'}";
  res.replace("'","\"");
  server.send(200, "application/json", res);
}

void handleAction(){
  String pin =  "-";
  String level = "-";
  String res="";
  for (uint8_t i=0; i<server.args(); i++){
    if(server.argName(i)=="pin"){
      pin = server.arg(i);
    }
    if(server.argName(i)=="level"){
      level = server.arg(i);
    }
  }
  if(pin!="-" && level!="-" && pin.toInt()<pinCount && pin.toInt()>=0){
    digitalWrite(pines[pin.toInt()], level.toInt());
    res = "{\"error_code\":\"0\",\"pin\":\""+pin+"\",\"level\":\""+level+"\",\"msg\":\"Success!\"}";
  }
  else{
    res = "{\"error_code\":\"1\",\"pin\":\""+pin+"\",\"level\":\""+level+"\",\"msg\":\"Error! "+pinCount+"\"}";
  }
  server.send(200, "application/json", res);
}

void handleNotFound(){
  digitalWrite(LED_BUILTIN, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(LED_BUILTIN, 0);
}

void setup(void){
  for(int i=0; i<sizeof(pines);i++){
    pinMode(pines[i], OUTPUT);
    digitalWrite(pines[i], 0);
  }
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  
  IPAddress ip = WiFi.localIP();
  nodeIP.concat(ip[0]);nodeIP.concat(".");
  nodeIP.concat(ip[1]);nodeIP.concat(".");
  nodeIP.concat(ip[2]);nodeIP.concat(".");
  nodeIP.concat(ip[3]);
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/info",handleInfo);
  server.on("/action",handleAction);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}
