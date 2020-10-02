#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>

//ADC max value = 4095 / 3.3V

//Pinout
#define led 2
#define bat_v 35
#define ecg_sensor 34
#define temp_sensor 23
#define exam_reset 22
#define loN 27
#define loP 28

//Wi-Fi
const char* ssid ="SSID";
const char* password="SECRETPASS"; 

//MQTT
const char* broker_mqtt= "192.168.1.103"; //Broker ip
int broker_port= 1883; // Broker port
#define id_mqtt "ecg_device"

//Global objects and variables
WiFiClient espClient;
PubSubClient mqtt(espClient);

#define DHTTYPE DHT11 // Define DHT sensor type
DHT dht(temp_sensor, DHTTYPE);


unsigned long prevTimerEcg =0; //Store last ECG send time
unsigned long prevTimerTemp =0; //Store last Temperature send time
unsigned long prevTimerBpm =0; //Store last BPM send time
int bpm_val=0;
float bat_past=0;


//Prototypes
 void pinInit();
 void serialInit();
 void wifiInit(); 
 void wifiConect();
 void mqttInit(); 
 void mqttReconect(); 
 void isConected(void); 
 void readEcgSignal();
 void readTemperature();
 void readBateryLevel();
 void resetExam();

//Setup
void setup() {

  pinInit();
  serialInit();
  wifiInit();
  mqttInit();
  
  bat_past= analogRead(bat_volt); // Read batery voltage on start
  char sendBuffer[6];
  dtostrf(bat_past,5,0,sendBuffer);   //Convert value to char array 
  mqtt.publish("BATTERY", sendBuffer); //Send MQTT message to broker (tópico, mensagem)  

}

//Pinout mode
void pinInit(){
  pinMode(ecg_sensor, INPUT);
  pinMode(loN, INPUT);
  pinMode(loP, INPUT);
  pinMode(temp_sensor, INPUT);
  pinMode(bat_volt, INPUT);
  pinMode(exam_reset, INPUT);
  pinMode(led,OUTPUT);
}

void serialInit(){
    Serial.begin(115200);
    Serial.println("");
    Serial.println("------- Serial Communication starting -------");
    Serial.println("Connection estabilished");
    Serial.println("--Baud: 115200");
    Serial.println("");
}

void wifiInit(){
  delay(10);
  Serial.println("-------Estabilishing Wi-Fi connection-------");
  Serial.print("Connecting to: ");
  Serial.println(ssid);
  Serial.print("Wait");

  wifiConect();
}

void wifiConect(){

  if(WiFi.status() == WL_CONNECTED){
    return;
  }
  else{
  WiFi.begin(ssid,password);
  digitalWrite(led,HIGH);
  while(WiFi.status() != WL_CONNECTED){
    delay(100);
    Serial.print(".");
  }
  digitalWrite(led,LOW);
  Serial.println("Connected");
  Serial.print("--IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("");
  }
}

void mqttInit(){
  Serial.println("-------Setting up MQTT Broker-------");
  mqtt.setServer(broker_mqtt,broker_port);
  Serial.print("--Broker: ");
  Serial.print(broker_mqtt);
  Serial.print(":");
  Serial.println(broker_port);
  Serial.println("");
}

void mqttReconect(){
  while(!mqtt.connected()){
    Serial.println("-------Connecting to Broker-------");
    Serial.print("Wait"); 
    delay(100); 
    Serial.print(".");

    if(mqtt.connect(id_mqtt)){
      Serial.println("Connected");
      Serial.print("--Broker: ");
      Serial.print(broker_mqtt);
      Serial.print(":");
      Serial.println(broker_port);
    }
  }
}

void isConected(){
  if(!mqtt.connected()){
    mqttReconect();
  }
  wifiConect();
}

void readTemperature(){
  unsigned long interval = 300000; //5 minutes
  unsigned long timerNow= millis();
  char sendBuffer[6];
  if((unsigned long)(timerNow-prevTimerTemp) >= interval){
    float temperature= dht.readTemperature();
    dtostrf(temperature,4,2,sendBuffer); 
    mqtt.publish("TEMPERATURE", sendBuffer);
    prevTimerTemp=millis();
  }
}


void readEcgSignal(){
  unsigned long interval = 20;
  unsigned long timerNow= millis();
  char sendBuffer[6];
  if((unsigned long)(timerNow-prevTimerEcg) >= interval){
    int ecg= analogRead(ecg_sensor); 
    dtostrf(ecg,5,0,sendBuffer); 
    mqtt.publish("ECG", sendBuffer); 
    prevTimerEcg=millis();
  }
}

void readBateryLevel(){
  char sendBuffer[6];
  float bat_now = analogRead(bat_volt);
  if(bat_now < bat_past){
    dtostrf(bat_now,5,0,sendBuffer); 
    mqtt.publish("BATTERY", sendBuffer); 
    bat_past = bat_now;
  }
}


void loop() {
  isConected(); 
  readTemperature(); 
  readEcgSignal(); 
  readBateryLevel(); 
  mqtt.loop(); //Keep Broker connection
}
