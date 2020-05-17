CC = g++
obj = main.o AStar.o

solver.out: $(obj)
	$(CC) $(obj) -o $@

main.o: main.cpp
	$(CC) -c $^ -o $@

AStar.o: AStar.cpp
	$(CC) -c $^ -o $@

clean: 
	rm -f solver.out $(obj)