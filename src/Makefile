# Makefile for CMM

ifeq ($(OS), Windows_NT)

all:
	if not exist ..\bin (mkdir ..\bin)
	g++ -O3 -o ..\bin\CMM CMM.cpp -lboost_program_options

debug:
	if not exist ..\bin (mkdir ..\bin)
	g++ -g -Wall -o ..\bin\CMM CMM.cpp -lboost_program_options

clean:
	if exist ..\bin\CMM.exe (del /F ..\bin\CMM.exe)

else

all:
	mkdir -p ../bin
	g++ -O3 -o ../bin/CMM CMM.cpp -lboost_program_options

debug:
	mkdir -p ../bin
	g++ -g -Wall -o ../bin/CMM CMM.cpp -lboost_program_options

clean:
	rm -f ../bin/CMM

endif
