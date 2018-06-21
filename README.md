# ESP8266-Balkonstation

## Vorhaben & Motivation

Die Balkonstation soll zwei Hauptzwecke erfüllen. Einerseits soll eine Wetterstation entstehen. Dieser Teil des Projektes ist eher Spielerei und vielleicht binde ich die Daten irgendwann in ein Open-Data-Projekt ein. Die zweite und mir wichtigere Aufgabe der ESP8266-Balkonstation ist die Wasserversorgung meiner Chilli-Pflanze. Da im Sommer bei unkonstantem Wetter immer wieder mal die Erde der kleinen Töpfe vollkommen austrocknen, soll nun automatisch gegossen werden. Ich hätte gerne eine Sicherstellung der Versorgung mit Solarenergie, damit für die grundlegende Funktionsweise ein autarkes System geschaffen wird.

## Benötigte Bauteile

- ESP8266 Mikrocontroller [https://amzn.to/2JWbwMH]
- DS18B20 Temperatursensor [https://amzn.to/2tnks2B]
- SainSmart 5V 2 Kanal-Relais (ein Kanal würde auch reichen, dieses lag noch rum) [https://amzn.to/2I8AD9o]
- YL-38/YL-69 Bodenfeuchtesensor [https://amzn.to/2K59Ood]
- 5V / 2,3 W Wasserpumpe [https://amzn.to/2I8rqOp]
- 2,5m Wasserschlauch [https://amzn.to/2tftFe6]
- Breadboard & Jumper-Kabel [https://amzn.to/2JT3t3f]

## Hardwareaufbau



## Benötigte Programme

- ArduinoIDE [https://www.arduino.cc/en/Main/Software]
- Bibliotheken für den ESP8266 [https://arduino-hannover.de/2015/04/08/arduino-ide-mit-dem-esp8266/]

## Beschreibung der Software

Das Programm startet, wählt sich in das hinterlegte WLAN ein und führt die Messung der Sensorsysteme durch. Anschließend werden die Messdaten zur Visualisierung an den Dienst Thingspeak geschickt. Wenn danach im Fall des Wasserbedarfs die Pumpe angesteuert und wieder abgestellt wurde, wird der ESP8266 für eine Stunde in den Deep-Sleep-Modus versetzt.
