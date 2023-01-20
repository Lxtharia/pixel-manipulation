

libpixelsort.so: v2-pixelsort-effect.o
	gcc -shared $< -o $@
	make clean


%.o: %.c
	gcc -c $< -o $@

.PHONY: clean
clean:
	rm ./*.o
