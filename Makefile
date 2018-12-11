all: build examples

examples: build/example_simple build/example

build/example_simple: examples/example_simple.cpp
	g++ examples/example_simple.cpp -I. -o $@

build/example: examples/example.cpp
	g++ examples/example.cpp -I. -o $@

build:
	mkdir -p build

clean:
	rm -rf build

.PHONY: build clean