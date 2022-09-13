/*
 * Example of initiating HTTPS sessions via LTE connectivity
 *
 * Critical to making this work:
 * 1. Setup ca_cert.h file to represent HTTPS endpoint you want to use
 * 2. Set GPRS APN
 * 3. Set Endpoint server credentials
 * 4. Setup HTTPS request (GET/POST) as your application requires
 *
 */

#define TINY_GSM_MODEM_SIM7000
#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb
#define SerialAT Serial1

// See all AT commands as they are being run (this is how the sausage is made, very noisy)
//#define DUMP_AT_COMMANDS

/*
 * Sections enabled
 */
#define TINY_GSM_TEST_GPRS    true

// set GSM PIN, if any
#define GSM_PIN ""


/*
 * Your GPRS credentials, if any
 */
const char apn[]  = "";     //SET TO YOUR APN
const char gprsUser[] = "";
const char gprsPass[] = "";


/*
 * Endpoint server details
 *
 */

const char weatherApiKey[] = ""; // Get a API key here: https://home.openweathermap.org/users/sign_up
const char server[] = "api.openweathermap.org";
const String resource = "/data/2.5/weather?zip=94598,US&units=imperial&appid=" + String(weatherApiKey); // Change zip code below for your region
const int  port = 443;


// Support libraries
#include <SSLClient.h>
#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>
#include <SPI.h>
#include <SD.h>
#include <Ticker.h>



/*
 * The file below you will need to create based on the website you would like to connect to securely.
 * This file contains the trust anchor to ensure the site you are connecting to is valid.
 *
 * Use the website: https://openslab-osu.github.io/bearssl-certificate-utility/
 * Enter the website to pull the certs from the URL, cut and past the result into the file below.
 */

#include "ca_cert.h" // This TA is specifically for https://api.openweathermap.org/




#ifdef DUMP_AT_COMMANDS  // if enabled it requires the streamDebugger lib
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, Serial);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif


/*
 * HTTPS Transport setup: This next section sets up for HTTPS traffic.
 */

TinyGsmClient modem_client(modem);
SSLClient secure_presentation_layer(modem_client, TAs, (size_t)TAs_NUM, A7);
HttpClient https_client = HttpClient(secure_presentation_layer, server, port);


/*
 * Set a bunch of necessary PIN outs to make modem operate correctly.  Don't mess with this section unless you know what you are doing.
 */

#define uS_TO_S_FACTOR 1000000ULL  // Conversion factor for micro seconds to seconds
#define TIME_TO_SLEEP  60          // Time ESP32 will go to sleep (in seconds)

#define UART_BAUD   115200
#define PIN_DTR     25
#define PIN_TX      27
#define PIN_RX      26
#define PWR_PIN     4

#define SD_MISO     2
#define SD_MOSI     15
#define SD_SCLK     14
#define SD_CS       13
#define LED_PIN     12


/*
 * Let's get going!
 */

void setup()
{
    // Set console baud rate for Serial Window from IDE
    Serial.begin(115200);
    delay(10);

    // Set LED OFF
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);

    // Reset Modem
    pinMode(PWR_PIN, OUTPUT);
    digitalWrite(PWR_PIN, HIGH);
    delay(300);
    digitalWrite(PWR_PIN, LOW);

    // Check if SD card present
    Serial.println("\n\nChecking for SD Card...");
    SPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
    if (!SD.begin(SD_CS)) {
        Serial.println("No SD Card found.");
    } else {
        uint32_t cardSize = SD.cardSize() / (1024 * 1024);
        String str = "SDCard Size: " + String(cardSize) + "MB";
        Serial.println(str);
    }

    Serial.println("\nWait 8 seconds for bootstrap to complete...");
    delay(8000);

    SerialAT.begin(UART_BAUD, SERIAL_8N1, PIN_RX, PIN_TX);

    // Restart takes quite some time
    // To skip it, call init() instead of restart()
    Serial.println("Initializing modem...");
    if (!modem.restart()) {
        Serial.println("Failed to restart modem, attempting to continue without restarting");
    }
}



/*
 * Start main loop
 */

void loop()
{
    String name = modem.getModemName();
    delay(500);
    Serial.println("Modem Name: " + name);

    String modemInfo = modem.getModemInfo();
    delay(500);
    Serial.println("Modem Info: " + modemInfo);

    // Turn of GPS for this example as it is not needed
    // Set SIM7000G GPIO4 LOW ,turn off GPS power
    // CMD:AT+SGPIO=0,4,1,0
    // Only in version 20200415 is there a function to control GPS power
    modem.sendAT("+SGPIO=0,4,1,0");
    if (modem.waitResponse(10000L) != 1) {
        DBG(" SGPIO=0,4,1,0 false ");
    }

#if TINY_GSM_TEST_GPRS
    // Unlock your SIM card with a PIN if needed
    if ( GSM_PIN && modem.getSimStatus() != 3 ) {
        modem.simUnlock(GSM_PIN);
        DBG("SIM Check...");
    }
#endif


    /*
     * setNetworkMode:
      2 Automatic
      13 GSM only
      38 LTE only
      51 GSM and LTE only
     */
    String res;
    res = modem.setNetworkMode(38);
    if (res != "1") {
        DBG("setNetworkMode  false ");
        return ;
    }
    delay(200);

    /*
     * setPreferredMode:
      1 CAT-M
      2 NB-Iot
      3 CAT-M and NB-IoT
    */
    res = modem.setPreferredMode(1);
    if (res != "1") {

        DBG("setPreferredMode  false ");
        return ;
    }
    delay(200);



Serial.println("\n\n\nWaiting for network...");
if (!modem.waitForNetwork()) {
    delay(10000);
    return;
}

if (modem.isNetworkConnected()) {
    Serial.println("Network connected");
}

Serial.println("\n---Setting up GPRS connection---\n");
Serial.println("Connecting to: " + String(apn));
if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    delay(10000);
    return;
}

Serial.print("GPRS status: ");
if (modem.isGprsConnected()) {
    Serial.println("connected!");
} else {
    Serial.println("not connected");
}

//Serial.print("Adding 8 sec delay for connectivity");
//delay(8000);

      String cop = modem.getOperator();
      Serial.println("Operator:" + String(cop));

      int csq = modem.getSignalQuality();
      Serial.println("Signal quality:" + String(csq) + "\n\n");
/*
 * Start HTTPS request
 */
Serial.println("Building HTTPS request");
https_client.beginRequest();
https_client.get(resource);
// The next couple lines are to support a POST transaction. Use that which applies to your application
//String jsonData = "{\"some\": \"data\"}";
//https_client.post(resource);
//https_client.sendHeader("Authorization", "Bearer " + String(KEY));
//https_client.sendHeader("Content-Type", "application/json");
//https_client.sendHeader("Content-Type", "application/x-www-form-urlencoded");
//https_client.sendHeader("Content-Length", jsonData.length());
//https_client.print(jsonData);
Serial.print("Sending Request\n");
https_client.endRequest();


// read the status code and body of the response
Serial.print("\nAwaiting response...\n\n");

int statusCode = https_client.responseStatusCode();
String response = https_client.responseBody();
Serial.print("Status code: ");
Serial.println(statusCode);
Serial.print("Response: ");
Serial.println(response);


Serial.print("\nWaiting 60 seconds to make the call again...");
delay(60000); // Wait 60 seconds before going again



#if TINY_GSM_TEST_GPRS
    modem.gprsDisconnect();
    if (!modem.isGprsConnected()) {
        Serial.println("GPRS disconnected");
    } else {
        Serial.println("GPRS disconnect: Failed.");
    }
#endif

Serial.println("Putting modem to sleep");
esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
delay(200);
esp_deep_sleep_start();

// Do nothing forevermore
while (true) {
  modem.maintain();
}
}
