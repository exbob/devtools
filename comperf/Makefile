ifndef CC
	CC="gcc"
endif

comperf:comperf.c
	$(CC) -Wall $< -o $@ -lrt

clean:
	rm -rf comperf
