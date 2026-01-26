#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Glowna struktura 
struct Sprzet {
    int item_id;
    char name[50];
    int status; // 0 - dostepny, 1 - wypozyczony
    int borrower_id;
    int rental_count;
};

// Stan programu 
struct Stan {
    struct Sprzet *lista;
    int ile;
    int pojemnosc;
};

// Funkcja na wskazniku do stanu
void wypozycz(struct Stan *s, int id_sprzetu, int id_klienta) {
    for (int i = 0; i < s->ile; i++) {
        if (s->lista[i].item_id == id_sprzetu) {
            if (s->lista[i].status == 1) {
                printf("Blad: Sprzet o ID %d jest juz wypozyczony!\n", id_sprzetu);
                return;
            }
            s->lista[i].status = 1;
            s->lista[i].borrower_id = id_klienta;
            s->lista[i].rental_count++;
            printf("Udalo sie wypozyczyc %s.\n", s->lista[i].name);
            return;
        }
    }
    printf("Nie ma sprzetu o takim ID.\n");
}

void zwroc(struct Stan *s, int id_sprzetu) {
    for (int i = 0; i < s->ile; i++) {
        if (s->lista[i].item_id == id_sprzetu) {
            if (s->lista[i].status == 0) {
                printf("Blad: Ten sprzet nie jest wypozyczony!\n");
                return;
            }
            s->lista[i].status = 0;
            s->lista[i].borrower_id = -1;
            printf("Sprzet %s wrocil do bazy.\n", s->lista[i].name);
            return;
        }
    }
    printf("Nie znaleziono takiego ID.\n");
}

// Sprawdzanie czy ID juz istnieje 
int czy_id_istnieje(struct Stan *s, int nowe_id) {
    for (int i = 0; i < s->ile; i++) {
        if (s->lista[i].item_id == nowe_id) return 1;
    }
    return 0;
}

int main() {
    struct Stan stan;
    stan.ile = 0;
    stan.pojemnosc = 4;
    // Pamiec dynamiczna
    stan.lista = malloc(stan.pojemnosc * sizeof *stan.lista);
    if (!stan.lista) {
        fprintf(stderr, "Brak pamieci przy alloc!\n");
        return 1;
    }

    // Wczytywanie z CSV
    FILE *f = fopen("baza.csv", "r");
    if (f) {
        // Format: id;nazwa;status;kto;licznik
        while (fscanf(f, "%d;%49[^;];%d;%d;%d\n", &stan.lista[stan.ile].item_id, 
               stan.lista[stan.ile].name, &stan.lista[stan.ile].status, 
               &stan.lista[stan.ile].borrower_id, &stan.lista[stan.ile].rental_count) == 5) {
            stan.ile++;
            if (stan.ile >= stan.pojemnosc) {
                stan.pojemnosc *= 2;
                struct Sprzet *tmp = realloc(stan.lista, stan.pojemnosc * sizeof *stan.lista);
                if (!tmp) {
                    fprintf(stderr, "Brak pamieci przy realloc!\n");
                    fclose(f);
                    free(stan.lista);
                    return 1;
                }
                stan.lista = tmp;
            }
        }
        fclose(f);
    }

    int menu;
    while (1) {
        printf("\n--- EWIDENCJA SPRZETU (UKW) ---\n");
        printf("1. Dodaj 2. Wypozycz 3. Zwroc 4. Listy 5. Raporty 0. Wyjdz\nWybor: ");
        if (scanf("%d", &menu) != 1) break;

        if (menu == 0) break;

        if (menu == 1) {
            int nid;
            printf("Podaj nowe ID: "); if (scanf("%d", &nid) != 1) { while (getchar()!='\n'); continue; }
            if (czy_id_istnieje(&stan, nid)) {
                printf("Blad: ID %d juz jest w systemie!\n", nid); 
            } else {
                if (stan.ile >= stan.pojemnosc) {
                    stan.pojemnosc *= 2;
                    struct Sprzet *tmp = realloc(stan.lista, stan.pojemnosc * sizeof *stan.lista);
                    if (!tmp) {
                        fprintf(stderr, "Brak pamieci przy realloc!\n");
                        free(stan.lista);
                        return 1;
                    }
                    stan.lista = tmp;
                }
                stan.lista[stan.ile].item_id = nid;
                printf("Nazwa (bez spacji): "); scanf("%49s", stan.lista[stan.ile].name);
                stan.lista[stan.ile].status = 0;
                stan.lista[stan.ile].borrower_id = -1;
                stan.lista[stan.ile].rental_count = 0;
                stan.ile++;
            }
        } 
        else if (menu == 2) {
            int sid, kid;
            printf("ID sprzetu i ID klienta: "); if (scanf("%d %d", &sid, &kid) != 2) { while (getchar()!='\n'); continue; }
            wypozycz(&stan, sid, kid);
        }
        else if (menu == 3) {
            int sid;
            printf("ID do zwrotu: "); if (scanf("%d", &sid) != 1) { while (getchar()!='\n'); continue; }
            zwroc(&stan, sid);
        }
        else if (menu == 4) {
            printf("1. Dostepne 2. Wypozyczone: ");
            int opcja; if (scanf("%d", &opcja) != 1) { while (getchar()!='\n'); continue; }
            printf("ID\tNazwa\t\tStatus\tKlient\tLicznik\n");
            for (int i = 0; i < stan.ile; i++) {
                if (opcja == 1 && stan.lista[i].status == 1) continue;
                if (opcja == 2 && stan.lista[i].status == 0) continue;
                printf("%d\t%-15s\t%s\t%d\t%d\n", stan.lista[i].item_id, stan.lista[i].name,
                       stan.lista[i].status ? "Zajety" : "Wolny", stan.lista[i].borrower_id, stan.lista[i].rental_count);
            }
        }
        else if (menu == 5) { // Raporty i Sortowanie
            if (stan.ile == 0) { printf("Baza pusta.\n"); continue; }
            
            // Sortowanie babelkowe po liczniku
            for (int i = 0; i < stan.ile - 1; i++) {
                for (int j = 0; j < stan.ile - i - 1; j++) {
                    if (stan.lista[j].rental_count < stan.lista[j+1].rental_count) {
                        struct Sprzet tmp = stan.lista[j];
                        stan.lista[j] = stan.lista[j+1];
                        stan.lista[j+1] = tmp;
                    }
                }
            }
            printf("TOP 1: %s (id: %d) - %d razy\n", stan.lista[0].name, stan.lista[0].item_id, stan.lista[0].rental_count);
            printf("Ostatni: %s - %d razy\n", stan.lista[stan.ile-1].name, stan.lista[stan.ile-1].rental_count);
        }
    }

    // Zapis do CSV 
    f = fopen("baza.csv", "w");
    if (f) {
        for (int i = 0; i < stan.ile; i++) {
            fprintf(f, "%d;%s;%d;%d;%d\n", stan.lista[i].item_id, stan.lista[i].name,
                    stan.lista[i].status, stan.lista[i].borrower_id, stan.lista[i].rental_count);
        }
        fclose(f);
    } else {
        fprintf(stderr, "Nie mozna otworzyc pliku do zapisu.\n");
    }

    // Zwolnienie pamieci
    free(stan.lista);
    return 0;
}