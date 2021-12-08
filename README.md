# Sklep Internetowy na przedmiot Biznes Elektroniczy

Projekt imituje sprzedaż biletów do kina Helios, a dokładniej oddziału "Gdańsk-Metropolia"

## Moduły:
- PrestaShop - sklep
- Scraper - program w C++ pobierający dane ze strony kina [Helios](https://www.helios.pl/49,Gdansk/Repertuar/index/dzien/0/kino/49)
i przekształcający je na format CSV zrozumiały dla PrestaShop
- Selenium - test złożenia zamówienia i założenia konta w Pythonie

## Uruchomienie:
Sklep do działania potrzebuje bazy danych dołączonej do repozytorium jako plik
["Database Backup.sql"](https://github.com/Kacperos155/Biznes-Elektroniczny/blob/master/Database%20Backup.sql)  
  
Ustawienia serwera bazy danych znajdują się w pliku
["PrestaShop/app/config/parameters.php"](https://github.com/Kacperos155/Biznes-Elektroniczny/blob/master/PrestaShop/app/config/parameters.php) 