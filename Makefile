
cc=emcc
flags=--bind
bin=dist/wisard.js
code=src/wisard_bind.cc
all:
	$(cc) $(flags) -o $(bin) $(code)
