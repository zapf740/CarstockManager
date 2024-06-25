#include <stdio.h>
#include <stdlib.h>
#include "initialisieren.h"
#include "sqlite3.h"


    sqlite3 *db;
    int rc;
    char *sql, *sql2, *sql3;
    char *fehlernachricht;



void initialisieren() {

    rc = sqlite3_open("/Aufgaben/bin/Release/auto.db", &db);
    if(rc != 0) {
        printf("Datenbank konnte nicht ge\x94 \bffnet werden: %s\n", sqlite3_errmsg(db));
        exit(EXIT_FAILURE);
    }

    sql = "CREATE TABLE IF NOT EXISTS autos ("
                                   "Vin TEXT PRIMARY KEY Not Null,"
                                   "Typ TEXT,"
                                   "Marke TEXT,"
                                   "Modell TEXT,"
                                   "Jahr INTEGER,"
                                   "Laufleistung INTEGER,"
                                   "Hubraum INTEGER,"
                                   "Emissionen REAL,"
                                   "Beschreibung TEXT,"
                                   "Preis INTEGER"
                                   ");";
    rc = sqlite3_exec(db, sql, 0, 0, &fehlernachricht);

    if(rc != 0){
        printf("Datenbankfehler: %s\n", fehlernachricht);
        sqlite3_free(fehlernachricht);
        exit(EXIT_FAILURE);
    }

    sql2 = "CREATE TABLE IF NOT EXISTS kunden ("
    "KundenID INTEGER PRIMARY KEY AUTOINCREMENT,"
    "Vorname TEXT NOT NULL,"
    "Nachname TEXT NOT NULL,"
    "Adresse TEXT,"
    "Telefonnummer TEXT,"
    "Email TEXT"
    ");";

        rc = sqlite3_exec(db, sql2, 0, 0, &fehlernachricht);

    if(rc != 0){
        printf("Datenbankfehler: %s\n", fehlernachricht);
        sqlite3_free(fehlernachricht);
        exit(EXIT_FAILURE);
    }

    sql3 = "CREATE TABLE IF NOT EXISTS verkäufe ("
    "VerkaufID INTEGER PRIMARY KEY AUTOINCREMENT,"
    "Vin TEXT NOT NULL,"
    "KundenID INTEGER NOT NULL,"
    "Verkaufsdatum DATE,"
    "Preis INTEGER,"
    "FOREIGN KEY (Vin) REFERENCES autos(Vin),"
    "FOREIGN KEY (KundenID) REFERENCES kunden(KundenID)"
    ");";

        rc = sqlite3_exec(db, sql2, 0, 0, &fehlernachricht);

    if(rc != 0){
        printf("Datenbankfehler: %s\n", fehlernachricht);
        sqlite3_free(fehlernachricht);
        exit(EXIT_FAILURE);
    }


    sqlite3_close(db);
}
