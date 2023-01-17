#include <stdio.h>
#include <stdlib.h>

// Struktura Krawedz reprezentuje pojedyncz� kraw�d� w grafie skierowanym
// z - wierzcho�ek pocz�tkowy kraw�dzi
// dokad - wierzcho�ek ko�cowy kraw�dzi
// nastepna - wska�nik na nast�pn� kraw�d� wychodz�c� z tego samego wierzcho�ka
struct Krawedz {
    int z;
    int dokad;
    struct Krawedz* nastepna;
};

// Struktura Graf reprezentuje graf skierowany
// liczbaWierzcholkow - liczba wierzcho�k�w w grafie
// krawedzie - tablica list kraw�dzi dla ka�dego wierzcho�ka
struct Graf {
    int liczbaWierzcholkow;
    struct Krawedz** krawedzie;
};

// Funkcja tworz�ca graf skierowany z liczb� wierzcho�k�w podan� jako argument
struct Graf* utworzGraf(int liczbaWierzcholkow) {
	// Alokacja pami�ci dla struktury Graf
    struct Graf* g = (struct Graf*) malloc(sizeof(struct Graf));
    g->liczbaWierzcholkow = liczbaWierzcholkow;
    // Alokacja pami�ci dla tablicy list kraw�dzi
    g->krawedzie = (struct Krawedz**) malloc(liczbaWierzcholkow * sizeof(struct Krawedz*));
    // Inicjalizacja tablicy list kraw�dzi jako pustych
    for (int i = 0; i < liczbaWierzcholkow; i++) {
        g->krawedzie[i] = NULL;
    }
    return g;
}
// Funkcja dodaj�ca kraw�d� do grafu skierowanego
// g - wska�nik na graf
// z - wierzcho�ek pocz�tkowy kraw�dzi
// dokad - wierzcho�ek ko�cowy kraw�dzi
void dodajKrawedz(struct Graf* g, int z, int dokad, bool dwukierunkowa) {
    // Alokacja pami�ci dla nowej kraw�dzi
    struct Krawedz* k = (struct Krawedz*) malloc(sizeof(struct Krawedz));
    k->z = z;
    k->dokad = dokad;
    // Dodanie nowej kraw�dzi na pocz�tek listy kraw�dzi dla wierzcho�ka z
    k->nastepna = g->krawedzie[z];
    g->krawedzie[z] = k;

    if(dwukierunkowa){
        // Dodanie kraw�dzi zwrotnej
        struct Krawedz* k2 = (struct Krawedz*) malloc(sizeof(struct Krawedz));
        k2->z = dokad;
        k2->dokad = z;
        k2->nastepna = g->krawedzie[dokad];
        g->krawedzie[dokad] = k2;
    }
}

// Funkcja wypisuj�ca wszystkich s�siad�w dla ka�dego wierzcho�ka w grafie
void wypiszSasiadow(struct Graf* g) {
    for (int i = 0; i < g->liczbaWierzcholkow; i++) {
        printf("Sasiedzi wierzcholka %d: ", i);
        // Pobranie listy kraw�dzi dla wierzcho�ka i
        struct Krawedz* k = g->krawedzie[i];
        while (k != NULL) {
            printf("%d ", k->dokad);
            k = k->nastepna;
        }
        printf("\n");
    }
}

// Funkcja wypisuj�ca wszystkie wierzcho�ki, kt�re s� s�siadami ka�dego wierzcho�ka w grafie
void wypiszSasiadowDlaSasiadow(struct Graf* g) {
    for (int i = 0; i < g->liczbaWierzcholkow; i++) {
        printf("Sasiedzi sasiadow dla wierzcholka %d: ", i);
        // Pobranie listy kraw�dzi dla wierzcho�ka i
        struct Krawedz* k = g->krawedzie[i];
        while (k != NULL) { 
			// Pobranie listy kraw�dzi dla s�siada wierzcho�ka i
            struct Krawedz* k2 = g->krawedzie[k->dokad];
            while (k2 != NULL) {
                if (k2->dokad != i) {
                    printf("%d ", k2->dokad);
                }
                k2 = k2->nastepna;
            }
            k = k->nastepna;
        }
        printf("\n");
    }
}

// Funkcja wypisuj�ca stopnie wychodz�ce dla ka�dego wierzcho�ka w grafie
void wypiszStopnieWychodzace(struct Graf* g) {
    for (int i = 0; i < g->liczbaWierzcholkow; i++) {
        int stopien = 0;
        // Pobranie listy kraw�dzi dla wierzcho�ka i
        struct Krawedz* k = g->krawedzie[i];
        while (k != NULL) {
            stopien++;
            k = k->nastepna;
        }
        printf("Stopien wychodzacy dla wierzcholka %d: %d\n", i, stopien);
    }
}

// Funkcja wypisuj�ca stopnie wchodz�ce dla ka�dego wierzcho�ka w grafie
void wypiszStopnieWchodzace(struct Graf* g) {
	// Tworzenie tablicy przechowuj�cej stopnie wchodz�ce dla ka�dego wierzcho�ka
    int* stopnie = (int*) calloc(g->liczbaWierzcholkow, sizeof(int));
    for (int i = 0; i < g->liczbaWierzcholkow; i++) {
    	// Pobranie listy kraw�dzi dla wierzcho�ka i
        struct Krawedz* k = g->krawedzie[i];
        while (k != NULL) {
            stopnie[k->dokad]++;
            k = k->nastepna;
        }
    }
    for (int i = 0; i < g->liczbaWierzcholkow; i++) {
        printf("Stopien wchodzacy dla wierzcholka %d: %d\n", i, stopnie[i]);
    }
    free(stopnie);
}

// Funkcja wypisuj�ca wszystkie wierzcho�ki izolowane w grafie
void wypiszIzolowane(struct Graf* g) {
    int izolowane = 0;
    for (int i = 0; i < g->liczbaWierzcholkow; i++) {
        if (g->krawedzie[i] == NULL) {
            printf("Wierzcholek %d jest izolowany\n", i);
            izolowane++;
        }
    }
    if (izolowane == 0) {
        printf("Brak izolowanych wierzcholkow\n");
    }
}

// Funkcja wypisuj�ca wszystkie p�tle w grafie
void wypiszPetle(struct Graf* g) {
    for (int i = 0; i < g->liczbaWierzcholkow; i++) {
        struct Krawedz* k = g->krawedzie[i];
        while (k != NULL) {
            if (k->dokad == i) {
                printf("Petla w wierzcholku %d\n", i);
                break;
            }
            k = k->nastepna;
        }
    }
}

// Funkcja wypisuj�ca wszystkie kraw�dzie dwukierunkowe w grafie
void wypiszKrawedzieDwukierunkowe(struct Graf* g) {
    for (int i = 0; i < g->liczbaWierzcholkow; i++) {
        struct Krawedz* k = g->krawedzie[i];
        while (k != NULL) {
            struct Krawedz* k2 = g->krawedzie[k->dokad];
            while (k2 != NULL) {
                if (k2->dokad == i) {
                    printf("Krawedz dwukierunkowa miedzy wierzcholkami %d i %d\n", i, k->dokad);
                    break;
                }
                k2 = k2->nastepna;
            }
            k = k->nastepna;
        }
    }
}

int main() {
    int liczbaWierzcholkow = 6;
    struct Graf* g = utworzGraf(liczbaWierzcholkow);
    dodajKrawedz(g, 0, 1, true);
    dodajKrawedz(g, 0, 2, false);
    dodajKrawedz(g, 1, 2, false);
    dodajKrawedz(g, 3, 3, true);
    dodajKrawedz(g, 2, 3, false);
    dodajKrawedz(g, 3, 4, false);
    // a) wszystkich s�siad�w dla ka�dego wierzcho�ka
    wypiszSasiadow(g);
    printf("\n");
    // b) wszystkie wierzcho�ki, kt�re s� s�siadami ka�dego wierzcho�ka 
    wypiszSasiadowDlaSasiadow(g);
    printf("\n");
    // c) stopnie wychodz�ce wszystkich wierzcho�k�w 
    wypiszStopnieWychodzace(g);
    printf("\n");
    // d) stopnie wchodz�ce wszystkich wierzcho�k�w 
    wypiszStopnieWchodzace(g);
    printf("\n");
    // e) wszystkie wierzcho�ki izolowane
    wypiszIzolowane(g);
    printf("\n");
    // f) wszystkie p�tle
    wypiszPetle(g);
    printf("\n");
    // g) wszystkie kraw�dzie dwukierunkowe
    wypiszKrawedzieDwukierunkowe(g);
    return 0;
}

