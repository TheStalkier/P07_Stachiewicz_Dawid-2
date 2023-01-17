#include <stdio.h>
#include <stdlib.h>

// Struktura Krawedz reprezentuje pojedyncz¹ krawêdŸ w grafie skierowanym
// z - wierzcho³ek pocz¹tkowy krawêdzi
// dokad - wierzcho³ek koñcowy krawêdzi
// nastepna - wskaŸnik na nastêpn¹ krawêdŸ wychodz¹c¹ z tego samego wierzcho³ka
struct Krawedz {
    int z;
    int dokad;
    struct Krawedz* nastepna;
};

// Struktura Graf reprezentuje graf skierowany
// liczbaWierzcholkow - liczba wierzcho³ków w grafie
// krawedzie - tablica list krawêdzi dla ka¿dego wierzcho³ka
struct Graf {
    int liczbaWierzcholkow;
    struct Krawedz** krawedzie;
};

// Funkcja tworz¹ca graf skierowany z liczb¹ wierzcho³ków podan¹ jako argument
struct Graf* utworzGraf(int liczbaWierzcholkow) {
	// Alokacja pamiêci dla struktury Graf
    struct Graf* g = (struct Graf*) malloc(sizeof(struct Graf));
    g->liczbaWierzcholkow = liczbaWierzcholkow;
    // Alokacja pamiêci dla tablicy list krawêdzi
    g->krawedzie = (struct Krawedz**) malloc(liczbaWierzcholkow * sizeof(struct Krawedz*));
    // Inicjalizacja tablicy list krawêdzi jako pustych
    for (int i = 0; i < liczbaWierzcholkow; i++) {
        g->krawedzie[i] = NULL;
    }
    return g;
}
// Funkcja dodaj¹ca krawêdŸ do grafu skierowanego
// g - wskaŸnik na graf
// z - wierzcho³ek pocz¹tkowy krawêdzi
// dokad - wierzcho³ek koñcowy krawêdzi
void dodajKrawedz(struct Graf* g, int z, int dokad, bool dwukierunkowa) {
    // Alokacja pamiêci dla nowej krawêdzi
    struct Krawedz* k = (struct Krawedz*) malloc(sizeof(struct Krawedz));
    k->z = z;
    k->dokad = dokad;
    // Dodanie nowej krawêdzi na pocz¹tek listy krawêdzi dla wierzcho³ka z
    k->nastepna = g->krawedzie[z];
    g->krawedzie[z] = k;

    if(dwukierunkowa){
        // Dodanie krawêdzi zwrotnej
        struct Krawedz* k2 = (struct Krawedz*) malloc(sizeof(struct Krawedz));
        k2->z = dokad;
        k2->dokad = z;
        k2->nastepna = g->krawedzie[dokad];
        g->krawedzie[dokad] = k2;
    }
}

// Funkcja wypisuj¹ca wszystkich s¹siadów dla ka¿dego wierzcho³ka w grafie
void wypiszSasiadow(struct Graf* g) {
    for (int i = 0; i < g->liczbaWierzcholkow; i++) {
        printf("Sasiedzi wierzcholka %d: ", i);
        // Pobranie listy krawêdzi dla wierzcho³ka i
        struct Krawedz* k = g->krawedzie[i];
        while (k != NULL) {
            printf("%d ", k->dokad);
            k = k->nastepna;
        }
        printf("\n");
    }
}

// Funkcja wypisuj¹ca wszystkie wierzcho³ki, które s¹ s¹siadami ka¿dego wierzcho³ka w grafie
void wypiszSasiadowDlaSasiadow(struct Graf* g) {
    for (int i = 0; i < g->liczbaWierzcholkow; i++) {
        printf("Sasiedzi sasiadow dla wierzcholka %d: ", i);
        // Pobranie listy krawêdzi dla wierzcho³ka i
        struct Krawedz* k = g->krawedzie[i];
        while (k != NULL) { 
			// Pobranie listy krawêdzi dla s¹siada wierzcho³ka i
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

// Funkcja wypisuj¹ca stopnie wychodz¹ce dla ka¿dego wierzcho³ka w grafie
void wypiszStopnieWychodzace(struct Graf* g) {
    for (int i = 0; i < g->liczbaWierzcholkow; i++) {
        int stopien = 0;
        // Pobranie listy krawêdzi dla wierzcho³ka i
        struct Krawedz* k = g->krawedzie[i];
        while (k != NULL) {
            stopien++;
            k = k->nastepna;
        }
        printf("Stopien wychodzacy dla wierzcholka %d: %d\n", i, stopien);
    }
}

// Funkcja wypisuj¹ca stopnie wchodz¹ce dla ka¿dego wierzcho³ka w grafie
void wypiszStopnieWchodzace(struct Graf* g) {
	// Tworzenie tablicy przechowuj¹cej stopnie wchodz¹ce dla ka¿dego wierzcho³ka
    int* stopnie = (int*) calloc(g->liczbaWierzcholkow, sizeof(int));
    for (int i = 0; i < g->liczbaWierzcholkow; i++) {
    	// Pobranie listy krawêdzi dla wierzcho³ka i
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

// Funkcja wypisuj¹ca wszystkie wierzcho³ki izolowane w grafie
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

// Funkcja wypisuj¹ca wszystkie pêtle w grafie
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

// Funkcja wypisuj¹ca wszystkie krawêdzie dwukierunkowe w grafie
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
    // a) wszystkich s¹siadów dla ka¿dego wierzcho³ka
    wypiszSasiadow(g);
    printf("\n");
    // b) wszystkie wierzcho³ki, które s¹ s¹siadami ka¿dego wierzcho³ka 
    wypiszSasiadowDlaSasiadow(g);
    printf("\n");
    // c) stopnie wychodz¹ce wszystkich wierzcho³ków 
    wypiszStopnieWychodzace(g);
    printf("\n");
    // d) stopnie wchodz¹ce wszystkich wierzcho³ków 
    wypiszStopnieWchodzace(g);
    printf("\n");
    // e) wszystkie wierzcho³ki izolowane
    wypiszIzolowane(g);
    printf("\n");
    // f) wszystkie pêtle
    wypiszPetle(g);
    printf("\n");
    // g) wszystkie krawêdzie dwukierunkowe
    wypiszKrawedzieDwukierunkowe(g);
    return 0;
}

