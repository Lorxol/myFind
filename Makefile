CC ?= gcc
CPPFLAGS = -D_DEFAULT_SOURCE
CFLAGS = -Wall -Werror -Wextra -std=c99 -pedantic

all:
	$(CC) $(CFLAGS) $(CPPFLAGS) -o myfind src/main.c src/myfind.c src/parser.c -g

light:
	$(CC) $(CPPFLAGS) -o myfind src/main.c src/myfind.c src/parser.c -g

test: all
	./test/testsuite.sh

testl: light
	./test/testsuite.sh

clean:
	rm myfind
