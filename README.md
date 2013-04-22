Compiler-ZUT
============

Kompilator na zajęcia z 'Metod kompilacji'

Zależności:
* bison
* flex
* Kompilator obsługujący C++11 (korzystałem z g++ 4.7.2)
* Biblioteka Boost (headers-only)

Możliwości języka
* Zmienne
* Dwa typy zmiennych int oraz float
* Operacje +,-,*,/
* if, for, while
* warunki złożone z && oraz ||
* tablice jedno i wielowymiarowe
* Funkcje z obsługą parametrów i rekurencją (nie obsługuje zwracania wartości)

Język w założeniu ma przypominać C. Mogą wystąpić wyjątki od reguły, aby zmylić osobę, która będzie próbowała oddać gotowca :)

Język kopiuje wszystkie nie dogoności z języka ANSI C, czyli np. wszystkie deklaracje zmiennych powinny być na początku bloku.
