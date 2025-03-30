# SO2Projekt
## Dining philosophers problem - problem jedzących/ucztujących filozofów
W problemie mowa jest o filozofach siedzących przy jednym stole. Przed każdym z nich jest talerz z jedzeniem, a pomiędzy talerzami znajduje się jeden widelec (czyli każdy filozof posiada jeden widelec).
Filozofowie siedzą, rozmyślają, oraz głodnieją. Gdy zgłodnieją, chcą zjeść swój posiłek, lecz żeby go zjeść, potrzebują dwóch widelców i mogą użyć tylko widelców znajdujących się najbliżej nich (czyli po lewej i prawej stronie jego talerza).
Problem polega na stworzeniu rozwiązania, kktóre uniknie zakleszczenia - filozofowie mogliby podnieść po jednym widelcu i każdy z nich czekałby na odblokowanie drugiego widelca, tworząc blokadę - żaden wątek się nie zwolni, ponieważ czekają na siebie nawzajem.
Rozwiązanie polega na uniknięciu problemów z synchronizacją i dostępem do zasobów.
## Jak uruchomić program
1. Należy wejść z poziomu konsoli do folderu, w którym znajduje się program.
2. Należy uruchomić najpierw komendę ``` g++ -std=c++11 -o dining_philosophers main.cpp DiningPhilosophers.cpp -lpthread ```, a następnie:
   - dla Windowsa należy użyć komendy ```dining_philosophers.exe ```**[liczba filozofów]**
   - dla Linuxa należy użyć komendy ```./dining_philosophers ```**[liczba filozofów]**           
3. Żeby zakończyć program, alezy wpisać losowy znak w konsolę (lub po prostu kliknąć enter)
## Dodatkowe informacje dotyczące projektu
W programie występuje jeden wątek główny - uruchamiający wszystkich filozofów i czeka na zakończenie symulacji.
Każdy z filozofów również jest wątkiem - filozofowie są wektorem wątków. <br> 
### Sekcje krytyczne
- Pobieranie widelców - najpierw filozof musi sprawdzić dostępność obu widelców, zanim zacznie jeść
- Zwracanie widelców - zwracanie zasobów, by mogły być użyte przez innego filozofa 
### Zastosowane rozwiązania                                                                    
- SpinLock - zapobiega równoczesnemu dostępowi wielu filozofów do sekcji krytycznej
- Zastosowanie mechanizmu pierwszeństwa - filozof zawsze próbuje najpierw podnieść widelec z jego lewej strony
- Zastosowanie losowych długości czasów myślenia oraz jedzenia - uniknięcie wykonania tej samej czynności przez wszystkich filozofów w tym samym czasie, by uniknąć deadlocka
