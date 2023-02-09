

# main app
process-image.out: process-image.c effects/v2-pixelsort-effect.c
	gcc $< -o $@

# any shared lib
%.so: %.o
	gcc -shared effects/$< -o effects/$@

# any c
%.o: %.c
	gcc -c $< -o $@

.PHONY: clean
clean:
	rm ./*.o
