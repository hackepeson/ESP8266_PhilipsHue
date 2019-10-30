#include <ESP8266WiFi.h>
#include <SPI.h>

IPAddress ip(192, 168, 0, 240);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

int light = 1;                                   // Number of the Hue light to be switched

const char hueHubIP[] = "192.168.0.101";       // Hue hub IP
const char hueUsername[] = "zCp-8WgOnEos2rJ7DHizJh3INQ55tgYTvNnq1IMQ";  // hue bridge username
const int hueHubPort = 80;

const char ssid[] = "S130";                      // Network SSID (name)
const char pass[] = "";                  // Network password

boolean onOffState = true;                       // To store actual on/off state of lights as reported by Hue bridge

WiFiClient client;

void getHue()
{
  if (client.connect(hueHubIP, hueHubPort))
  {
    client.print("GET /api/");
    client.print(hueUsername);
    client.print("/lights/");
    client.print(light);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(hueHubIP);
    client.println("Content-type: application/json");
    client.println("keep-alive");
    client.println();
    while (client.connected())
    {
      if (client.available())
      {
        client.findUntil("\"on\":", "\0");
        onOffState = (client.readStringUntil(',') == "true");
        break;
      }
    }
    client.stop();
  }
}

void setLight1Red()
{
  String command = {"on":true, "sat":254, "bri":254,"hue":10000};
  setHue(command);
  
}

void setHue(String command)
{
  if (client.connect(hueHubIP, hueHubPort))
  {
    client.print("PUT /api/");
    client.print(hueUsername);
    client.print("/lights/");
    client.print(light);
    client.println("/state HTTP/1.1");
    client.println("keep-alive");
    client.print("Host: ");
    client.println(hueHubIP);
    client.print("Content-Length: ");
    client.println(command.length());
    client.println("Content-Type: text/plain;charset=UTF-8");
    client.println();                             // Blank line before body
    client.println(command);
    client.stop();
  }
}


void setup()
{
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1);
  }
}

void loop()
{
  getHue();                                       // Lights 1 status

 setLight1Red();

  delay(10);
  //#ESP.deepSleep(0);
}
