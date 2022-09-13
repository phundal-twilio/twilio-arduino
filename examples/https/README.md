# HTTPS API example for LilyGo board using LTE

## Prerequisite libraries required in the Arduino IDE
Within the Arduino IDE, open **Tools > Manage Libraries...**

**Add the following Libraries:**
* SSLClient
* ArduinoHttpClient
* URLEncode
* ArduinoJson

## Configuration

 1. Copy over ca_cert.h file to with the correct HTTPS Trush Anchors you want to use.  In this example, the ca_cert.h is the TA for https://api.openweathermap.org/
    1. Open https://openslab-osu.github.io/bearssl-certificate-utility/
    2. Enter the URL your device will attach to with HTTPS.  In this example, I used https://api.openweathermap.org/
    3. Copy and past the resulting text into the ca_cert.h file
 2. Set GPRS APN ("super" if you are using Twilio Super SIM)
 3. Set endpoint server credentials
    1. Obtain an API key from https://home.openweathermap.org/users/sign_up
    2. Enter your key into weatherApiKey
 4. Within the resource path, set the postal code you would like to retrieve weather from (94598 in this example)

## Troubleshooting
* To be able to see all the AT commands being run behind the scenes, uncomment line 17. (**#define DUMP_AT_COMMANDS**)
