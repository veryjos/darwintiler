all: main

release: NFLAGS=-d:release
release: main

main:
	rm -rf src/nimcache
	nim c --noMain --noLinking --header:src/darwintiler.h src/darwintiler.nim
	nimble build $(NFLAGS)
