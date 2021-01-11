# ItalyMap
![](https://github.com/lucadentella/ItalyMap/raw/main/images/pcb.jpg)
## Description / *Descrizione*
ItalyMap is an electronic project to create a dynamic map of Italy. Thanks to the use of RGB LEDs, one for each Italian region, it is possible to give a visual representation of data relating to these regions.

*ItalyMap è un progetto elettronico per realizzare una mappa dell'Italia dinamica. Grazie all'uso di led RGB, uno per ogni regione italiana, è possibile dare una rappresentazione visuale di dati relativi a tali regioni.*
## Folders / *Cartelle*
 - **eagle**, project in Autodesk Eagle format / *progetto in formato Autodesk Eagle*
 - **gerber**, gerber files ready for manufacturing (JLCPCB service) / *files gerber pronti per la produzione (service JLCPCB)*
 - **sketch**, Arduino programs (esp8266 board) / *programmi Arduino (scheda esp8266)*
 	 - Test_Italian-Flag, displays italian flag / *visualizza la bandiera italiana*
	 - Covid_NewCases-perRegion, displays the new Covid-19 cases / *visualizza i nuovi casi di Covid-19*
	 - Covid_VaccinationRate-perRegion, displays the vaccination rate / *visualizza il numero di vaccinazioni*
## Partlist / *Elenco componenti*
 - [SK6812 MINI-E](https://www.aliexpress.com/item/4000475685852.html) addressable LED
 - 1x03 pinhead
 - Wemos D1 Mini (to run Arduino example)
### Test_Italian-Flag:
 ![](https://github.com/lucadentella/ItalyMap/raw/main/images/flag.jpg)
### Covid_NewCases-perRegion:
 ![](https://github.com/lucadentella/ItalyMap/raw/main/images/covid19.jpg)
### Covid_VaccinationRate-perRegion:
 ![](https://github.com/lucadentella/ItalyMap/raw/main/images/vaccine.jpg)
 