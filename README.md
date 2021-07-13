# MFHL_Semesterprojekt_Security


Das hierzusehende Projekt dient dazu eine sehr einfache Sicherheitsanlage zu erschaffen. 

Collaborators: Marvin Timmerman, Felix Büntemeier, Hendrick Nessen & Leif Meyer

Komponente:
  * ESP8266 Lolin Wemos D1 Mini 
  * 2 Buttons
  * 2 Leds (Optional) 
  * 1 Bewegungssensor
  * 1 Netzteil
  
  Möglicherweise mit Battery:
   * 1 D1 Mini Battery Shield
   * 1 LiPo Battery
  
Das Projekt muss ein Home Zustand und ein Away Zustand haben. 
  In diese Zustände kommt man indem man den Home Button oder Away Button betätigt.

Das Projekt muss im Away Zustand Bewegung für 5-10 Sekunden erkennen und dann ein Timer auslösen für 1 Minute. 
  * Falls der Timer abläuft wird eine Mail oder Discord Benachrichtigung verschickt.
  * Falls der Home Button gedrückt wird, passiert nichts weiteres.
  * Soll Rote LED leuchten lassen.

Das Projekt muss im Home Zustand einfach nur bereit sein in dem Away Zustand zu wechseln falls Button betätigt wird.
 * Soll Grüne LED leuchen lassen. 
  
