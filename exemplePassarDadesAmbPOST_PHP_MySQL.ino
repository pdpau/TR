//si usas esp32
//#include <HTTPClient.h>
//#include <WiFi.h>

//si usas esp8266
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <DHT.h>
#define DHTPIN 0
#define DHTTYPE DHT22

DHT dht0(DHTPIN, DHTTYPE);

const char* ssid = "your_wifi_ssid";
const char* password =  "your_password";


void setup() {
  delay(10);
  Serial.begin(115200);
  dht0.begin();

  WiFi.begin(ssid, password);

  // comprobamos conexión
  Serial.print("Conectando...");
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }

  Serial.print("Conectado con éxito, mi IP es: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  
  double temperatura = dht0.readTemperature();
  double humedad = dht0.readHumidity();

  while (isnan(humedad) || isnan(temperatura)) {
    Serial.println("Fallo al recibir los datos del DHT22");
    delay(2000);
    return;
  }

  // comprobamos que los datos son correctos
  Serial.println(temperatura);
  Serial.println(humedad);

  String temp = "temp=";
  String hum = "&hum=";
  
  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;

    String datos_a_enviar = temp + temperatura + hum + humedad;

    //Indicamos el destino
    http.begin("http://192.168.0.11/dataToDB.php"); //Archivo PHP con el que se mandarán los datos a MySQL
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    //Enviamos el post pasándole, los datos que queremos enviar. (esta función nos devuelve un código que guardamos en un int)
    int codigo_respuesta = http.POST(datos_a_enviar);

    if (codigo_respuesta > 0) {
      Serial.println("Código HTTP ► " + String(codigo_respuesta));
      
      if (codigo_respuesta == 200) {
        String cuerpo_respuesta = http.getString();
        Serial.println("El servidor respondió ▼ ");
        Serial.println(cuerpo_respuesta);

      }

    } else {

      Serial.print("Error enviando POST, código: ");
      Serial.println(codigo_respuesta);

    }

    http.end();  //libero recursos

  } else {

    Serial.println("Error en la conexión WIFI");

  }

  delay(8000);
}
