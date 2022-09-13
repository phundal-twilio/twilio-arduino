# Twilio Sync example for LilyGo board using LTE

## Prerequisite libraries required in the Arduino IDE
Within the Arduino IDE, open `Tools > Manage Libraries...`

**Add the following Libraries:**
* SSLClient
* ArduinoHttpClient
* URLEncode
* ArduinoJson

## Configuration

 1. Copy over the `ca_cert.h` file with the proper Trust Anchor (TA) certificates you want to use.  In the example given, the `ca_cert.h` is the TA for https://sync.twilio.com
 2. Set the APN for the SIM you are using
    1. `super` if you are using Twilio Super SIM
 3. Set endpoint server credentials
    1. Obtain an API key from https://www.twilio.com/try-twilio
    2. Under `Account > Auth Tokens & API Keys`, create a new set of API keys. More on how to do this here: [API Key Resource](https://www.twilio.com/docs/iam/keys/api-key).
    3. For the new API Keys, copy the `key` and `secret` into `twilioAPIKey` and `twilioAPISecret`, respectively.  Alternatively (but not recommended), you can use your `Account SID` and `Auth Token`.
 4. In Twilio Sync, create a `new Sync Service` and `new Sync List`.  More on how to do this here: [Service Resource](https://www.twilio.com/docs/sync/api/service).
 5. Within the resource path, set the appropriate Sync service SID and Sync List SID.  More on how this URL is formatted here: [Update a ListItem resource](https://www.twilio.com/docs/sync/api/listitem-resource#update-a-listitem-resource).

## Troubleshooting
* To be able to see all the AT commands being run behind the scenes, uncomment line 17: `#define DUMP_AT_COMMANDS`
* To check your the syntax of your payload, I like to use a service such as [webhook.site](https://webhook.site), setup a free account and start a listener.  Once this has been started, uncomment `#define TEST_URL_PAYLOAD`, and enter your listener URL path as the resource combination after the comment at `//Check payload at webhook.site`.
