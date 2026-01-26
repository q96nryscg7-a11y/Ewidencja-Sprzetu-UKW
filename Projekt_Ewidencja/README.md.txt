# Projekt: Ewidencja Sprzętu (Temat 12)
Autor: Mykyta Miakyshev
Uczelnia: UKW Bydgoszcz

## Krótki opis
Program służy do zarządzania wypożyczalnią sprzętu. Pozwala na dodawanie przedmiotów, 
wypożyczanie ich konkretnym osobom, zwracanie oraz generowanie raportów TOP N 
najczęściej używanych pozycji.

## Kompilacja
gcc -Wall -Wextra main.c -o program

## Instrukcja
1. Po uruchomieniu załaduje się baza z pliku .csv (jeśli istnieje).
2. Menu pozwala wybrać operację (1-5).
3. Przy dodawaniu należy podać unikalne ID (program sprawdza duplikaty).
4. Przy wypożyczaniu podajemy ID sprzętu i ID klienta.
5. Przy wyjściu (0) program automatycznie zapisuje zmiany.

## Format pliku baza.csv
1 linia = 1 rekord. Pola oddzielone średnikiem (;).
Kolejność: id;nazwa;status;id_pozyczajacego;licznik_wypozyczen