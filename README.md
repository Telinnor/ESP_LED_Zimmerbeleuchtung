# WIFI LED Zimmerbeleuchtung

Ziel ist es mehrere LED-Stripes zu einem zusammen zu fassen. :)

## Idee

Die Idee ist es einen Controller (Master) zu haben, wo alle Berechnungen durchgeführt werden. Diese Information wird dann über Funk (z.B. WLAN) an die Stripes (Slaves) übermittelt, sodass diese in Echtzeit die Pixel anzeigen lassen können.

### Umsetzung 1

Die Slaves pingen den Master mehrere Male in der Sekunde an und zeigen den aktuellen Status an ohne Überprüfung, ob sich Daten geändert haben.

Diese Methode ist nicht Leistungsoptimiert, da der Master potenziell nicht mit der Masse an Anfragen klar kommt. Viele Daten werden auch unnötigerweise versendet was die Geschwindigkeit zu Schaden kommt.

