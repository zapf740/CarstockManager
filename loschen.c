
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "loschen.h"
#include "sqlite3.h"
#include "hinzufugen.h"
#include "initialisieren.h"

void loschen() {
    extern sqlite3 *db;
    sqlite3_stmt *stmt;
    extern int rc;
    char vinn[18];
    extern char *sql;
    extern char *fehlernachricht;
    char vorname[50], nachname[50], adresse[100], telefonnummer[20], email[50], verkaufsdatum[11];
    int kundenID, preis;

    initialisieren();
    i2();
    i3();


    sql = "SELECT * FROM autos;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        printf("VIN | Typ | Marke | Modell | Jahr | Laufleistung | Hubraum | Emissionen | Beschreibung | Preis | \n");
        printf("-------------------------------------------------------------------------------------------------\n");

    while(sqlite3_step(stmt) == SQLITE_ROW) {
        printf("%s | %s | %s | %s | %d | %d | %d | %d | %s | %d |\n",
            sqlite3_column_text(stmt, 0),
            sqlite3_column_text(stmt, 1),
            sqlite3_column_text(stmt, 2),
            sqlite3_column_text(stmt, 3),
            sqlite3_column_int(stmt, 4),
            sqlite3_column_int(stmt, 5),
            sqlite3_column_int(stmt, 6),
            sqlite3_column_int(stmt, 7),
            sqlite3_column_text(stmt, 8),
            sqlite3_column_int(stmt, 9));
    }
    sqlite3_finalize(stmt);

    if (rc != SQLITE_OK) {
        printf("Datenbankfehler: %s\n", sqlite3_errmsg(db));
        sqlite3_free(fehlernachricht);
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }


    printf("Geben Sie die Vin des Autos ein, das verkauft werden soll: ");
    scanf("%17s", vinn);
    getchar();

    char select_sql[500];
    snprintf(select_sql, sizeof(select_sql), "SELECT * FROM autos WHERE Vin = '%s';", vinn);
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("Datenbankfehler beim Abrufen des Autos: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        // Capture car details
        const char *typ = (const char *)sqlite3_column_text(stmt, 1);
        const char *marke = (const char *)sqlite3_column_text(stmt, 2);
        const char *modell = (const char *)sqlite3_column_text(stmt, 3);
        const char *jahr = (const char *)sqlite3_column_text(stmt, 4);
        const char *hubraum = (const char *)sqlite3_column_text(stmt, 5);
        const char *emissionen = (const char *)sqlite3_column_text(stmt, 6);
        const char *beschreibung = (const char *)sqlite3_column_text(stmt, 7);
        const char *preis_auto = (const char *)sqlite3_column_text(stmt, 8);
        const char *laufleistung = (const char *)sqlite3_column_text(stmt, 9);

        sqlite3_finalize(stmt);


        printf("Geben Sie die KundenID ein: ");
        scanf("%d", &kundenID);
        getchar();

        printf("Geben Sie den Vornamen ein: ");
        fgets(vorname, sizeof(vorname), stdin);
        vorname[strcspn(vorname, "\n")] = '\0';

        printf("Geben Sie den Nachnamen ein: ");
        fgets(nachname, sizeof(nachname), stdin);
        nachname[strcspn(nachname, "\n")] = '\0';

        printf("Geben Sie die Adresse ein: ");
        fgets(adresse, sizeof(adresse), stdin);
        adresse[strcspn(adresse, "\n")] = '\0';

        printf("Geben Sie die Telefonnummer ein: ");
        fgets(telefonnummer, sizeof(telefonnummer), stdin);
        telefonnummer[strcspn(telefonnummer, "\n")] = '\0';

        printf("Geben Sie die Email ein: ");
        fgets(email, sizeof(email), stdin);
        email[strcspn(email, "\n")] = '\0';

        char kunden_sql[500];
        snprintf(kunden_sql, sizeof(kunden_sql),
                 "INSERT INTO kunden (Vorname, Nachname, Adresse, Telefonnummer, Email) "
                 "VALUES ('%s', '%s', '%s', '%s', '%s');",
                 vorname, nachname, adresse, telefonnummer, email);
        rc = sqlite3_exec(db, kunden_sql, 0, 0, &fehlernachricht);
        if (rc != SQLITE_OK) {
            printf("Datenbankfehler beim Erstellen des Kunden: %s\n", sqlite3_errmsg(db));
            if (fehlernachricht) {
                printf("Fehlernachricht: %s\n", fehlernachricht);
                sqlite3_free(fehlernachricht);
            }
            sqlite3_close(db);
            exit(EXIT_FAILURE);
        }


        int kundenID = sqlite3_last_insert_rowid(db);


        printf("Geben Sie das Verkaufsdatum (YYYY-MM-DD) ein: ");
        fgets(verkaufsdatum, sizeof(verkaufsdatum), stdin);
        verkaufsdatum[strcspn(verkaufsdatum, "\n")] = '\0';

        printf("Geben Sie den Verkaufspreis ein: ");
        scanf("%d", &preis);

        char verkauf_sql[500];
        snprintf(verkauf_sql, sizeof(verkauf_sql),
                 "INSERT INTO verkauf (Vin, KundenID, Verkaufsdatum, Preis) "
                 "VALUES ('%s', %d, '%s', %d);",
                 vinn, kundenID, verkaufsdatum, preis);
        rc = sqlite3_exec(db, verkauf_sql, 0, 0, &fehlernachricht);
        if (rc != SQLITE_OK) {
            printf("Datenbankfehler beim Einf gen des Verkaufs: %s\n", sqlite3_errmsg(db));
            if (fehlernachricht) {
                printf("Fehlernachricht: %s\n", fehlernachricht);
                sqlite3_free(fehlernachricht);
            }
            sqlite3_close(db);
            exit(EXIT_FAILURE);
        }

        printf("Verkauf erfolgreich hinzugef gt.\n");


        char delete_sql[200];
        snprintf(delete_sql, sizeof(delete_sql), "DELETE FROM autos WHERE Vin = '%s';", vinn);


        char join_sql[1000];
        snprintf(join_sql, sizeof(join_sql),
                 "SELECT a.Typ, a.Marke, a.Modell, a.Jahr, a.VIN, a.Hubraum, a.Emissionen, a.Beschreibung, a.Preis, a.Laufleistung, "
                 "k.Vorname, k.Nachname, k.Adresse, k.Telefonnummer, k.Email, "
                 "v.Verkaufsdatum, v.Preis "
                 "FROM verkauf v "
                 "JOIN kunden k ON v.KundenID = k.KundenID "
                 "LEFT JOIN autos a ON v.Vin = a.VIN "
                 "WHERE v.Vin = '%s';", vinn);

        printf("Debug: SQL Query - %s\n", join_sql); // Debugging SQL query

        rc = sqlite3_prepare_v2(db, join_sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            printf("Datenbankfehler beim Erstellen der Rechnung: %s\n", sqlite3_errmsg(db));
            sqlite3_free(fehlernachricht);
            sqlite3_close(db);
            exit(EXIT_FAILURE);
        }

        printf("Rechnung:\n");
        printf("-------------------------------------------------------------------------------------------------\n");
        int row_count = 0;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            row_count++;
            printf("Typ: %s\n", sqlite3_column_text(stmt, 0));
            printf("Marke: %s\n", sqlite3_column_text(stmt, 1));
            printf("Modell: %s\n", sqlite3_column_text(stmt, 2));
            printf("Jahr: %d\n", sqlite3_column_int(stmt, 3));
            printf("VIN: %s\n", sqlite3_column_text(stmt, 4));
            printf("Hubraum: %d\n", sqlite3_column_int(stmt, 5));
            printf("Emissionen: %d\n", sqlite3_column_int(stmt, 6));
            printf("Beschreibung: %s\n", sqlite3_column_text(stmt, 7));
            printf("Preis: %d\n", sqlite3_column_int(stmt, 8));
            printf("Laufleistung: %d\n", sqlite3_column_int(stmt, 9));
            printf("Vorname: %s\n", sqlite3_column_text(stmt, 10));
            printf("Nachname: %s\n", sqlite3_column_text(stmt, 11));
            printf("Adresse: %s\n", sqlite3_column_text(stmt, 12));
            printf("Telefonnummer: %s\n", sqlite3_column_text(stmt, 13));
            printf("Email: %s\n", sqlite3_column_text(stmt, 14));
            printf("Verkaufsdatum: %s\n", sqlite3_column_text(stmt, 15));
            printf("Verkaufspreis: %s\n", sqlite3_column_text(stmt, 16));
            printf("-------------------------------------------------------------------------------------------------\n");
        }
        sqlite3_finalize(stmt);

        if (row_count == 0) {
            printf("Keine Daten gefunden f r VIN: %s\n", vinn);
        }

        printf("Verkauf des Autos mit der VIN %s erfolgreich registriert.\n\n\n", vinn);


                rc = sqlite3_exec(db, delete_sql, 0, 0, &fehlernachricht);
        if (rc != SQLITE_OK) {
            printf("Datenbankfehler1: %s\n", sqlite3_errmsg(db));
            sqlite3_free(fehlernachricht);
            sqlite3_close(db);
            exit(EXIT_FAILURE);
        }
    }
     else {
        printf("Keine Daten gefunden f r VIN: %s\n", vinn);
        sqlite3_finalize(stmt);
    }





}

