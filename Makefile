server: src/server.o src/http.o
	gcc -g src/server.o src/http.o -o server -Wall -Wextra -Werror

src/server.o: src/server.c src/http.h
	gcc -c src/server.c -o src/server.o -Wall -Wextra -Werror

src/http.o: src/http.c src/http.h
	gcc -c src/http.c -o src/http.o -Wall -Wextra -Werror