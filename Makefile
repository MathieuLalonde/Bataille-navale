exec: build
	./batnav

build: batnav.c
	gcc -Wall batnav.c -o batnav

.PHONY: clean

clean:
	rm -f motcabatnavche