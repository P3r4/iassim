m = 200
ma = 0
mb = 9
x = 0
t = hex/a.hex
all: clean encoder iassim

encoder: clean
	gcc -o encoder src/ias-hex-encoder/ias-hex-encoder.c

iassim: clean
	gcc -o iassim src/ias-simulator/*.c

clean:
	rm -f *~
	rm -f src/ias-simulator/*~
	rm -f src/ias-simulator/headers/*~
	rm -f src/ias-hex-encoder/*~
	rm -f hex/*~
	rm -f examples/*~
	rm -f encoder
	rm -f iassim

hex: encoder
	./encoder $(f) $(t)
	
run: iassim
	./iassim $(f) $(m) $(ma) $(mb) $(x) 
