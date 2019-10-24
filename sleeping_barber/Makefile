all: clean compile run
compile: source.cpp
	g++ -Wall -pthread source.cpp -o main 
run: main
	./main sleeping_barber 
clean: 
	mv main mainbak
