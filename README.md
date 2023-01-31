# ISA-Project
Čtečka novinek ve formátu Atom a RSS s podporou TLS

Napište program feedreader, který bude vypisovat informace uvedené ve stažených zdrojích (feed) ve formátu Atom a RSS 2.0. Pro RSS se řiďte dokumentací odkazovanou níže. Program po spuštění stáhne zadané zdroje a na standardní výstup vypíše informace požadované uživatelem (např. názvy článků).

Při vytváření programu je povoleno použít hlavičkové soubory pro práci se sokety a další obvyklé funkce používané v síťovém prostředí (jako je netinet/*, sys/*, arpa/* apod.), knihovnu pro práci s vlákny (pthread), signály, časem, stejně jako standardní knihovnu jazyka C (varianty ISO/ANSI i POSIX), C++ a STL. Pro práci s daty ve formátu XML je doporučená knihovna libxml2, případně jiné knihovny dostupné na serveru merlin a eva. Pro práci s TLS je doporučená knihovna openssl, případně jiné knihovny dostupné na serveru merlin a eva. Pro práci s XML a TLS můžete použít i jiné knihovny, musí však být jasně označeny, že jde o převzatý kód a musí být součástí odevzdávaného archivu. Jiné knihovny nejsou povoleny.

(12.5 bodu z 20)
