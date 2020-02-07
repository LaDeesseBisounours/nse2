all: src/test_cycles.cc
	g++ -lpthread src/test_cycles.cc -o test_cycles
	
