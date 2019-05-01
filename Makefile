g9cc: g9cc.c

test: g9cc
	./g9cc -test
	./test.sh

clean:
	rm -f g9cc *.o *~ tmp*
