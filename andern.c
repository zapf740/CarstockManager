#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "andern.h"
#include "sqlite3.h"
#include "initialisieren.h"


void update_record(const char *name, const char *zeile, const char *wert) {
    extern sqlite3 *db;
    extern int rc;
    extern char *fehlernachricht;
    char feld[50], new_wert[100], sql[500];

    while (1) {
        printf("Geben Sie den Feldnamen ein, den Sie aktualisieren möchten (oder 'exit' zum Beenden): ");
        fgets(feld, sizeof(feld), stdin);
        feld[strcspn(feld, "\n")] = '\0';

        if (strcmp(feld, "exit") == 0) {
            break;
        }

        printf("Geben Sie den neuen Wert für %s ein: ", feld);
        fgets(new_wert, sizeof(new_wert), stdin);
        new_wert[strcspn(new_wert, "\n")] = '\0';

        snprintf(sql, sizeof(sql), "UPDATE %s SET %s = '%s' WHERE %s = '%s';", name, feld, new_wert, zeile, wert);

        rc = sqlite3_exec(db, sql, 0, 0, &fehlernachricht);
        if (rc != SQLITE_OK) {
            printf("Datenbankfehler beim Aktualisieren des Datensatzes: %s\n", sqlite3_errmsg(db));
            if (fehlernachricht) {
                printf("Fehlernachricht: %s\n", fehlernachricht);
                sqlite3_free(fehlernachricht);
            }
            continue;
        }

        printf("Datensatz erfolgreich aktualisiert.\n");
    }
}


void update_autos() {
    extern sqlite3 *db;
    sqlite3_stmt *stmt;
    extern int rc;
    char vin[18], sql[500];

    printf("Geben Sie die VIN des Autos ein, das Sie aktualisieren möchten: ");
    scanf("%17s", vin);
    getchar();

    snprintf(sql, sizeof(sql), "SELECT * FROM autos WHERE Vin = '%s';", vin);
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("Datenbankfehler beim Abrufen des Autos: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("Aktualisierung des Autos mit der VIN %s\n", vin);
        sqlite3_finalize(stmt);
        update_record("autos", "Vin", vin);
    } else {
        printf("Keine Daten gefunden für VIN: %s\n", vin);
        sqlite3_finalize(stmt);
    }
}


void update_kunden() {
    extern sqlite3 *db;
    sqlite3_stmt *stmt;
    extern int rc;
    char kunden_id[10], sql[500];

    printf("Geben Sie die KundenID des Kunden ein, den Sie aktualisieren möchten: ");
    scanf("%9s", kunden_id);
    getchar();

    snprintf(sql, sizeof(sql), "SELECT * FROM kunden WHERE KundenID = '%s';", kunden_id);
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("Datenbankfehler beim Abrufen des Kunden: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("Aktualisierung des Kunden mit der KundenID %s\n", kunden_id);
        sqlite3_finalize(stmt);
        update_record("kunden", "KundenID", kunden_id);
    } else {
        printf("Keine Daten gefunden für KundenID: %s\n", kunden_id);
        sqlite3_finalize(stmt);
    }
}


