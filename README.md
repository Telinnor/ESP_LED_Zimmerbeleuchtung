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

