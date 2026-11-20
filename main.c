#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "initialisieren.h"
#include "hinzufugen.h"
#include "loschen.h"
#include "anzeigen.h"
#include "andern.h"
#include "sqlite3.h"

HINSTANCE hInst;
HWND hMainWnd, hListView;
HWND hButton1, hButton2, hButton3, hButton4, hButton5;
HWND hStatus;
sqlite3 *db;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void AddListViewColumns(HWND hWndListView) {
    LVCOLUMN lvColumn;
    int iCol;

    char* columnNames[] = {"VIN", "Typ", "Marke", "Modell", "Jahr", "Laufleistung", "Hubraum", "Emissionen", "Beschreibung", "Preis"};

    for (iCol = 0; iCol < 10; iCol++) {
        lvColumn.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
        lvColumn.pszText = columnNames[iCol];
        lvColumn.cx = 105;
        lvColumn.iSubItem = iCol;
        ListView_InsertColumn(hWndListView, iCol, &lvColumn);
    }
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = {0};
    MSG msg;

    hInst = hInstance;

    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "AUTOSManagementApp";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClass(&wc))
        return -1;

        SetWindowText(hStatus, "WILKOMMEN SIE Autohändler!!");

    hMainWnd = CreateWindow(wc.lpszClassName, "AUTOS Management Programm", WS_OVERLAPPEDWINDOW,
                            CW_USEDEFAULT, CW_USEDEFAULT, 1100, 600, NULL, NULL, hInstance, NULL);

    if (!hMainWnd)
        return -1;

    ShowWindow(hMainWnd, nCmdShow);
    UpdateWindow(hMainWnd);

    initialisieren();

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static int aus = 0;
    switch (message) {
       case WM_CREATE:
{

    hButton1 = CreateWindow("BUTTON", "Neues Auto hinzufügen", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                            10, 10, 200, 30, hWnd, (HMENU)1, hInst, NULL);
    hButton2 = CreateWindow("BUTTON", "Auto verkaufen", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                            10, 50, 200, 30, hWnd, (HMENU)2, hInst, NULL);
    hButton3 = CreateWindow("BUTTON", "Autos anzeigen", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                            10, 90, 200, 30, hWnd, (HMENU)3, hInst, NULL);
    hButton4 = CreateWindow("BUTTON", "Daten ändern", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                            10, 130, 200, 30, hWnd, (HMENU)4, hInst, NULL);
    hButton5 = CreateWindow("BUTTON", "Programm beenden", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                            10, 170, 200, 30, hWnd, (HMENU)5, hInst, NULL);
    hStatus = CreateWindow("STATIC", "", WS_VISIBLE | WS_CHILD | SS_LEFT,
                           10, 210, 760, 30, hWnd, NULL, hInst, NULL);


    RECT rcClient;
    GetClientRect(hWnd, &rcClient);

    int listViewWidth = rcClient.right - 0;

    hListView = CreateWindow(WC_LISTVIEW, "", WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT,
                             10, 250, listViewWidth, rcClient.bottom - 280, hWnd, NULL, hInst, NULL);


    AddListViewColumns(hListView);
}
break;


        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case 1:
                    hinzufugen();
                    SetWindowText(hStatus, "Neues Auto hinzugefügt");
                    break;
                case 2:
                    loschen();
                    SetWindowText(hStatus, "Auto verkauft");
                    break;
                case 3:
                    {
                        int subwahl = MessageBox(hWnd, "Wählen Sie eine Aktion (IN TERMINAL EINGEBEN!!!!!!!):\n\n1: Alle Autos anzeigen\n2: Autos einer bestimmten Marke anzeigen\n3: Autos sortiert anzeigen", "Autos anzeigen", MB_ICONWARNING | MB_OK);
                        if (subwahl == IDOK) {
                            int aktion;
                            scanf("%d", &aktion);
                            switch (aktion) {
                                case 1:
                                    anzeigen(hWnd, hListView, 1);
                                    break;
                                case 2:
                                    anzeigen(hWnd, hListView, 2);
                                    break;
                                case 3:
                                    anzeigen(hWnd, hListView, 3);
                                    break;
                                default:
                                    MessageBox(hWnd, "Ungültige Auswahl!", "Fehler", MB_OK);
                            }
                        }
                    }
                    break;
                case 4:
                    MessageBox(hWnd, "\nMenü:\n1. Auto aktualisieren\n2. Kunde aktualisieren\n3. Zurück", "Daten ändern", MB_OK);
                    scanf("%d", &aus);
                    getchar();
                    switch (aus) {
                        case 1:
                            update_autos();
                            SetWindowText(hStatus, "Auto aktualisiert");
                            break;
                        case 2:
                            update_kunden();
                            SetWindowText(hStatus, "Kunde aktualisiert");
                            break;
                        case 3:
                            SetWindowText(hStatus, "Zurück zum Hauptmenü");
                            break;
                        default:
                            MessageBox(hWnd, "Ungültige Auswahl, bitte versuchen Sie es erneut.", "Fehler", MB_OK);
                    }
                    break;
                case 5:
                    PostMessage(hWnd, WM_CLOSE, 0, 0);
                    break;
                default:
                    MessageBox(hWnd, "Ungültige Auswahl!", "Fehler", MB_OK);
                    break;
            }
            break;

        case WM_DESTROY:
            sqlite3_close(db);
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
