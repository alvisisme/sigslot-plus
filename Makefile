all: out examples

examples: out/example_simple out/example

out/example_simple: examples/example_simple.cpp
	g++ examples/example_simple.cpp -I. -o $@

out/example: examples/example.cpp
	g++ examples/example.cpp -I. -o $@

out:
	mkdir -p out

clean:
	rm -rf out