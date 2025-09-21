CFLAGS = -O2

build: main_o cmplx_math_o fft_o sdl_handle_o port_hand_o
	gcc build/main.o build/cmplx_math.o build/fft.o build/sdl_handle.o build/port_hand.o -o main -lm -lSDL2 -lportaudio 

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

sdl_handle_o: sdl_handle.c sdl_handle.h | build_folder
	gcc $(CFLAGS) -c sdl_handle.c -o build/sdl_handle.o

port_hand_o: port_hand.c port_hand.h | build_folder
	gcc $(CFLAGS) -c port_hand.c -o build/port_hand.o

debug: CFLAGS += -g
debug: clean build
debug: 
	gdb main

