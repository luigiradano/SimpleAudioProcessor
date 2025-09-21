#include <portaudio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "cmplx_math.h"


#ifndef PORT_HAND
#define PORT_HAND

void initPA(uint16_t device, uint32_t audioBuffSize, uint32_t ringBuffSize, uint32_t sampleRate);

void startStream();
void stopStream();
void quitPA();
cmplx_rb_t* getAudioRingBuff();
#endif


