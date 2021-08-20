# Makefile for CMM

ifeq ($(OS), Windows_NT)

all:
	if not exist ..\bin (mkdir ..\bin)
	g++ -DNDEBUG -O3 -std=gnu++17 -o ..\bin\CMM CMM.cpp -lboost_program_options

debug:
	if not exist ..\bin (mkdir ..\bin)
	g++ -g -std=gnu++17 -Wall -o ..\bin\CMM CMM.cpp -lboost_program_options

clean:
	if exist ..\bin\CMM.exe (del /F ..\bin\CMM.exe)

else

all:
	mkdir -p ../bin
	g++ -DNDEBUG -O3 -std=gnu++17 -o ../bin/CMM CMM.cpp -lboost_program_options

debug:
	mkdir -p ../bin
	g++ -g -std=gnu++17 -Wall -o ../bin/CMM CMM.cpp -lboost_program_options

clean:
	rm -f ../bin/CMM

endif
