#include <stdio.h>
#include "initialisieren.h"
#include "hinzufugen.h"
#include "loschen.h"
#include "anzeigen.h"
#include "sqlite3.h"



int main()
{
    extern sqlite3 *db;
    extern int rc;
    extern char *sql, *sql2, *sql3;
    extern char *fehlernachricht;

    printf("Willkommen zum Verwaltungsprogramm f\x81r Ihre Autos!\n\n\n");
    initialisieren();
    int auswahl = 0;
    a:
    while (auswahl != 5){
        printf("W\x84hlen Sie eine Aktion aus:\n\n");
        printf("1: Neus Auto hinzuf\x81gen\n");
        printf("2: Auto verkaufen\n");
        printf("3: Autos anzeigen\n");
        printf("4: Programm beenden\n\n");
        printf("Ihre Eingabe: ");
        scanf("%i", &auswahl);

        switch (auswahl){
        case 1:
            hinzufugen();
            break;
        case 2:
            loschen();
            break;
        case 3:
            anzeigen();
            break;
        case 4:
            printf("Auf Wiedersehen!");
            break;
        default:
            printf("Ung\x81ltige Eingabe!\n\n");
            goto a;
            break;
        }
    }
    return 0;
}

