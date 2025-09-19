CFLAGS = -Wall -Wextra -O2

build: main_o cmplx_math_o fft_o
	gcc build/main.o build/cmplx_math.o build/fft.o -o main -lm -lSDL2 -lportaudio 

build_folder: 
	mkdir -p build

clean:
	rm -rf build main

main_o: main.c cmplx_math.h main.h| build_folder
	gcc $(CFLAGS) -c main.c -o build/main.o

cmplx_math_o: cmplx_math.c cmplx_math.h | build_folder
	gcc $(CFLAGS) -c cmplx_math.c -o build/cmplx_math.o

fft_o: fft.c fft.h cmplx_math.h | build_folder
	gcc $(CFLAGS) -c fft.c -o build/fft.o

debug:
	gcc main.c cmplx_math.c -o main -lm -lSDL2 -lportaudio
	gdb main

