CC = g++
obj = Digit.o main.o AStar.o

solver: $(obj)
	$(CC) $(obj) -o $@

Digit.o: Digit.cpp
	$(CC) -c $^ -o $@

main.o: main.cpp
	$(CC) -c $^ -o $@

AStar.o: AStar.cpp
	$(CC) -c $^ -o $@

clean: 
	rm -f solver $(obj)