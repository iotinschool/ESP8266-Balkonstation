#include <ESP8266WiFi.h>                                                  // Bibliothek zum ESP8266 einbinden
#include <OneWire.h>                                                      // Bibliothek für 1-Wire-Bus einbinden
#include <DallasTemperature.h>                                            // Bibliothek für den DS18B20 einbinden

#define relais 13                                                         // (D0) Pin für das Relais zum Schalten der Pumpe

#define DS18B20 5                                                         // (D1) Pin für den DS18B20 zum Messen der Temperatur
float temp = 0;                                                           // Initialisierung der Variablen zum Temperaturwert                                               

#define LED 0                                                             // (D2) Pin für die LED rot zur Anzeige, dass der Tank leer ist
#define LED 4                                                             // (D3) Pin für die LED gelb zur Anzeige, dass der Tank halb voll ist

#define us_trigger 14                                                     // (D5) Pin für den Ultraschallsensor, Auslöser
#define us_echo 12                                                        // (D6) Pin für den Ultraschallsensor, Messwert
long dauer = 0;                                                           // Dauer für die Schallwelle des Ultraschallsensors
long entfernung = 0;                                                      // Entfernung für die Schallwelle des Ultraschallsensors
#define USONIC_DIV 58.0

byte humidity_sensor_pin = A0;
byte humidity_sensor_vcc = 6;
int feuchtigkeit = 0;

OneWire oneWire(DS18B20);                                                 // 1-Wire-Instanz erstellen
DallasTemperature sensors(&oneWire);                                      // Dallas-Temperatursensor aktivieren


const char* ssid = "Pluto";                                               // SSID des WLANS
const char* password = "04038644486705549308";                            // Passwort des WLANS

WiFiClient client;                                                        // Erstellung einer WLAN-Client-Instanz

String apiKey = "UAFX33RMZN1BYNXN";                                       // Enter your Write API key from ThingSpeak
const char* server = "api.thingspeak.com";

void setup_wifi() {
  delay(10);                                                              // We start by connecting to a WiFi network
  Serial.println();                                                       // Serial Monitor Zeilenumbruch
  Serial.print("Connecting to ");                                         // Serial Monitor Ausgabe
  Serial.println(ssid);                                                   // Serial Monitor Ausgabe der SSID
  WiFi.mode(WIFI_STA);                                                    // WLAN-Modus einstellen
  WiFi.begin(ssid, password);                                             // WLAN-Verbindung mit SSID und Passwort herstellen

  while (WiFi.status() != WL_CONNECTED) {                                 // Falls keine WLAN-Verbindung besteht, Punkte ausgeben
    delay(500);                                                           // Verzögerungszeit zwischen zwei Punkten
    Serial.print(".");                                                    // Serial Monitor Ausgabe der Punkte
  }
  
  Serial.println("");                                                     // Serial Monitor Zeilenumbruch
  Serial.println("WiFi connected");                                       // Serial Monitor Ausgabe WLAN hergestellt
  Serial.println("IP address: ");                                         // Serial Monitor Ausgabe IP-Adresse
  Serial.println(WiFi.localIP());                                         // Serial Monitor Ausgabe der erhaltenen IP-Adresse
}

int read_humidity_sensor() {                                              // Funktion zum Auslesen des Erdfeuche-Sensors
  digitalWrite(humidity_sensor_vcc, HIGH);                                // Spannungsversorgung des Erdfeuche-Sensors einschalten
  delay(500);                                                             // eine halbe Sekunde warten
  int value = analogRead(humidity_sensor_pin);                            // Übergabe des Wertes vom Analogsensor
  digitalWrite(humidity_sensor_vcc, LOW);                                 // Spannungsversorgung des Erdfeuche-Sensors ausschalten
  return 1023 - value;                                                    // Wert zurückgeben
}

long singleMeasurement() {
  long duration = 0;
  digitalWrite(us_trigger, HIGH);
  delay(5);
  digitalWrite(us_trigger, LOW);
  dauer = pulseIn(us_echo, HIGH); 
  return (long) (((float) dauer / USONIC_DIV) * 10.0);
}

void setup() {                                                            // Setup-Methode
  Serial.begin(115200);                                                   // Start des Serial Monitor mit 115200 Baud
  setup_wifi();                                                           // Start des WLAN
  pinMode(DS18B20, INPUT);                                                // Pin des DS18B20 als Input einrichten
  pinMode(LED, OUTPUT);                                                   // Pin der LED als Output einrichten
  sensors.begin();                                                        // Start der Sensoren
  pinMode(us_trigger, OUTPUT);                                            // Trigger-Pin ist ein Ausgang
  pinMode(us_echo, INPUT);                                                // Echo-Pin ist ein Eingang
  pinMode(relais, OUTPUT);

  sensors.setResolution(12);                                              // Sensor-Auflösung einstellen
  sensors.requestTemperatures();                                          // Temperatur ermitteln
  temp = sensors.getTempCByIndex(0);                                      // Temperaturwert übergeben

  entfernung = singleMeasurement();
  entfernung = 266.5-entfernung;
  
  feuchtigkeit = read_humidity_sensor();
 
  if (client.connect(server,80)) {                                        // Wenn Verbindung mit api.thingspeak.com besteht
    String postStr = apiKey;                                              // Einfügen des API-Key
    postStr +="&field1=";                                                 // Feld 1 definieren
    postStr += String(temp);                                              // Wert für die Temperatur übergeben
    postStr +="&field2=";                                                 // Feld 2 definieren
    postStr += String(feuchtigkeit);                                      // Wert für die Erdfeuchte übergeben
    postStr +="&field3=";                                                 // Feld 2 definieren
    postStr += String(entfernung);                                        // Wert für den Füllstand übergeben
    postStr += "\r\n\r\n";                                                // Abschließen
    
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    
    Serial.print("Temperatur: ");
    Serial.print(temp);
    Serial.print(" C");
    Serial.println();
    Serial.print("Erdfeuchte: ");
    Serial.print(feuchtigkeit);
    Serial.println();
    Serial.print("Füllstand: ");
    Serial.print(entfernung);
    Serial.print(" mm");
    Serial.println();
  }
  client.stop();

  if ((feuchtigkeit < 400) && (entfernung > 20)) {
    digitalWrite(relais, HIGH); //An dieser Stelle würde das Relais einsschalten
    Serial.print("Auffüllung ");
    for (int i=0; i <= 10; i++){
      Serial.print(".");
      delay(1000);
    }
    Serial.println();
    digitalWrite(relais, LOW); //Und wieder ausschalten
    delay(5000); //...und eine Sekunde warten.
  } else {
    Serial.println("Keine Auffüllung");
    delay(15000);
  }
  
  ESP.deepSleep(3600000000); // 20e6 is 20 microseconds
}

void loop() {                                                             // Loop-Methode
}





