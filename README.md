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
  
Das Sicherheitssystem muss über einen "Home" sowie "Away" Zustand verfügen.
Den Zugang zu diesen Zuständen erreicht man durch das Betätigen des integrierten Buttons.

Nach einmaligem Betätigen des Buttons wird der Zustand "Away" erreicht.
  * Die grüne LED erlischt und die rote LED permanent auf.
  * Der Timer beginnt und stoppt bei einer Dauer von 2 Minuten.
  * Nachdem der Timer abgelaufen ist, erfasst der Sensor jede Bewegung.
  * Nachdem eine Bewegung erkannt wurde, wird umgehend eine Benachrichtung an Discord sowie Email versandt.
  * Innerhalb der Zustände kann ein Wechsel ausnahmslos durch Betätigen des Buttons initiiert werden.

Das Sicherheitssystem wird im "Home" Zustand lediglich in Bereitsschaft versetzt. Hier wird nur das auf Betätigen des Buttons reagiert.
 * Die grüne LED leuchtet permanent auf.
  
