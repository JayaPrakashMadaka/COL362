#all: trie.o
#	g++ -g -O3 trie.o -o exec

#trie.o: trie.cpp
#	g++ -g -c trie.cpp -O3 -o trie.o

#all: pq.o
#	g++ -g -O3 pq.o -o exec

#pq.o: pq.cpp
#	g++ -g -c pq.cpp -O3 -o pq.o


all: final.o
	g++ -g -O3 final.o -o exec

final.o: final.cpp
	g++ -g -c final.cpp -O3 -o final.o

clean:
	rm *.o exec
