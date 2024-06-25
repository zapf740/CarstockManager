CREATE TABLE Kunde (
    Vorname      TEXT    NOT NULL,
    Nachname     TEXT    NOT NULL,
    VIN          TEXT    REFERENCES autos (vin) 
                         NOT NULL,
    Kundennummer INTEGER PRIMARY KEY AUTOINCREMENT
);


CREATE TABLE autos (
    typ          TEXT,
    marke        TEXT    NOT NULL,
    modell       TEXT    NOT NULL,
    jahr         INTEGER NOT NULL,
    vin          TEXT    PRIMARY KEY
                         NOT NULL,
    hubraum      INTEGER NOT NULL,
    verbrauch    REAL,
    beschreibung TEXT,
    preis        INTEGER NOT NULL
);

