third: third.c
	gcc -std=c11 -Wall -Werror -fsanitize=address -o third third.c -lm

clean:
	rm -f third
