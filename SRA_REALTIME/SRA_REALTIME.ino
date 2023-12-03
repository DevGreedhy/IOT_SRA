#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

// Proporcionar información sobre el proceso de generación de tokens.
#include "addons/TokenHelper.h"
// Proporcionar información de impresión de carga útil de RTDB y otras funciones de ayuda.
#include "addons/RTDBHelper.h"

// Inserta tus credenciales de red
#define WIFI_SSID "TINI"
#define WIFI_PASSWORD "motabebe"

// Inserta la clave API de tu proyecto Firebase
#define API_KEY "AIzaSyB28mtCRswFWgbuQqr0LgVx1PwCNb2KRQ4"

// Inserta el correo electrónico autorizado y la contraseña correspondiente
#define USER_EMAIL "pruebaesp@hotmail.com"
#define USER_PASSWORD "prueba123"

// Inserta la URL de la RTDB
#de   fine DATABASE_URL "https://esp-sra-default-rtdb.firebaseio.com/"

// Define objetos de Firebase
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Variable para guardar el UID del usuario
String uid;

// Variables para guardar rutas de la base de datos
String databasePath;
String sensorPath;
String bombaPath;

const int sensorPin = 36;
const int bombaPin = 14;
int valorSensor; // Agregada declaración de valorSensor
unsigned long tiempoAnterior = 0; // Agregada declaración de tiempoAnterior
const unsigned long tiempoEspera = 5000; // Agregada declaración de tiempoEspera

// Variables temporizadoras (enviar nuevas lecturas cada tres minutos)
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 5000;

// Inicializar WiFi
void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando a WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}

// Escribir valores enteros en la base de datos
void sendInt(String path, int value){
  if (Firebase.RTDB.setInt(&fbdo, path.c_str(), value)){
    Serial.print("Escribiendo valor: ");
    Serial.print(value);
    Serial.print(" en la siguiente ruta: ");
    Serial.println(path);
    Serial.println("ÉXITO");
    Serial.println("RUTA: " + fbdo.dataPath());
    Serial.println("TIPO: " + fbdo.dataType());
  }
  else {
    Serial.println("FALLÓ");
    Serial.println("RAZÓN: " + fbdo.errorReason());
  }
}

void readData(String path) {
  int sensorValue = 0;
  if (Firebase.RTDB.getInt(&fbdo, path.c_str())) {
    Serial.println("RUTA: " + fbdo.dataPath());
    Serial.println("TIPO: " + fbdo.dataType());
    if (fbdo.dataType() == "int") {
      sensorValue = fbdo.intData();
      Serial.print("VALOR DEL SENSOR desde Firebase: ");
      Serial.println(sensorValue);
    }
  } else {
    Serial.println("FALLÓ");
    Serial.println("RAZÓN: " + fbdo.errorReason());
  }
}

void setup() {
  Serial.begin(115200);

  // Inicializar WiFi
  initWiFi();
  
  // Asignar la clave API (obligatorio)
  config.api_key = API_KEY;

  // Asignar las credenciales de inicio de sesión del usuario
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Asignar la URL de la RTDB (obligatorio)
  config.database_url = DATABASE_URL;

  // Esperar la conexión a WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }

  // Asignar la clave API (obligatorio)
  config.api_key = API_KEY;

  // Asignar las credenciales de inicio de sesión del usuario
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Asignar la URL de la RTDB (obligatorio)
  config.database_url = DATABASE_URL;

  // Inicializar la biblioteca con autenticación y configuración de Firebase
  Firebase.begin(&config, &auth);

  // Esperar a que Firebase esté listo
  while (!Firebase.ready()) {
    delay(1000);
    Serial.println("Esperando a que Firebase esté listo...");
  }

  // Obtener el UID del usuario puede llevar unos segundos
  Serial.println("Obteniendo el UID del usuario");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }

  // Imprimir el UID del usuario
  uid = auth.token.uid.c_str();
  Serial.print("UID del usuario: ");
  Serial.println(uid);

  // Actualizar la ruta de la base de datos
  databasePath = "/UsersData/" + uid;

  // Actualizar la ruta de la base de datos para las lecturas del sensor
  sensorPath = databasePath + "/sensor"; // --> UsersData/<user_uid>/sensor
  bombaPath = databasePath + "/bomba"; // --> UsersData/<user_uid>/bomba
  pinMode(bombaPin, OUTPUT);
}

void loop() {
  valorSensor = analogRead(sensorPin);

  // Imprimir el valor del sensor en el puerto serie
  Serial.print("Valor del sensor: ");
  Serial.println(valorSensor);

  // Condiciones de Encendido y Apagado de la Bomba
  if (valorSensor > 3000) {
    digitalWrite(bombaPin, HIGH);
    // Imprimir mensaje indicando que la bomba está encendida
    Serial.println("Bomba encendida");
    sendInt(bombaPath, HIGH);  // Enviar el estado a la base de datos
  } else {
    digitalWrite(bombaPin, LOW);
    // Imprimir mensaje indicando que la bomba está apagada
    Serial.println("Bomba apagada");
    sendInt(bombaPath, LOW);  // Enviar el estado a la base de datos
  }

  // Esperar antes de volver a leer el sensor
  if (millis() - tiempoAnterior >= tiempoEspera) {
    tiempoAnterior = millis();

    // Enviar nuevas lecturas a la base de datos
    if (Firebase.ready() && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0)) {
      sendDataPrevMillis = millis();

      // Obtener las últimas lecturas del sensor
      int sensorValue = analogRead(sensorPin);

      // Enviar lecturas a la base de datos:
      sendInt(sensorPath, sensorValue);
      readData(sensorPath);
    }
  }
}