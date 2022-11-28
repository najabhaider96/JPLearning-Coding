/*
Welcome to JP Learning
*/
#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Manual IP Configuration
IPAddress ip(192, 168, 0, 190);
IPAddress myDns(192, 168, 0, 1);
IPAddress myGateway(192, 168, 0, 1);
IPAddress mySubnet(255, 255, 255, 0);


char* serverHost = "jiyepakistanlearning.herokuapp.com";
String requestUrl = "/api/v1/test";

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\nWelcome to JP Learning\n");
  Ethernet.begin(mac);
  //  Ethernet.begin(mac, ip);
  //  Ethernet.begin(mac, ip, myDns, myGateway);
  //  Ethernet.begin(mac, ip, myDns, myGateway, mySubnet);

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  delay(2000);
  Serial.print("Ethernet IP address is: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  EthernetClient GET_CLIENT;
  Serial.println("\n\nGetting from " + String(serverHost) + requestUrl);
  if (GET_CLIENT.connect(serverHost, 80)) {
    Serial.println("Server Connected for Get");
    GET_CLIENT.println("GET " + requestUrl + " HTTP/1.1");
    GET_CLIENT.println("Host: " + String(serverHost));
    GET_CLIENT.println("Connection: keep-alive");
    GET_CLIENT.println();

    while (GET_CLIENT.connected()) {
      String response = GET_CLIENT.readString();
      Serial.println("response: " + response);
      break;
    }
    GET_CLIENT.stop();
  } else
    Serial.println("Server Not Connected for Get");
  delay(1000);
}
