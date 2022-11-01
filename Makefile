CFLAGS=-Wall -O0 -g -std=c++11
OBJS = src/main.o src/parser.o src/register-allocator.o src/scanner.o src/io.o

%.o: %.cpp
	g++ -c -Iinclude -o $@ $< $(CFLAGS)

compiler.out: $(OBJS)
	g++ $(CFLAGS) -o $@ $(OBJS) -lstdc++

.PHONY: clean
clean:
	rm -f src/*.o compiler.out
