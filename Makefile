all: out examples

examples: out/example_simple

out/example_simple: examples/example_simple.cpp
	g++ examples/example_simple.cpp -I. -o $@

out:
	mkdir -p out

clean:
	rm -rf out