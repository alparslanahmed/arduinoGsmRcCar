// include the GSM library
#include <GSM.h>
#include <MQTT.h>

// PIN number if necessary
#define PINNUMBER ""

// APN information obrained from your network provider
#define apn       "internet" // replace with your GPRS APN
#define login     "internet"    // replace with your GPRS login
#define password  "t-mobile" // replace with your GPRS password

GSMClient net;
GPRS gprs;
GSM gsmAccess;
MQTTClient client;

unsigned long lastMillis = 0;

void connect() {
  Serial.print("\nconnecting...");
  while (!client.connect("arduino", "test", "test")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("/hello");
  // client.unsubscribe("/hello");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);

  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.
}
void setup() {
  Serial.begin(115200);

  // connection state
  bool connected = false;

  Serial.print("connecting to cellular network ...");

  // After starting the modem with gsmAccess.begin()
  // attach to the GPRS network with the APN, login and password
  while (!connected) {
    if ((gsmAccess.begin() == GSM_READY) &&
        (gprs.attachGPRS(apn, login, password) == GPRS_READY)) {
      connected = true;
    } else {
      Serial.print(".");
      delay(1000);
    }
  }

  Serial.print("\nconnecting...");
  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported
  // by Arduino. You need to set the IP address directly.
  client.setHost("deneme");
  client.begin("pee2556b.us-east-1.emqx.cloud",15829, net);
  client.onMessage(messageReceived);

  connect();
}
void loop() {
  client.loop();

  if (!client.connected()) {
    connect();
  }

  // publish a message roughly every second.
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    client.publish("/hello", "world");
  }
}
