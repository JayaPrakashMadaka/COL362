#all: trie.o
#	g++ -g -O3 trie.o -o exec

#trie.o: trie.cpp
#	g++ -g -c trie.cpp -O3 -o trie.o

#all: pq.o
#	g++ -g -O3 pq.o -o exec

#pq.o: pq.cpp
#	g++ -g -c pq.cpp -O3 -o pq.o


all: 2020CS10356_2020CS10390.o
	g++ -g -O3 2020CS10356_2020CS10390.o -o exec

2020CS10356_2020CS10390.o: 2020CS10356_2020CS10390.cpp
	g++ -g -c 2020CS10356_2020CS10390.cpp -O3 -o 2020CS10356_2020CS10390.o

clean:
	rm *.o exec
