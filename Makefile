OBJS = memory_pool.o main.o

main: $(OBJS)
	gcc $(OBJS) -o main
main.o: main.c memory_pool.h
	gcc --std=c++11 -c main.c -o main.o
memory_pool.o: memory_pool.c memory_pool.h
	gcc --std=c++11 -c memory_pool.c -o memory_pool.o
clean:
	rm -rf *.o main
