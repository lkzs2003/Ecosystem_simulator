# Symulator Ekosystemu

## Wprowadzenie
Projekt symulatora ekosystemu to aplikacja w języku C z interfejsem użytkownika w Pythonie, symulująca interakcje między populacjami królików i wilków oraz ich środowiskiem na wyspie.

## Architektura

### Uproszczona struktura
```
Symulator-Ekosystemu/
│
├── src/
│   ├── animal.h
│   ├── engine.h
│   ├── rabbit.h
│   ├── wolf.h
│   ├── island.h
│   ├── basic.c
│   ├── engine_sample.c
│   ├── generate_rabits.c
│   ├── wolf_logic.c
│   ├── rabbit_logic.c
│   ├── rng.c
│   └── utils.c
│
├── python/
│   ├── c_bridge.py
│   ├── elements.py
│   ├── gui.py
│   └── requirements.txt
│
├── cmake/
│   ├── CMakeLists.txt
│   └── ...
│
└── README.md
```
- **Silnik symulacji (`engine.c`)**: Centralny moduł zarządzający logiką symulacji, w tym cyklami życia zwierząt i warunkami środowiskowymi.
- **Logika Zwierząt (`rabbit_logic.c`, `wolf_logic.c`)**: Moduły definiujące specyficzne zachowania i dynamikę populacji królików i wilków.
- **Generator Liczb Losowych (`rng.c`)**: Dostarcza mechanizm losowości, kluczowy dla realistycznej symulacji zachowań i zdarzeń.
- **Funkcje Pomocnicze (`utils.c`)**: Zawierają ogólne narzędzia i funkcje wspierające różne aspekty symulacji.
- **Most między C a Pythonem** (`c_bridge.py`): Umożliwia komunikację i przepływ danych między logiką symulacji w C a interfejsem użytkownika w Pythonie.
- **Interfejs Użytkownika** (Python): `gui.py` prezentuje graficzny interfejs użytkownika, umożliwiając konfigurację parametrów symulacji i wizualizację wyników.
- **Zarządzanie konfiguracją:** `local_settings.cfg` przechowuje konfiguracje użytkownika i preferencje dotyczące działania symulatora.

Całość tworzy spójny, modułowy system, który umożliwia elastyczne modelowanie i analizę złożonych ekosystemów.

## Funkcjonalności

- Symulacja dynamiki populacji z uwzględnieniem czynników ekologicznych.
- Możliwość dostosowania parametrów symulacji.
- Wizualizacja danych symulacyjnych.

## Instalacja i Uruchamianie

### Wymagania Wstępne

- Upewnij się, że masz zainstalowane CMake, kompilator języka C oraz Python.

### Klonowanie Repozytorium

- Pobierz kod źródłowy projektu z repozytorium.

### Kompilacja z Użyciem CMake

- Otwórz terminal w katalogu projektu.
- Użyj polecenia `cmake .` aby przygotować projekt do kompilacji.
- Następnie użyj `make` do skompilowania projektu. CMake automatycznie wykorzysta konfiguracje do zbudowania projektu.

### Instalacja Zależności Pythona
  - Zainstaluj wymagane biblioteki Pythona z pliku requirements.txt za pomocą `pip install -r requirements.txt`.

- **Uruchomienie GUI:**
  - W pliku `local_settings.cfg` jak parametr lib_path ***ustaw ścieżkę do pliku libengine.dll bądź libengine.so***, powsatłego po kompilacji silnika
  - Po skompilowaniu uruchom plik gui.py w Pythonie, aby rozpocząć interakcję z symulatorem. uruchomić gui można na przykład poleceniem:
  ```bash
  python gui.py
  ```

## Scenariusze Użycia
- Badanie wpływu różnych czynników na populacje.
- Eksperymenty z różnymi strategiami przetrwania.

## Debugowanie i Rozwiązywanie Problemów
- Dokładny opis znanych błędów i sposoby ich rozwiązania.
- Wskazówki do debugowania specyficznych problemów.


