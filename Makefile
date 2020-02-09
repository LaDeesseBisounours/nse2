all: test_idt test_cycles


test_idt: src/test_idt.cc
	gcc src/test_idt.cc -o test_idt

test_cycles: src/test_cycles.cc
	g++ -lpthread src/test_cycles.cc -o test_cycles
	
