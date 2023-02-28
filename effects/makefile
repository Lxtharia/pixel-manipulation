
# main app
process-image.out: process-image.c effects/v2-pixelsort-effect.c
	gcc $< -o $@

# any shared lib
lib%.so: %.o
	gcc -shared $< -o $@

# any c
%.o: %.c
	gcc -fPIC -c $< -o $@

.PHONY: clean
clean:
	rm ./*.o
