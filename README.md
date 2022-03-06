# WIFI LED Zimmerbeleuchtung

Ziel ist es mehrere LED-Stripes zu einem zusammen zu fassen. :)

## Idee

Die Idee ist es einen Controller (Master) zu haben, wo alle Berechnungen durchgeführt werden. Diese Information wird dann über Funk (z.B. WLAN) an die Stripes (Slaves) übermittelt, sodass diese in Echtzeit die Pixel anzeigen lassen können.

### Umsetzung 1

Die Slaves pingen den Master mehrere Male in der Sekunde an und zeigen dann alle Pixeln vom Stripe an ohne Überprüfung, ob sich Daten geändert haben.

Diese Methode ist nicht Leistungsoptimiert, da der Master potenziell nicht mit der Masse an Anfragen klar kommt. Viele Daten werden auch unnötigerweise versendet was die Geschwindigkeit zu Schaden kommt.

### Umsetzung 2

Diese Idee ist die gleiche Logik wie Umsetzung 1 verwendet aber anstelle eines des TCP Protokolls das UDP Protokoll. Diese Veränderung soll durch das Weglassen der Fehlererkennung eine schneller Verbindung herstellen. [(TCP vs. UDP)](https://nordvpn.com/de/blog/tcp-vs-udp/). Der TCP webserver ist immer noch als Ausfallsicherheit vorhanden, wird nur verwendet damit die Anzahl der LEDs Störungsfrei übertragen wird.

#### Resultat

Diese Idee hat vollständig funktioniert ich konnte keine Störungen feststellen alle Daten wurden auch übermittelt. Es dauert circa 50ms zwischen einzelnen Datenpacketen, dies entsüricht somit 20Hz ist damit ein sehr guter Wert.

## Release

### V0.1

Diese Version basiert auf der [Umsetzung 2](#Umsetzung-2), es kann rotes Licht mit 100 Pixeln per UDP über WLAN pulsierend auf den Slave übertragen werden.

### V0.2

In dieser Version wurde der gesamte Code in eine Bibliothek zusammengefasst. Somit benötigt man nur noch die Dateien `Connection.cpp`, `Connection.h`, `WIFIsettings.cpp` in den eigenen Ordner zu kopieren. Es ist möglich anstelle von `strip.show()` einfach `con.handleConnection()` zu nutzen.

**`Veränderung im Code:`**

```cpp
#include "Connection.h"
Connection con
void setup(){
    con.beginConnection(&strip);
}
void loop(){
    con.handleConnection();
}
```

*Die Nutzung von Adafruit_NeoPixel wird vorrausgesetzt*

#### Verbesserungen

Alle Daten werden an eine fest hinterlegte IP-Addresse ("192.168.4.1") geschickt, somit ist keinen Dynamik möglich. Außerdem ist auch keine Möglichkeit mehrere Strips anzusteuern. Es wird nicht überprüft, ob der Strip noch empfängt und bei Ausfall von dem werden weiterhin Daten verschickt. In manchen Fällen wird auch "numLED" für den Strip nicht korrekt übertragen, hierfür wäre eine stabile TCP Verbiundung von Vorteil.

## TO-DO

* Aktivitätsüberwachungs
  * prüfen ob Stripe noch vorhanden ist
* TCP Verbindung für numLED
* verbindug mit echten HEX-Werten kein String mehr (Reduzierung der Daten um Faktor 2)
* nur Veränderungen übertragen (mögliche Reduzierung der Datenmenge, jedoch mehr Rechenzeit benötigt)
* mehr Clients unterstützen (aktuell max.: 1)

## bekannte Bugs

* numLED nicht immer korrekt übergeben
* Kann sich nicht automatisch fangen bei Neustart eines Teilnehmers

