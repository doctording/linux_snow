.PHONY:clean run

a.out:main_snow.c
	gcc -o $@ $< -lncurses -lpthread

clean:
	rm a.out

run:
	./a.out
