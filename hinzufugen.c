#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hinzufugen.h"
#include "sqlite3.h"
#include "initialisieren.h"

    extern sqlite3 *db;
    extern int rc;
    extern char *sql, *sql2, *sql3;
    extern char *fehlernachricht;

void hinzufugen (){
    char typ[20], vin[18], hubraum[5], ausstoss[4], modell[20], marke[20], jahr[4], laufleistung[4], beschreibung[1000], preis[10], sql[500], sqlTeil[10];
    initialisieren();

    fflush(stdin);
    printf("Geben Sie die VIN ein: ");
    fgets(vin, 20, stdin);
    printf("Geben Sie das Autotyp ein: ");
    fgets(typ, 20, stdin);
    printf("Um welcher Automarke handelt es sich?: ");
    fgets(marke, 20, stdin);
    printf("Geben Sie das Automodell ein: ");
    fgets(modell, 20, stdin);
    printf("Geben Sie den Hubraum ein: ");
    fgets(hubraum, 20, stdin);
    printf("Geben Sie die Co2-Emissionen ein: ");
    fgets(ausstoss, 20, stdin);
    printf("Geben Sie das Erstzulassungsjahr ein: ");
    fgets(jahr, 20, stdin);
    printf("Geben Sie die Laufleistung ein (in Tkm): ");
    fgets(jahr, 20, stdin);
    printf("Geben Sie eine Beschreibung ein: ");
    fgets(beschreibung, 20, stdin);
    printf("Geben Sie den Wert des Autos ein: ");
    fgets(preis, 20, stdin);

    strcpy(sql, "INSERT INTO autos (VIN, Typ, Marke, Modell, Jahr, "
        "Hubraum, Emmisionen, Beschreibung, Preis) VALUES (");
    strcat(sql, vin);
    strcpy(sqlTeil, ", '");
    strcat(sql, sqlTeil);
    strcat(sql, typ);
    strcat(sql, sqlTeil);
    strcat(sql, marke);
    strcat(sql, sqlTeil);
    strcat(sql, modell);
    strcat(sql, sqlTeil);
    strcat(sql, hubraum);
    strcat(sql, sqlTeil);
    strcat(sql, ausstoss);
    strcat(sql, sqlTeil);
    strcat(sql, jahr);
    strcat(sql, sqlTeil);
    strcat(sql, laufleistung);
    strcat(sql, sqlTeil);
    strcat(sql, beschreibung);
    strcat(sql, sqlTeil);
    strcat(sql, preis);
    strcpy(sqlTeil, ");");
    strcat(sql, sqlTeil);
    rc = sqlite3_exec(db, sql, 0, 0, &fehlernachricht);
    if(rc != 0){
        printf("Datenbankfehler: %s\n", fehlernachricht);
        sqlite3_free(fehlernachricht);
        exit(EXIT_FAILURE);
    }
    sqlite3_close(db);
}
