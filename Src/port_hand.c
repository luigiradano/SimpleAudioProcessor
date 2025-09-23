#include "../Inc/port_hand.h"

#include "../Inc/main.h"

PaStream* stream = NULL;
cmplx_rb_t inputBuff;
cmplx_rb_t outputBuff;
cmplx_rb_t* bufferObjs[2] = {&inputBuff, &outputBuff};

static int pa_callback(const void* inputBuffer, void* outputBuffer, uint64_t framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* transData) {
  float* inputDataF = (float*)inputBuffer;
  float* outputDataF = (float*)outputBuffer;
  cmplx_rb_t** tmp = transData;
  cmplx_rb_t* localInBuff = (cmplx_rb_t*)tmp[0];
  cmplx_rb_t* localOutBuff = (cmplx_rb_t*)tmp[1];

  if (inputDataF == NULL || localInBuff == NULL) {
    for (uint32_t i = 0; i < framesPerBuffer; i++) {
      appendBuff((cmplx_f_t){0, 0}, localInBuff);
    }
    return paContinue;
  }

  if (outputDataF == NULL) {
    for (uint32_t i = 0; i < framesPerBuffer; i++) {
      appendBuff((cmplx_f_t){inputDataF[i], 0}, localInBuff);
    }
  } else {
    for (uint32_t i = 0; i < framesPerBuffer; i++) {
      appendBuff((cmplx_f_t){inputDataF[i], 0}, localInBuff);
      outputDataF[i] = extractBuff(localOutBuff).x;
    }
  }

  return 0;
}

void initPA(uint16_t device, uint32_t audioBuffSize, uint32_t ringBuffSize, uint32_t sampleRate) {
  PaError err = Pa_Initialize();
  if (err != paNoError) {
    fprintf(stderr, "Error in PA init!\n");
    exit(3);
  }

  inputBuff = initRingBuff(ringBuffSize);
  outputBuff = initRingBuff(ringBuffSize);

  PaStreamParameters inputParams;
  inputParams.device = device;
  inputParams.channelCount = 1;
  inputParams.sampleFormat = paFloat32;
  inputParams.hostApiSpecificStreamInfo = NULL;

  PaStreamParameters outputParams;
  outputParams.device = device;
  outputParams.channelCount = 1;
  outputParams.sampleFormat = paFloat32;
  outputParams.hostApiSpecificStreamInfo = NULL;


  err = Pa_OpenStream(
      &stream,
      &inputParams,  /* input parameters */
      &outputParams, /* no output */
      sampleRate,
      audioBuffSize, /* frames per buffer */
      paNoFlag,
      pa_callback, /* your callback function */
      &bufferObjs   /* user data */
  );

  if (err != paNoError) {
    fprintf(stderr, "Error opening stream!\n");
    printf("PortAudio error: %s\n", Pa_GetErrorText(err));
    exit(4);
  }
}

void startStream() {
  Pa_StartStream(stream);
}

void stopStream() {
  Pa_StopStream(stream);
}

void quitPA() {
  Pa_CloseStream(stream);
  delRingBuff(&inputBuff);
}

cmplx_rb_t* getAudioInRingBuff() {
  return &inputBuff;
}
cmplx_rb_t* getAudioOutRingBuff() {
  return &outputBuff;
}
