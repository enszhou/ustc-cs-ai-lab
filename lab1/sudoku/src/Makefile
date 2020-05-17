CC = g++
obj = Sudoku.o main.o

solver: $(obj)
	$(CC) $(obj) -o $@

Sudoku.o: Sudoku.cpp
	$(CC) -c $^ -o $@

main.o: main.cpp
	$(CC) -c $^ -o $@

clean: 
	rm -f solver $(obj)