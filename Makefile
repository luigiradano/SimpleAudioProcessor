CFLAGS = -Wall -O2

build: main_o cmplx_math_o fft_o sdl_handle_o port_hand_o hist_o
	gcc build/main.o build/cmplx_math.o build/fft.o build/sdl_handle.o build/port_hand.o build/histogram_sdl.o -o main -lm -lSDL2 -lportaudio 

build_folder: 
	mkdir -p build

clean:
	rm -rf build main

main_o: Src/main.c Inc/cmplx_math.h Inc/main.h| build_folder
	gcc $(CFLAGS) -c Src/main.c -o build/main.o

cmplx_math_o: Src/cmplx_math.c Inc/cmplx_math.h | build_folder
	gcc $(CFLAGS) -c Src/cmplx_math.c -o build/cmplx_math.o

fft_o: Src/fft.c Inc/fft.h Inc/cmplx_math.h | build_folder
	gcc $(CFLAGS) -c Src/fft.c -o build/fft.o

sdl_handle_o: Src/sdl_handle.c Inc/sdl_handle.h | build_folder
	gcc $(CFLAGS) -c Src/sdl_handle.c -o build/sdl_handle.o

hist_o: Src/histogram_sdl.c Inc/histogram_sdl.h | build_folder
	gcc $(CFLAGS) -c Src/histogram_sdl.c -o build/histogram_sdl.o

port_hand_o: Src/port_hand.c Inc/port_hand.h | build_folder
	gcc $(CFLAGS) -c Src/port_hand.c -o build/port_hand.o

debug: CFLAGS += -g
debug: clean build
debug: 
	gdb main

