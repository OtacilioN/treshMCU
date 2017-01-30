#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti WiFiMulti;


SoftwareSerial swSer(14, 12, false, 256);

void setup() {
  Serial.begin(115200);
  swSer.begin(115200);

  WiFiMulti.addAP("hackerdeiphone", "asenhaesenha");
  while(WiFiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
  }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    delay(500);

  Serial.println("\nSoftware serial test started");

  for (char ch = ' '; ch <= 'z'; ch++) {
    swSer.write(ch);
  }
  swSer.println("");

}

char buff;
String ID = "";
String acess = "";
String gambi = "";
void loop() {
  while (swSer.available() > 0) {
    ID = "";
    acess = "";
    
    buff =  swSer.read();
    while(buff != '\n'){
      ID.concat(String(buff));
      buff =  swSer.read();
      delay(200);
    }

    Serial.println(ID);

    buff =  swSer.read();
    while(buff != '\n'){
      acess.concat(String(buff));
      buff =  swSer.read();
      delay(200);
    }

    Serial.println(acess);

    delay(200);

    //
    const uint16_t port = 5000;
    const char * host = "192.168.43.14"; // ip or dns

    
    
    Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;

    if (!client.connect(host, port)) {
        Serial.println("connection failed");
        Serial.println("wait 5 sec...");
        delay(5000);
        return;
    }else{
      Serial.println("Client Connect maybeee"); 
    }

    // This will send the request to the server
    gambi.concat("GET ");
    gambi.concat("/init?ID=");
    gambi += ID.substring(0, (ID.length()-1));
    gambi.concat("&acess=");
    gambi += acess.substring(0, (acess.length()-1));
    gambi.concat(" HTTP/1.1");

    client.print(gambi);
    gambi = "";


    //read back one line from server
    String line = client.readStringUntil('\r');
    Serial.println(line);
    client.println(line);

    Serial.println("closing connection");
    client.stop();
    
    Serial.println("wait 5 sec...");
    delay(5000);

  }
}
