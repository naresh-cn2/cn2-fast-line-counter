all:
	gcc -O3 -march=native fast_error_filter_v2.c -o fast_filter_v2

clean:
	rm -f fast_filter_v2
