compile:
	gcc -Wall -g3 -fsanitize=address -pthread sever.c -o server
	gcc -Wall -g3 -fsanitize=address -pthread client.c -o client
FLAGS    = -L /lib64
LIBS     = -lusb-1.0 -l pthread
