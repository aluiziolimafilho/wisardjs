
cc=emcc
flags=--bind
bin=test/wisard.js
code=src/wisard_bind.cc
all:
	$(cc) $(flags) -o $(bin) $(code)
