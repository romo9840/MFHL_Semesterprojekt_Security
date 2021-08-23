# MFHL_Semesterprojekt_Security


Das hierzusehende Projekt dient dazu eine sehr einfache Sicherheitsanlage zu erschaffen. 

Collaborators: Marvin Timmerman, Felix Büntemeier, Hendrik Nessen & Leif Meyer

Hardware:
  * ESP8266 Lolin Wemos D1 Mini 
  * ~~2 Buttons~~ ESP8266 Button Shield
  * Zwei Leds 5V (Rot und Grün)
  * ~~2 Resistors 200 Ohm (Subject to change)~~
  * Ein PIR Bewegungssensor
  * Ein Netzteil
  
  ~~Möglicherweise mit Battery:~~
   * ~~1 D1 Mini Battery Shield~~
   * ~~1 LiPo Battery~~

Software:
 * Arduino 
 * Node-RED
  
Das Projekt muss über einen "Home" sowie "Away" Zustand verfügen.
Den Zugang zu diesen Zuständen erreicht man durch das Betätigen des integrierten Buttons.

Das Projekt muss im Away Zustand Bewegung für 5-10 Sekunden erkennen und dann ein Timer auslösen für 1 Minute. 
  * Falls der Timer abläuft wird eine Mail oder Discord Benachrichtigung verschickt.
  * Falls der Home Button gedrückt wird, passiert nichts weiteres.
  * Soll Rote LED leuchten lassen.

Das Projekt muss im Home Zustand einfach nur bereit sein in dem Away Zustand zu wechseln falls Button betätigt wird.
 * Soll Grüne LED leuchen lassen. 
  
