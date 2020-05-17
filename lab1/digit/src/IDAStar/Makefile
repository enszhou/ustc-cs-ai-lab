CC = g++
obj = main.o AStar.o

solver: $(obj)
	$(CC) $(obj) -o $@

main.o: main.cpp
	$(CC) -c $^ -o $@

AStar.o: AStar.cpp
	$(CC) -c $^ -o $@

clean: 
	rm -f solver $(obj)