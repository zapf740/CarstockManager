#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "anzeigen.h"
#include "sqlite3.h"
#include "initialisieren.h"



int callback(void *nichtVerwendet, int anzahl, char **inhalte, char **spaltennamen) {
        for (int i = 0; i < anzahl; i++) {
            printf("%s = %s\n", spaltennamen[i], inhalte[i]);
        }
        printf("\n\n\n");
    return 0;
}

    extern sqlite3 *db;
    sqlite3_stmt *stmt;
    extern int rc;
    extern char *sql, *sql2, *sql3;
    extern char *fehlernachricht;
    char*Mark;
    int wahl;

void sortieren(sqlite3 *db, int wahl) {

    switch(wahl) {
        case 1:
            sql = "SELECT * FROM autos ORDER BY Preis ASC;";
            break;
        case 2:
            sql = "SELECT * FROM autos ORDER BY Preis DESC;";
            break;
        case 3:
            sql = "SELECT * FROM autos ORDER BY Jahr DESC;";
            break;
        case 4:
            sql = "SELECT * FROM autos ORDER BY Jahr ASC;";
            break;
        case 5:
            sql = "SELECT * FROM autos ORDER BY Laufleistung ASC;";
            break;
        case 6:
            sql = "SELECT * FROM autos ORDER BY Laufleistung DESC;";
            break;
        default:
            printf("Ungültige Wahl\n");
            return;
    }

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", sqlite3_errmsg(db));
        return;
    }


    printf("Typ | Marke | Modell | Jahr | Vin | Hubraum | Verbrauch | Beschreibung | Preis | Laufleistung\n");
    printf("-------------------------------------------------------------------------------------------------\n");

    while(sqlite3_step(stmt) == SQLITE_ROW) {
        printf("%s | %s | %s | %d | %s | %d | %.2f | %s | %d | %d\n",
            sqlite3_column_text(stmt, 0),
            sqlite3_column_text(stmt, 1),
            sqlite3_column_text(stmt, 2),
            sqlite3_column_int(stmt, 3),
            sqlite3_column_text(stmt, 4),
            sqlite3_column_int(stmt, 5),
            sqlite3_column_double(stmt, 6),
            sqlite3_column_text(stmt, 7),
            sqlite3_column_int(stmt, 8),
            sqlite3_column_int(stmt, 9));
    }

    sqlite3_finalize(stmt);

}

void anzeigen (){
initialisieren();

    neueEingabe:
    printf("W\x84hlen Sie eine der folgenden Aktionen aus:\n\n");
    printf("1: Alle Objekte anzeigen\n");
    printf("2: Alle Objekte einer Marke anzeigen\n");
    printf("3: Alle Objekte sortiert anzeigen\n");
    printf("4: Zurück\n");
    printf("Ihre Eingabe: ");
    scanf("%i", &wahl);
    printf("\n\n");
    switch (wahl){
    case 1:
        sql = "SELECT * FROM autos;";
        break;
    case 2:
        printf("Geben sie die Marke ein:\n");
        scanf("%s", Mark);
        sql = "SELECT * FROM autos WHERE UPPER(marke) = '";
        strcat(sql, Mark);
        strcat(sql, "');");
        break;
    case 3:
    printf("Nach was möchten Sie sotrieren?\n");
    printf("1: Preis (Aufsteigend)\n");
    printf("2: Preis (Absteigend)\n");
    printf("3: Jüngste zuerst\n");
    printf("4: Älteste zuerst\n");
    printf("5: niedrigste Laufleistung\n");
    printf("6: Höchste Laufleistung\n");
    printf("Ihre Wahl:");
    scanf("%d", &wahl);
    sortieren(db, wahl);
        break;
    case 4:
        main();
        break;
    default:
        printf("\nUng\x81ltige Eingabe!\n");
        goto neueEingabe;
        break;
    }
    rc = sqlite3_exec(db, sql, callback, 0, &fehlernachricht);
    if(rc != 0){
        printf("Datenbankfehler: %s\n", fehlernachricht);
        sqlite3_free(fehlernachricht);
        exit(EXIT_FAILURE);
    }
    sqlite3_close(db);
}
