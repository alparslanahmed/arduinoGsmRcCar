// include the GSM library
#include <GSM.h>
#include <ArduinoMqttClient.h>

// PIN number if necessary
#define PINNUMBER ""

// APN information obrained from your network provider
#define apn       "internet" // replace with your GPRS APN
#define login     "internet"    // replace with your GPRS login
#define password  "t-mobile" // replace with your GPRS password

GSMClient net;
GPRS gprs;
GSM gsmAccess;
MqttClient mqttClient(net);

//set interval for sending messages (milliseconds)

const long interval = 8000;

unsigned long previousMillis = 0;


int count = 0;


const char broker[] = "";

int        port     = 1883;

const char topic[]  = "deneme_kanal";

const char topic2[]  = "deneme_kanal_2";

const char topic3[]  = "deneme_kanal_3";

const char forwards[] = "w";
const char backwards[] = "s";
const char left[] = "a";
const char right[] = "d";

int steer = 1;
int gas = 1;

const int leftF = 8;
const int leftB = 9;
const int rightF = 10;
const int rightB = 11;

unsigned long lastMillis = 0;

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);

  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.
}
void setup() {

  pinMode(leftF, OUTPUT);
  pinMode(leftB, OUTPUT);
  pinMode(rightF, OUTPUT);
  pinMode(rightB, OUTPUT);
  
  Serial.begin(115200);


  digitalWrite(leftF, LOW);
  digitalWrite(leftB, LOW);
  digitalWrite(rightF, LOW);
  digitalWrite(rightB, LOW);
      
  // connection state
  bool connected = false;

  Serial.print("connecting to cellular network ...");

  // After starting the modem with gsmAccess.begin()
  // attach to the GPRS network with the APN, login and password
  while (!connected) {
    if ((gsmAccess.begin() == GSM_READY) &&
        (gprs.attachGPRS(apn, login, password) == GPRS_READY)) {
      connected = true;
      Serial.print("\nGSM READY!!!");
    } else {
      Serial.print(".");
      delay(1000);
    }
  }

  Serial.print("\nconnecting...");

  Serial.print("Attempting to connect to the MQTT broker: ");

  Serial.println(broker);


  if (!mqttClient.connect(broker, port)) {

    Serial.print("MQTT connection failed! Error code = ");

    Serial.println(mqttClient.connectError());


    while (1);

  }
  
  mqttClient.onMessage(onMqttMessage);

  Serial.println("You're connected to the MQTT broker!");
  mqttClient.subscribe("deneme_kanal");
  Serial.print("Waiting for messages");

}
void loop() {
  mqttClient.poll();
}


void onMqttMessage(int messageSize) {
  // we received a message, print out the topic and contents
  Serial.println("Received a message with topic '");
  Serial.print(mqttClient.messageTopic());
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");

  // use the Stream interface to print the contents
  while (mqttClient.available()) {
  
    String data = mqttClient.readString();

    Serial.println(data);
    
    if(data.length() == 1 &&  data == forwards){
      gas += 1;

      if(gas > 2){
        gas = 2;
        }
    }

    if(data.length() == 1 &&  data == backwards){
      gas -= 1;

      if(gas < 0){
        gas = 0;
        }
    }

    if(data.length() == 1 &&  data == left){
       steer += 1;

      if(steer > 2){
        steer = 2;
        }
    }
    
    if(data.length() == 1 &&  data == right){
       steer -= 1;

      if(steer < 0){
        steer = 0;
      }
    }
  }

  if(gas == 0){
     digitalWrite(leftF, HIGH);
     digitalWrite(leftB, LOW);
  }

  if(gas == 1){
     digitalWrite(leftF, LOW);
     digitalWrite(leftB, LOW);
  }

  if(gas == 2){
     digitalWrite(leftF, LOW);
     digitalWrite(leftB, HIGH);
  }

  Serial.println(gas);

  if(steer == 0){
     digitalWrite(rightF, HIGH);
     digitalWrite(rightB, LOW);
  }

  if(steer == 1){
     digitalWrite(rightF, LOW);
     digitalWrite(rightB, LOW);
  }

  if(steer == 2){
     digitalWrite(rightF, LOW);
     digitalWrite(rightB, HIGH);
  }

  delay(250);

  digitalWrite(leftF, LOW);
  digitalWrite(leftB, LOW);
  digitalWrite(rightF, LOW);
  digitalWrite(rightB, LOW);

}
