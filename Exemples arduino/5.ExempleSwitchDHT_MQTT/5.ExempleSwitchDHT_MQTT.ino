#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ABlocks_DHT.h>

// declarar variables
double humidity;
double ordenRelay;
double temperature;
int donut = 2;

// conectar con MQTT
const char mqtt_wifi_ssid[]="***";
const char mqtt_wifi_pass[]="***";
const char mqtt_broker[]="***";
const int mqtt_port=1883;
const char mqtt_user[]="";
const char mqtt_pass[]="";
const char mqtt_clientid[]="";
WiFiClient mqtt_wifiClient;
PubSubClient mqtt_client(mqtt_wifiClient);
char mqtt_payload[64];

// definir pin DHT
DHT dht0(D2,DHT22);

void mqtt_setup() {
	delay(10);;
	WiFi.begin(mqtt_wifi_ssid,mqtt_wifi_pass);
	while (WiFi.status() != WL_CONNECTED) delay(500);
	randomSeed(micros());
  mqtt_client.setServer(mqtt_broker, mqtt_port);
  mqtt_client.setCallback(mqtt_callback);
	mqtt_subscribe();
}

void mqtt_loop() {
	if (!mqtt_client.connected()) {
		mqtt_client.connect(mqtt_clientid,mqtt_user,mqtt_pass);
		mqtt_subscribe();
	}
	if (mqtt_client.connected()) {
  	mqtt_client.loop();
	}
}

double mqtt_payload2double(unsigned char *_payload, int _length) {
	int i;
	for (i = 0; i<_length && i<64; i++) {
		mqtt_payload[i] = _payload[i];
	}
	mqtt_payload[i] = 0;
	return atof(mqtt_payload);
}

String mqtt_payload2string(unsigned char *_payload, int _length) {
	int i;
	for (i = 0; i<_length && i<64; i++) {
		mqtt_payload[i] = _payload[i];
	}
	mqtt_payload[i] = 0;
	return String(mqtt_payload);
}

// voids de recepcion del estado del switch
void mqtt_callback(char* _topic, unsigned char* _payload, unsigned int _payloadlength) {
	double v=mqtt_payload2double(_payload,_payloadlength);
	String vt=mqtt_payload2string(_payload,_payloadlength);
	if(String(_topic)==String(String("Topic/recibir/orden/del/switch")))ordenRelay=v;
}
void mqtt_subscribe() {
	mqtt_client.subscribe(String(String("Topic/recibir/orden/del/switch")).c_str());
}

// funcion para leer los sensores (obvio)
void leerSensores() {
  humidity = dht0.readHumidity();
  temperature = dht0.readTemperature();
}

// funcion para publicar los valores (obvio)
void publicarValores() {
  mqtt_client.publish(String(String("Topic/enviar/humedad")).c_str(),String(humidity).c_str());
  delay(500);
  mqtt_client.publish(String(String("Topic/enviar/temperatura")).c_str(),String(temperature).c_str());
  delay(500);
}


void setup() {
  
  mqtt_setup();
  
  pinMode(D2, OUTPUT);
  pinMode(D2, INPUT);
  dht0.begin();
}


void loop() {
  
	yield();

	mqtt_loop();
  
  if (mqtt_client.connected()) {
      if ((ordenRelay == 1)) {
        mqtt_client.publish(String(String("Topic/mensaje/de/comprobacion")).c_str(),String(String("ON")).c_str()); // no es necesario, se puede omitir
        leerSensores();
        publicarValores();
        donut = 1;
        delay(50);
      }
      if ((ordenRelay == 0 && donut != 0)) {
        mqtt_client.publish(String(String("Topic/mensaje/de/comprobacion")).c_str(),String(String("OFF")).c_str()); // no es necesario, se puede omitir
        humidity = 0;
        temperature = 0;
        // leerSensores(); no lee los valores de los sensores, solo envia un 0
        publicarValores();
        donut = 0;
        delay(50);
      }
  }
}
