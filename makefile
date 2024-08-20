#makefile
lab11: lab11.o ghcontrol.o led2472g.o hts221.o lps25h.o
	gcc -g -o lab11 lab11.o ghcontrol.o led2472g.o hts221.o lps25h.o -li2c
lab11.o: lab11.c ghcontrol.h
	gcc -g -c lab11.c
ghcontrol.o: ghcontrol.c ghcontrol.h
	gcc -g -c ghcontrol.c
led2472g.o: led2472g.c led2472g.h
	gcc -g -c led2472g.c
hts221.o: hts221.c hts221.h
	gcc -g -c hts221.c
lps25h.o: lps25h.c lps25h.h
	gcc -g -c lps25h.c

.PHONY:clean
clean:
	rm -f *.o
	touch *

