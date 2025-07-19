#include <WiFi.h>
#include <WebServer.h>

// DATOS DEL WIFI
const char* ssid = "RedWIFI";
const char* password = "contraseña";

const int gasPin = 34;      // Pin del sensor de monoxido
const int buzzerPin = 35;   // Pin del buzzer
const int umbralCO = 350;   // Ajusta este valor

bool ambienteSeguro = true;

WebServer server(80);

void handleRoot() {
  // "PAGINA WEB"
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta http-equiv='refresh' content='2'>"; 
  html += "<meta charset='UTF-8'>";
  html += "<title>Monitor de Monóxido de carbono</title></head>";
  html += "<body><h1 style='font-size: 48px; text-align: center; color: ";
  html += ambienteSeguro ? "green;'>✅ AMBIENTE SEGURO" : "red;'>⚠️ MONÓXIDO DE CARBONO DETECTADO";
  html += "</h1></body></html>";

  server.send(200, "text/html", html);
}

void setup() {
  pinMode(buzzerPin, OUTPUT);

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  
  server.on("/", handleRoot);
  server.begin();
  Serial.println("Servidor iniciado. IP: " + WiFi.localIP().toString());
}

void loop() {
  int gasValue = analogRead(gasPin);
  // Leer valor del sensor
  Serial.print("Nivel de CO: ");
  Serial.println(gasValue);
  if (gasValue > umbralCO) {
    digitalWrite(buzzerPin, HIGH); // Activa buzzer
    ambienteSeguro = false;
    Serial.println("¡CO detectado!");  
  }
  else{
    digitalWrite(buzzerPin, LOW);   // Apaga buzzer
    ambienteSeguro = true;
  }

  server.handleClient();
  delay(100);
}