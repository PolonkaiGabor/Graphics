# Lakás Építő Játék (Sims-szerű szimuláció)

Ez a projekt egy **egyszerű lakásépítő szimuláció**, amely hasonló mechanikákon alapul, mint a *The Sims* játék. A felhasználó falakat és padlót építhet, testre szabhatja a textúrákat, valamint váltogathat nézetek és eszközök között. A cél egy tetszőleges belső tér megtervezése interaktív módon.

## Fő funkciók

- Fal és padló építés különböző nézetekben
- Textúrák váltása gombnyomásra
- Interaktív kamera- és kijelölési rendszer
- Segítség megjelenítése a játék során
- Spot lámpa fényerő szabályozása

## Irányítás

| Gomb | Funkció |
|------|---------|
| **F1** | Súgó panel megjelenítése |
| **W, A, S, D** | Kamera mozgatása a térben |
| **Egér mozgatása + klikkelés** | Kamera forgatása |
| **1 – 7** | Használni kívánt textúra kiválasztása |
| **TAB** | Sor/oszlop nézet váltás (falépítésnél) |
| **M** | Padló/fal építés váltása |
| **Nyilak (← ↑ ↓ →)** | Kijelölt terület mozgatása |
| **Shift** | Több terület kijelölése (kék szín jelzi) |
| **Enter** | Kijelölt terület(ek) megépítése |
| **+ / -** | Spot lámpa fényerő állítása |
| **ESC** | Kilépés a programból |

## Textúrák és grafika

- A textúrák cserélhetők a `1–7` gombok segítségével.
- A felhasználói felület egy egyszerű OpenGL-es 3D térben jelenik meg.
- A Program ikonja AI segítségével készült.

## Fejlesztési információk

A projekt **C nyelven**, **OpenGL** és **SDL2** segítségével készült.

### Követelmények

- SDL2
- OpenGL

### Fordítás
- make -el fordítás majd
- ./"The Sims 5.exe" parancsal futtatás