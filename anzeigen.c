#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "anzeigen.h"
#include "sqlite3.h"
#include "initialisieren.h"

extern sqlite3 *db;
sqlite3_stmt *stmt;
extern int rc;
extern char *sql;
extern char *fehlernachricht;
char Mark[100];
int wahl;

void DisplayResultsInListView(HWND hListView, sqlite3_stmt *stmt) {
    ListView_DeleteAllItems(hListView);
    int row = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        LVITEM lvItem;
        lvItem.mask = LVIF_TEXT;
        lvItem.iItem = row;
        lvItem.iSubItem = 0;
        lvItem.pszText = (char *)sqlite3_column_text(stmt, 0);
        ListView_InsertItem(hListView, &lvItem);

        for (int i = 1; i < 10; i++) {
            ListView_SetItemText(hListView, row, i, (char *)sqlite3_column_text(stmt, i));
        }
        row++;
    }
    sqlite3_finalize(stmt);
}

void sortieren(sqlite3 *db, int wahl, HWND hListView) {
    switch (wahl) {
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
            MessageBox(NULL, "Ungültige Wahl", "Fehler", MB_OK);
            return;
    }

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        MessageBox(NULL, sqlite3_errmsg(db), "SQL Error", MB_OK);
        return;
    }

    DisplayResultsInListView(hListView, stmt);
}

void anzeigen(HWND hWnd, HWND hListView, int wahl) {
    char sql[512];
    switch (wahl) {
        case 1:
            strcpy(sql, "SELECT * FROM autos;");
            break;
        case 2:
            GetWindowText(GetDlgItem(hWnd, 1001), Mark, sizeof(Mark));
            sprintf(sql, "SELECT * FROM autos WHERE UPPER(marke) = UPPER('%s');", Mark);
            break;
        case 3:


            sortieren(db, wahl, hListView);
            return;
        default:
            MessageBox(hWnd, "Ungültige Wahl", "Fehler", MB_OK);
            return;
    }

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        MessageBox(hWnd, sqlite3_errmsg(db), "SQL Error", MB_OK);
        return;
    }

    DisplayResultsInListView(hListView, stmt);
}
