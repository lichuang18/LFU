main: main.c lfu.h lfu_impl.h lfu.c 
	g++ -g -o main main.c lfu.h lfu_impl.h lfu.c
clean:
	rm main
