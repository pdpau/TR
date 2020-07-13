#include <ESP8266WiFi.h>

#include <PubSubClient.h>

#include <ABlocks_DHT.h>

double Humedad;
double Temperatura;

const char mqtt_wifi_ssid[]="ssid";
const char mqtt_wifi_pass[]="password";
const char mqtt_broker[]="your_broker";
const int mqtt_port=1883;
const char mqtt_user[]="";
const char mqtt_pass[]="";
const char mqtt_clientid[]="";
WiFiClient mqtt_wifiClient;
PubSubClient mqtt_client(mqtt_wifiClient);
char mqtt_payload[64];
DHT dht0(0,DHT22);

void mqtt_setup(){
	delay(10);;
	WiFi.begin(mqtt_wifi_ssid,mqtt_wifi_pass);
	while (WiFi.status() != WL_CONNECTED) delay(500);
	randomSeed(micros());
  mqtt_client.setServer(mqtt_broker, mqtt_port);
  mqtt_client.setCallback(mqtt_callback);
	mqtt_subscribe();
}

void mqtt_loop(){
	if (!mqtt_client.connected()) {
		mqtt_client.connect(mqtt_clientid,mqtt_user,mqtt_pass);
		mqtt_subscribe();
	}
	if (mqtt_client.connected()) {
  	mqtt_client.loop();
	}
}

double mqtt_payload2double(unsigned char *_payload, int _length){
	int i;
	for (i = 0; i<_length && i<64; i++){
		mqtt_payload[i] = _payload[i];
	}
	mqtt_payload[i] = 0;
	return atof(mqtt_payload);
}

String mqtt_payload2string(unsigned char *_payload, int _length){
	int i;
	for (i = 0; i<_length && i<64; i++){
		mqtt_payload[i] = _payload[i];
	}
	mqtt_payload[i] = 0;
	return String(mqtt_payload);
}
void mqtt_callback(char* _topic, unsigned char* _payload, unsigned int _payloadlength){
	double v=mqtt_payload2double(_payload,_payloadlength);
	String vt=mqtt_payload2string(_payload,_payloadlength);
}

void mqtt_subscribe(){
}

void leer_sensores() {
  Humedad = dht0.readHumidity();
  Temperatura = dht0.readTemperature();
}

void subir_datos_a_la_nube() {
  mqtt_client.publish(String(String("topic/to/subscribe")).c_str(),String(Temperatura).c_str());
  delay(1000);
  mqtt_client.publish(String(String("topic/to/subscribe")).c_str(),String(Humedad).c_str());
  delay(1000);
}

void detener_medici_C3_B3n_nn_minutos() {
  delay(5000);
}

void setup() {
  	mqtt_setup();
pinMode(0, INPUT);
dht0.begin();
}


void loop() {
	yield();

	mqtt_loop();
    leer_sensores();
    subir_datos_a_la_nube();
    detener_medici_C3_B3n_nn_minutos();

}
