# Síťové aplikace a správa sítí – ISA
## Čtečka novinek ve formátu Atom s podporou TLS
#### Autor: Marina Kravchuk (xkravc02) 14.11.2022
---
## Zadání
 Napište program feedreader, který bude vypisovat informace uvedené ve stažených zdrojích (feed) ve formátu Atom a RSS. Program po spuštění stáhne zadané zdroje a na standardní výstup vypíše informace požadované uživatelem (např. názvy článků).

Implementovano v jazyce C

---

## Použití programu:
feedreader <URL | -f <feedfile>> [-c <certfile>] [-C <certaddr>] [-T] [-a] [-u] [-h]

##### Popis jednotlivých parametrů:

URL				Adresa URL, jejiž feedy se mají načíst a zpracovat. Musí začínat http:// či https://. Za hostitelem je možné zadat explicitní port uvozený dvojtečkou.
- f feedfile		Cesta k souboru feedfile obsahujícího více adres URL s feedy, které se mají načíst a zpracovat.
- c certfile		Cesta k souboru s certifikáty pro ověření důvěryhodnosti.
- C certaddr		Cesta k adresáři s certifikáty pro ověření důvěryhodnosti.
- T				Zobrazit časové údaje u položek feedu.
- a				Zobrazit informace o autorovi u položek feedu.
- u				Zobrazit asociovanou adresu u položky feedu.
- h				Zobrazit nápovědu k programu.
---

## Příklad spuštění:

./feedreader 'https://what-if.xkcd.com/feed.atom' -u -T

## Seznam odevzdaných souborů:
- `Makefile`
- `feefreader.c`
- `args_parser.c` a `args_parser.h`
- `feed_parser.c` a `feed_parser.h`
- `url_parser.c` a `url_parser.h`
- `xml_parser.c` a `xml_parser.h`
- `downloader.c` a `downloader.h`
- `README.md`
- `manual.pdf`
