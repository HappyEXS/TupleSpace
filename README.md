# Wieloprocesowy system realizujący komunikację w języku komunikacyjnym TupleSpace

### Projekt UXP (Unix - Programowanie i Architektura)

### TupleSpace realizuje trzy główne operacje:
- TSoutpout (krotka) - umieszcza krotkę w przestrzeni
- TSinput (wzorzec) - pobiera i usuwa krotkę z przestrzeni
- TSread (wzorzc) - pobiera bez usuwania krotkę z przestrzeni

### Krotki
Są tablicami dowolnej długości składającymi się z elementów typów: 
string, integer, float. Przykładowe krotki:
- (1, "abc", 3.1415)
- (2.1337, "xyz", 10)
- ("Ala", 3, "ma kota", 43.12)

### Wzorzec 
Jest krotką specyfikującą typy składników krotki. Z przestrzeni zwracana
jest pierwsza krotka która spełnia warunki wzorca. Przykładowe wzorce:
- (int, str, fl)
- (fl, str, int)
- (str, int, str, fl)

## Implementacja

### Serwer
Proces serwera odbiera komunikaty od klientów i dodaje, zwraca lub usuwa krotki z przestrzeni.

Kompilacja: 
```
g++ -o server.o server.cpp Tuple.cpp TupleTemplate.cpp
```
Uruchomienie:
```
./server.o
```

### Klient
Proces klienta posiada konsolowy interfejs który pozwala na wprowadzanie poleceń do przestrzeni krotek.

Kompilacja:
```
g++ -o client.o client.cpp Tuple.cpp TupleTemplate.cpp
```
Uruchomienie:
```
./client.o
```