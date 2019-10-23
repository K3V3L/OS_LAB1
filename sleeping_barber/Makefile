all: clean compile run
compile: source.cpp
	g++ -Wall -pthread source.cpp -o main 
run: main
	./main 5 2
clean: 
	mv main mainbak
