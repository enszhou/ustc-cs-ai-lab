CC = g++
obj = main.o IDAStar.o

idastar_solver.out: $(obj)
	$(CC) $(obj) -o $@

main.o: main.cpp
	$(CC) -c $^ -o $@

IDAStar.o: IDAStar.cpp
	$(CC) -c $^ -o $@

clean: 
	rm -f idastar_solver.out $(obj)