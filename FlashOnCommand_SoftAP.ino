/**
 * ESP8266 Flash On Command (SoftAP version) by Michael White
 * Flash the blue LED on the board when message recieved via UDP
 */
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

//SSID of network
char ssid[] = "WeMos"; //SSID for created access point
char pass[] = "password"; //Password for created access point

//UDP variables
WiFiUDP Udp;
unsigned int localUdpPort = 4210;
char incomingPacket[255];
char replyPacket[] = "got the message!";

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(115200);
  delay(10);

  // Create SoftAP
  Serial.println();
  Serial.println();
  Serial.println("Creating soft AP...");
  Serial.printf("SSID: %s\n", ssid);
  Serial.printf("Password: %s\n", pass);
  WiFi.softAP(ssid, pass);
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

  // Start UDP and print port
  Udp.begin(localUdpPort);
  Serial.print("UDP port connected! Port: ");
  Serial.println(localUdpPort);
}

void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);
    
    // send back a reply, to the IP address and port we got the packet from
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(replyPacket);
    Udp.endPacket();

    // blink LED
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    digitalWrite(LED_BUILTIN, HIGH);
  }
}
