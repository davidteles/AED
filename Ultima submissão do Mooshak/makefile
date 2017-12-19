CFLAGS= -Wall  -ansi -O3

autopark: main.o map.o database.o cars.o restrictions.o LinkedList.o escreve_saida.o
	gcc $(CFLAGS) -o autopark main.o map.o database.o cars.o restrictions.o LinkedList.o escreve_saida.o

main.o: main.c main.h map.h database.h cars.h restrictions.h LinkedList.h escreve_saida.h
	gcc $(CFLAGS) -c main.c

map.o: map.c main.h map.h database.h cars.h restrictions.h LinkedList.h escreve_saida.h
	gcc $(CFLAGS) -c map.c

database.o: database.c main.h map.h database.h cars.h restrictions.h LinkedList.h escreve_saida.h
	gcc $(CFLAGS) -c database.c

cars.o: cars.c main.h map.h database.h cars.h restrictions.h LinkedList.h escreve_saida.h
	gcc $(CFLAGS) -c cars.c

restrictions.o: restrictions.c main.h map.h database.h cars.h restrictions.h LinkedList.h escreve_saida.h
	gcc $(CFLAGS) -c restrictions.c

LinkedList.o: LinkedList.c main.h map.h database.h cars.h restrictions.h LinkedList.h escreve_saida.h
	gcc $(CFLAGS) -c LinkedList.c

escreve_saida.o: escreve_saida.c main.h map.h database.h cars.h restrictions.h LinkedList.h escreve_saida.h
	gcc $(CFLAGS) -c escreve_saida.c

clean:
	rm -f *.o *.~ autopark *.gch
