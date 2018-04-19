/*
    This sketch sends data via HTTP GET requests to data.sparkfun.com service.

    You need to get streamId and privateKey at data.sparkfun.com and paste them
    below. Or just customize this script to talk to other HTTP servers.

*/


#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "C:\Users\Rafal\Documents\Arduino\myWifi.h"

//const char* ssid     = "your-ssid";
//const char* password = "your-password";
//const char* iftttKey = "secret";

// https://maker.ifttt.com/trigger/test/with/key/secret
const char* host = "maker.ifttt.com";
const char* eventId   = "test";

String url = String("https://") + host + "/trigger/" + eventId + "/with/key/" + iftttKey;

const char* rootCa = "-----BEGIN CERTIFICATE-----\n" \
                     "MIIEADCCAuigAwIBAgIBADANBgkqhkiG9w0BAQUFADBjMQswCQYDVQQGEwJVUzEh\n" \
                     "MB8GA1UEChMYVGhlIEdvIERhZGR5IEdyb3VwLCBJbmMuMTEwLwYDVQQLEyhHbyBE\n" \
                     "YWRkeSBDbGFzcyAyIENlcnRpZmljYXRpb24gQXV0aG9yaXR5MB4XDTA0MDYyOTE3\n" \
                     "MDYyMFoXDTM0MDYyOTE3MDYyMFowYzELMAkGA1UEBhMCVVMxITAfBgNVBAoTGFRo\n" \
                     "ZSBHbyBEYWRkeSBHcm91cCwgSW5jLjExMC8GA1UECxMoR28gRGFkZHkgQ2xhc3Mg\n" \
                     "MiBDZXJ0aWZpY2F0aW9uIEF1dGhvcml0eTCCASAwDQYJKoZIhvcNAQEBBQADggEN\n" \
                     "ADCCAQgCggEBAN6d1+pXGEmhW+vXX0iG6r7d/+TvZxz0ZWizV3GgXne77ZtJ6XCA\n" \
                     "PVYYYwhv2vLM0D9/AlQiVBDYsoHUwHU9S3/Hd8M+eKsaA7Ugay9qK7HFiH7Eux6w\n" \
                     "wdhFJ2+qN1j3hybX2C32qRe3H3I2TqYXP2WYktsqbl2i/ojgC95/5Y0V4evLOtXi\n" \
                     "EqITLdiOr18SPaAIBQi2XKVlOARFmR6jYGB0xUGlcmIbYsUfb18aQr4CUWWoriMY\n" \
                     "avx4A6lNf4DD+qta/KFApMoZFv6yyO9ecw3ud72a9nmYvLEHZ6IVDd2gWMZEewo+\n" \
                     "YihfukEHU1jPEX44dMX4/7VpkI+EdOqXG68CAQOjgcAwgb0wHQYDVR0OBBYEFNLE\n" \
                     "sNKR1EwRcbNhyz2h/t2oatTjMIGNBgNVHSMEgYUwgYKAFNLEsNKR1EwRcbNhyz2h\n" \
                     "/t2oatTjoWekZTBjMQswCQYDVQQGEwJVUzEhMB8GA1UEChMYVGhlIEdvIERhZGR5\n" \
                     "IEdyb3VwLCBJbmMuMTEwLwYDVQQLEyhHbyBEYWRkeSBDbGFzcyAyIENlcnRpZmlj\n" \
                     "YXRpb24gQXV0aG9yaXR5ggEAMAwGA1UdEwQFMAMBAf8wDQYJKoZIhvcNAQEFBQAD\n" \
                     "ggEBADJL87LKPpH8EsahB4yOd6AzBhRckB4Y9wimPQoZ+YeAEW5p5JYXMP80kWNy\n" \
                     "OO7MHAGjHZQopDH2esRU1/blMVgDoszOYtuURXO1v0XJJLXVggKtI3lpjbi2Tc7P\n" \
                     "TMozI+gciKqdi0FuFskg5YmezTvacPd+mSYgFFQlq25zheabIZ0KbIIOqPjCDPoQ\n" \
                     "HmyW74cNxA9hi63ugyuV+I6ShHI56yDqg+2DzZduCLzrTia2cyvk0/ZM/iZx4mER\n" \
                     "dEr/VxqHD3VILs9RaRegAhJhldXRQLIQTO7ErBBDpqWeCtWVYpoNz4iCxTIM5Cuf\n" \
                     "ReYNnyicsbkqWletNw+vHX/bvZ8=\n" \
                     "-----END CERTIFICATE-----\n";

void setup()
{
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

int value = 3;

#ifdef __cplusplus
extern "C" {
#endif
uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();

float temp() {
  return (temprature_sens_read() - 32) / 1.8;
}

void loop()
{
  delay(5000);
  --value;

  if (value < 0) return;

  // https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFiClientSecure/examples/WiFiClientSecure/WiFiClientSecure.ino
  WiFiClientSecure client;
  /* set SSL/TLS certificate */
  client.setCACert(rootCa);

  Serial.println("Connect to server via port 443");
  if (!client.connect(host, 443)) {
    Serial.println("Connection failed!");
    return;
  }
  Serial.println("Connected to server!");

  String data = String("{\"value1\":") + hallRead() + ",\"value2\":" + temp() + ",\"value3\":\"vv31\"}";
  /* create HTTP request */
  client.println(String("POST ") + url + " HTTP/1.1");
  client.println(String("Host: ") + host);
  client.println("Content-Type: application/json; charset=utf-8");
  client.println(String("Content-Length: ") + data.length());
  client.println();
  client.println(data);
  client.println();

  Serial.print("Waiting for response ");
  while (!client.available()) {
    delay(50); //
    Serial.print(".");
    }
  /* if data is available then receive and print to Terminal */
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  /* if the server disconnected, stop the client */
  if (!client.connected()) {
    Serial.println();
    Serial.println("Server disconnected");
    client.stop();
  }
}



