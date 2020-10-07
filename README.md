# Projekt - steganografia w pliku BMP
## Założenia
Projekt steganografia w pliku BMP polegał na napisaniu programu, który
umieszczał w pliku BMP dowolny plik podany przez użytkownika.
Program potrafi "ukrywać" jedynie pliki wielkości do 65kB w plikach BMP o
głębi kolorów wyższej niż 8 (16, 24, 32).
## Limitacje
Program korzysta wyłącznie z obrazów o głębi kolorów wyższej niż 8,
ponieważ dla głębi kolorów 8 oraz niżej, obrazy BMP korzystają z tablicy
kolorów, gdzie każdy kolor jest jawnie wpisany i każda zmiana nawet jednego
bitu bardzo znacząco wpływa na zmianę koloru danego pixela.

Program ukrywa pliki nie większe niż 65kB, ponieważ wykorzystuje puste
miejsca w nagłówkach plików BMP do przechowywania wielkości ukrytego pliku,
niestety do wykorzystania jest tam jedynie 2 bajty, co pozwala na zapisanie
maksymalnej wielkości pliku właśnie 65kB.
## Użycie
Po sklonowaniu repozytorium należy skompilować program korzystając z
narzędzia `make`. Po wywołaniu `make` w głównym katalogu projektu powinien
utworzyć się podkatalog `bin`. Tam znajduje się plik wywoływalny programu `main`,
w przypadku korzystania z systemu MS Windows plik wykonywalny `main.exe`.

**Do kompilacji projektu wymagane jest posiadanie kompilatora GCC w standardzie
przynajmniej C89 oraz narzędzie `make`**

Program można wywołać z flagą `-h`, wtedy wyświetli się poniższa pomoc:

    Pierwszy argument wywołania programu powninien zawierać następujące flagi: -w lub -r
    Flaga -w służy do zapisywania danych w obrazie
    Flagi, które są po niej wymagane są nastepujące:
    -i -- po niej należy podać plik z danymi, które program ma ukryć
    -p -- po niej należy podać plik obrazu, w którym ma zostać ukryty plik
    -o -- po niej należy podać plik wynikowy, w którym będzie ukryty plik
    -b -- po niej należy podać na ilu bitach ma zostać zapisana informacja (obsługiwane liczby bitów: 1, 2, 4, 8)
    Flaga -r służy do odczytywania danych z obrazu
    Flagi, które są po niej wymagane są następujące:
    -p -- po niej nalezy podać plik obrazu, z którego ma zostać odczytany ukryty plik
    -o -- po niej należy podać plik wynikowy, do którego na zostać odczytana informacja
    -b -- po niej nalezy podać na ilu bitach została zapisana ukryta informacja
    Obsługiwane pliki BMP: 16bit[RGB(1,5,5,5)], 24bit[RGB(8,8,8)], 32bit[ARGB(8,8,8,8), XRBG(8,8,8,8)]
