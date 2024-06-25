#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "funktionen.h"
#include "sqlite3.h"

typedef struct {
    char typ[100];
    char vin[18];
    int hubraum;
    double verbrauch;
    char modell[500];
    char marke[500];
    int jahr;
    char beschreibung[1000];
    int preis;
} Auto;




int callback(void *nichtVerwendet, int anzahl, char **inhalte, char **spaltennamen) {
        for (int i = 0; i < anzahl; i++) {
            printf("%s = %s\n", spaltennamen[i], inhalte[i]);
        }
        printf("\n\n\n");
    return 0;
}

void vermieten (){
    sqlite3 *db;
    int rc;
    char *fehlernachricht;
    char eingabe[10];
    char sql[100];
    char sqlTeil[5];
    rc = sqlite3_open("/Aufgaben/bin/Release/auto.db", &db);
    printf("Geben Sie die ID der Wohnung ein, die vermietet werden soll: ");
    scanf("%s", eingabe);
    strcpy(sql, "UPDATE Wohnungen SET Vermietet = 1 WHERE ID = ");
    strcat(sql, eingabe);
    strcpy(sqlTeil, ";");
    strcat(sql, sqlTeil);
    printf("\n\n\n");
    rc = sqlite3_exec(db, sql, 0, 0, &fehlernachricht);
    if(rc != 0){
        printf("Datenbankfehler: %s\n", fehlernachricht);
        sqlite3_free(fehlernachricht);
        exit(EXIT_FAILURE);
    }
}

void verkaufen (){
    sqlite3 *db;
    int rc;
    char *fehlernachricht;
    char eingabe[10];
    char sql[100];
    char sqlTeil[5];
    rc = sqlite3_open("/Aufgaben/bin/Release/auto.db", &db);
    printf("Geben Sie die ID der Wohnung ein, die verkauft werden soll: ");
    scanf("%s", eingabe);
    strcpy(sql, "DELETE FROM Wohnungen WHERE ID = ");
    strcat(sql, eingabe);
    strcpy(sqlTeil, ";");
    strcat(sql, sqlTeil);
    printf("\n\n\n");
    rc = sqlite3_exec(db, sql, 0, 0, &fehlernachricht);
    if(rc != 0){
        printf("Datenbankfehler: %s\n", fehlernachricht);
        sqlite3_free(fehlernachricht);
        exit(EXIT_FAILURE);
    }
}



int ausfuhren(sqlite3 *db, char *sql) {
    char *error;
    int rc = sqlite3_exec(db, sql, 0, 0, &error);
    if (rc != 0) {
        printf("SQL fehler: %s\n", error);
    }
    return rc;
}


void einfugen(sqlite3 *db, Auto autod) {
    char sql[5000];
    double doeingabe;


    printf("Automarke: ");
    scanf("%s", autod.marke);

    printf("\nModell des Autos: ");
    scanf("%s", autod.modell);

    printf("\nAutotyp: ");
    scanf("%s", autod.typ);

    printf("\nJahr der Erstzulassung: ");
    scanf("%d", &autod.jahr);

    printf("\nVIN des Autos: ");
    scanf("%s", autod.vin);

    printf("\nHubraum des Motors in ccm: ");
    scanf("%d", &autod.hubraum);

    printf("\nWie viel verbraucht das auf 100km?: ");
    scanf("%lf", &doeingabe);
    autod.verbrauch = doeingabe;

    printf("\nBeschreibung: ");
    scanf("%s", autod.beschreibung);

    printf("\nPreis: ");
    scanf("%d", &autod.preis);


    sprintf(sql, "INSERT INTO autos (vin, hubraum, verbrauch, modell, marke, jahr, beschreibung, preis) VALUES ('%s', %d, %.2f, '%s', '%s', %d, '%s', '%d')",
            autod.vin, autod.hubraum, autod.verbrauch, autod.modell, autod.marke, autod.jahr, autod.beschreibung, autod.preis);

    if (ausfuhren(db, sql) != 0) {
        fprintf(stderr, "Fehler beim Einfügen des Autos in die Datenbank\n");
    } else {
        printf("Auto erfolgreich eingefügt!\n");
    }
}
