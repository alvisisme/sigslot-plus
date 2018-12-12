all: examples

examples: build/simple build/chain

build/simple: build examples/simple.cpp
	g++ examples/simple.cpp -I. -o $@

build/chain: build examples/chain.cpp
	g++ examples/chain.cpp -I. -o $@

build:
	@mkdir -p build

test:
	@./build/simple
	@./build/chain

clean:
	rm -rf build

.PHONY: build test clean 