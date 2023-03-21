cc = GCC
CFLAGS= -std=c99 -Wall -Wextra -Werror -pedantic -g 

EXEC = ipkcpc
OBJS = ipkcpc.o 

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC)

.PHONY: clean

clean: 
	rm $(OBJS) $(EXEC)