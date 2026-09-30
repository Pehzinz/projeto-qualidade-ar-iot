#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <DHTesp.h>
#include <ArduinoJson.h>

// WiFi — Wokwi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Broker Público (Sem SSL/TLS - Ideal para Wokwi)
const char* mqtt_server = "broker.hivemq.com";
const int   mqtt_port   = 1883;

// Tópicos para a Sala 202
const char* topic_pub = "escola/sala101/dados";
const char* topic_sub = "escola/sala101/atuador";

// Pinos
#define DHT_PIN     15
#define LED_R       25
#define LED_G       26
#define LED_B       27
#define BUZZER_PIN  14
#define MQ135_PIN   34

DHTesp dht;
WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;

void callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for (int i = 0; i < length; i++) msg += (char)payload[i];
  
  if (msg == "CRITICO") {
    digitalWrite(BUZZER_PIN, HIGH);
    analogWrite(LED_R, 255); analogWrite(LED_G, 0); analogWrite(LED_B, 0);
  } else if (msg == "ATENCAO") {
    digitalWrite(BUZZER_PIN, LOW);
    analogWrite(LED_R, 255); analogWrite(LED_G, 165); analogWrite(LED_B, 0);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    analogWrite(LED_R, 0); analogWrite(LED_G, 255); analogWrite(LED_B, 0);
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao Broker (Sala 101)... ");
    String clientId = "ESP32_Wokwi_101-";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("Conectado!");
      client.subscribe(topic_sub);
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" tentando em 2s...");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  dht.setup(DHT_PIN, DHTesp::DHT22);
  
  pinMode(LED_R, OUTPUT); 
  pinMode(LED_G, OUTPUT); 
  pinMode(LED_B, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 3000) {
    lastMsg = now;

    TempAndHumidity data = dht.getTempAndHumidity();
    int mq135Raw = analogRead(MQ135_PIN);
    int co2ppm = map(mq135Raw, 0, 4095, 400, 3000);

    StaticJsonDocument<200> doc;
    doc["sala"]        = "Sala_101";
    doc["temperatura"] = isnan(data.temperature) ? 0 : data.temperature;
    doc["umidade"]     = isnan(data.humidity) ? 0 : data.humidity;
    doc["co2"]         = co2ppm;

    char buffer[256];
    serializeJson(doc, buffer);
    client.publish(topic_pub, buffer);
    Serial.println("Dados Sala 101 publicados!");
  }
}