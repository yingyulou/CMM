all:
	mkdir -p ../bin
	g++ -std=gnu++17 -Wall -DNDEBUG -O3 -o ../bin/CMM Kernel.cpp -lboost_program_options

debug:
	mkdir -p ../bin
	g++ -std=gnu++17 -Wall -g -o ../bin/CMM Kernel.cpp -lboost_program_options

clean:
	rm -f ../bin/CMM
