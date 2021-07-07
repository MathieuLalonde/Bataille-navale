exec: build
	./batnav

build: batnav.c
	gcc -Wall batnav.c -o batnav

deluxe: batnav.c
	gcc -Wall batnav.c -DDELUXE -o batnav
	./batnav

.PHONY: clean

clean:
	rm -f motcabatnavche