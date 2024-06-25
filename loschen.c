#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "loschen.h"
#include "sqlite3.h"


void loschen (){
    sqlite3 *db;
    int rc;
    char *fehlernachricht;
    char eingabe[10];
    char sql[100];
    char sqlTeil[5];
    rc = sqlite3_open("makler.db", &db);
    printf("Geben Sie die ID der Wohnung ein, die verkauft werden soll: ");
    scanf("%s", eingabe);
    strcpy(sql, "DELETE FROM Wohnungen WHERE ID = ");
    strcat(sql, eingabe);
    strcpy(sqlTeil, ";");
    strcat(sql, sqlTeil);
    printf("\n\n");
    rc = sqlite3_exec(db, sql, 0, 0, &fehlernachricht);
    if(rc != 0){
        printf("Datenbankfehler: %s\n", fehlernachricht);
        sqlite3_free(fehlernachricht);
        exit(EXIT_FAILURE);
    }
    printf("Verkauf der Wohnung mit der ID %s erfolgreich registriert.\n\n\n", eingabe);
}
