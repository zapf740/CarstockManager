#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "hinzufugen.h"
#include "sqlite3.h"
#include "initialisieren.h"

extern sqlite3 *db;
extern int rc;
extern char *fehlernachricht;

int nummer(const char *str) {
    for (int i = 0; i < strlen(str); i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}


void hinzufugen() {
    char typ[20], vin[18], hubraum[5], ausstoss[4], modell[20], marke[20], jahr[4], laufleistung[4], preis[10], sql[1500], beschreibung[500];

    while(1){
    printf("Geben Sie die VIN ein: ");
    scanf("%s", vin);
    if(nummer(vin)){
        break;
    }
    else{
        printf("\nBitte Zahl eingeben!\n\n");
    }
    }
    printf("Geben Sie das Autotyp ein: ");
    scanf("%s", typ);
    getchar();
    printf("Um welcher Automarke handelt es sich?: ");
    scanf("%s", marke);
    getchar();
    printf("Geben Sie das Automodell ein: ");
    scanf("%s", modell);
    getchar();
    while(1){
    printf("Geben Sie den Hubraum ein(Liter): ");
    scanf("%s", hubraum);
        if(nummer(hubraum)){
        break;
    }
    else{
        printf("\nBitte Zahl eingeben!\n\n");
    }
    }
    while(1){
    printf("Geben Sie die Co2-Emissionen ein (g/100km): ");
    scanf("%s", ausstoss);
        if(nummer(ausstoss)){
        break;
    }
    else{
        printf("\nBitte Zahl eingeben!\n\n");
    }
    }
    while(1){
    printf("Geben Sie das Erstzulassungsjahr ein: ");
    scanf("%s", jahr);
    if(nummer(jahr)){
        break;
    }
    else{
        printf("\nBitte Zahl eingeben!\n\n");
    }
    }
    while(1){
    printf("Geben Sie die Laufleistung ein (in Tkm): ");
    scanf("%s", laufleistung);
    if(nummer(laufleistung)){
        break;
    }
    else{
        printf("\nBitte Zahl eingeben!\n\n");
    }
    }
    printf("Geben Sie eine Beschreibung ein: ");
    getchar();
    fgets(beschreibung, sizeof(beschreibung), stdin);
    beschreibung[strcspn(beschreibung, "\n")] = 0;
    printf("Eingegbene Beschreibung: %s\n", beschreibung);
    while(1){
    printf("Geben Sie den Wert des Autos ein: ");
    scanf("%s", preis);
    if(nummer(preis)){
        break;
    }
    else{
        printf("\nBitte Zahl eingeben!\n\n");
    }
    }

    sprintf(sql,"INSERT INTO autos (Vin, Typ, Marke, Modell, Jahr, Laufleistung, Hubraum, Emissionen, Beschreibung, Preis) "
        "VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s');",vin, typ, marke, modell, jahr, laufleistung, hubraum, ausstoss, beschreibung, preis);
    initialisieren();
    rc = sqlite3_exec(db, sql, 0, 0, &fehlernachricht);
    if (rc != SQLITE_OK) {
        printf("Datenbankfehler: %s\n", sqlite3_errmsg(db));
        sqlite3_free(fehlernachricht);
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }

}

