CFLAGS=-Wall -std=c11
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

g9cc: $(OBJS)
	$(CC) -o g9cc $(OBJS) $(LDFLAGS)

$(OBJS): g9cc.h

test: g9cc
	./g9cc -test
	./test.sh

clean:
	rm -f g9cc *.o *~ tmp*
