/*
  WiFiTelnetToSerial - Example Transparent UART to Telnet Server for esp8266

  Copyright (c) 2015 Hristo Gochkov. All rights reserved.
  This file is part of the ESP8266WiFi library for Arduino environment.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <ESP8266WiFi.h>


#define MOT_IZQ_A 16
#define MOT_IZQ_B 12
#define MOT_DER_A 13
#define MOT_DER_B 14

#define V 350
#define V_TURN 500
#define V_FAST 700

//how many clients should be able to telnet to this ESP8266
#define MAX_SRV_CLIENTS 1
const char* ssid = "cunaobot";
const char* password = "crmecatronica";

WiFiServer server(23);
WiFiClient serverClients[MAX_SRV_CLIENTS];

void setup() {
  delay(1000);

  pinMode(MOT_IZQ_A, OUTPUT);
  pinMode(MOT_IZQ_B, OUTPUT);
  pinMode(MOT_DER_A, OUTPUT);
  pinMode(MOT_DER_B, OUTPUT);

  digitalWrite(MOT_IZQ_A, LOW);
  digitalWrite(MOT_IZQ_B, LOW);
  digitalWrite(MOT_DER_A, LOW);
  digitalWrite(MOT_DER_B, LOW);

  
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("\nConnecting to "); Serial.println(ssid);
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20) {
    delay(500);
  }
  if (i == 21) {
    Serial.print("Could not connect to"); Serial.println(ssid);
    while (1) {
      delay(500);
    }
  }
  
  server.begin();
  server.setNoDelay(true);

  Serial.print("Ready! Use 'telnet ");
  Serial.print(WiFi.localIP());
  Serial.println(" 23' to connect");
}

void loop() {
  uint8_t i;
  //check if there are any new clients
  if (server.hasClient()) {
    for (i = 0; i < MAX_SRV_CLIENTS; i++) {
      //find free/disconnected spot
      if (!serverClients[i] || !serverClients[i].connected()) {
        if (serverClients[i]) {
          serverClients[i].stop();
        }
        serverClients[i] = server.available();
        Serial.print("New client: "); Serial.print(i);
        break;
      }
    }
    //no free/disconnected spot so reject
    if (i == MAX_SRV_CLIENTS) {
      WiFiClient serverClient = server.available();
      serverClient.stop();
      Serial.println("Connection rejected ");
    }
  }
  //check clients for data
  for (i = 0; i < MAX_SRV_CLIENTS; i++) {
    if (serverClients[i] && serverClients[i].connected()) {
      if (serverClients[i].available()) {
        //get data from the telnet client and push it to the UART
        while (serverClients[i].available()) {
          char c = serverClients[i].read();
          Serial.write(c);
          switch(c) {
            //Controles asdw (2 segunda marcha de w, x segunda marcha de s)
            case '2':
                analogWrite(MOT_IZQ_A, V_FAST);
                analogWrite(MOT_IZQ_B, 0);
                analogWrite(MOT_DER_A, V_FAST);
                analogWrite(MOT_DER_B, 0);
                break;
            case 'x':            
                analogWrite(MOT_IZQ_A, 0);
                analogWrite(MOT_IZQ_B, V_FAST);
                analogWrite(MOT_DER_A, 0);
                analogWrite(MOT_DER_B, V_FAST);
                break;
            case 'w':
                analogWrite(MOT_IZQ_A, V);
                analogWrite(MOT_IZQ_B, 0);
                analogWrite(MOT_DER_A, V);
                analogWrite(MOT_DER_B, 0);
                break;
            case 's':
                analogWrite(MOT_IZQ_A, 0);
                analogWrite(MOT_IZQ_B, V);
                analogWrite(MOT_DER_A, 0);
                analogWrite(MOT_DER_B, V);
                break;
            case 'd':
                analogWrite(MOT_IZQ_A, V_TURN);
                analogWrite(MOT_IZQ_B, 0);
                analogWrite(MOT_DER_A, 0);
                analogWrite(MOT_DER_B, V_TURN);
                break;                
            case 'a':
                analogWrite(MOT_IZQ_A, 0);
                analogWrite(MOT_IZQ_B, V_TURN);
                analogWrite(MOT_DER_A, V_TURN);
                analogWrite(MOT_DER_B, 0);
                break;  
                      
            //Controles jkli (8 segunda marcha de i, m segunda marcha de k)
            case 'm':
                analogWrite(MOT_IZQ_A, V_FAST);
                analogWrite(MOT_IZQ_B, 0);
                analogWrite(MOT_DER_A, V_FAST);
                analogWrite(MOT_DER_B, 0);
                break;
            case '8':            
                analogWrite(MOT_IZQ_A, 0);
                analogWrite(MOT_IZQ_B, V_FAST);
                analogWrite(MOT_DER_A, 0);
                analogWrite(MOT_DER_B, V_FAST);
                break;
            case 'k':
                analogWrite(MOT_IZQ_A, V);
                analogWrite(MOT_IZQ_B, 0);
                analogWrite(MOT_DER_A, V);
                analogWrite(MOT_DER_B, 0);
                break;
            case 'i':
                analogWrite(MOT_IZQ_A, 0);
                analogWrite(MOT_IZQ_B, V);
                analogWrite(MOT_DER_A, 0);
                analogWrite(MOT_DER_B, V);
                break;
            case 'l':
                analogWrite(MOT_IZQ_A, V_TURN);
                analogWrite(MOT_IZQ_B, 0);
                analogWrite(MOT_DER_A, 0);
                analogWrite(MOT_DER_B, V_TURN);
                break;                
            case 'j':
                analogWrite(MOT_IZQ_A, 0);
                analogWrite(MOT_IZQ_B, V_TURN);
                analogWrite(MOT_DER_A, V_TURN);
                analogWrite(MOT_DER_B, 0);
                break;
            // PARAR                      
            case ' ':
                analogWrite(MOT_IZQ_A, 0);
                analogWrite(MOT_IZQ_B, 0);
                analogWrite(MOT_DER_A, 0);
                analogWrite(MOT_DER_B, 0);
                break;
            
            default:
                Serial.write("Default");
          }
        }
      }
    }
  }
  //check UART for data
  if (Serial.available()) {
    size_t len = Serial.available();
    uint8_t sbuf[len];
    Serial.readBytes(sbuf, len);
    //push UART data to all connected telnet clients
    for (i = 0; i < MAX_SRV_CLIENTS; i++) {
      if (serverClients[i] && serverClients[i].connected()) {
        serverClients[i].write(sbuf, len);
        delay(1);
      }
    }
  }
}
