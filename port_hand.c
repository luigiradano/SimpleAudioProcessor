#include "port_hand.h"
#include "main.h"

PaStream* stream = NULL;
cmplx_rb_t dataBuff;

static int pa_callback(const void* inputBuffer, void* outputBuffer, uint64_t framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* transData){
  
  float* inputDataF = (float*) inputBuffer;
  cmplx_rb_t* localBuffer = (cmplx_rb_t*) transData;

  if(inputDataF == NULL || localBuffer == NULL){
    for(uint32_t i = 0; i < framesPerBuffer; i ++){
      appendBuff((cmplx_f_t){0,0}, localBuffer);
    }
    return paContinue;
  }

  for(uint32_t i = 0; i < framesPerBuffer; i ++){
      appendBuff((cmplx_f_t){inputDataF[i],0}, localBuffer);
  }

  return 0;
}

void initPA(uint16_t device, uint32_t audioBuffSize, uint32_t ringBuffSize, uint32_t sampleRate){
  PaError err = Pa_Initialize();
  if(err != paNoError){
    fprintf(stderr, "Error in PA init!\n");
    exit(3);
  }
  
  dataBuff = initRingBuff(ringBuffSize);

  PaStreamParameters inputParams;
  inputParams.device = device;
  inputParams.channelCount = 1;
  inputParams.sampleFormat = paFloat32;
  inputParams.hostApiSpecificStreamInfo = NULL;
  
  err = Pa_OpenStream(
          &stream,
          &inputParams,   /* input parameters */
          NULL,           /* no output */
          sampleRate,
          audioBuffSize,       /* frames per buffer */
          paNoFlag,
          pa_callback,    /* your callback function */
          &dataBuff        /* user data */
      );

  if(err != paNoError){
    fprintf(stderr, "Error opening stream!\n");
    printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    exit(4);
  }
  
}

void startStream(){
  Pa_StartStream(stream);
}

void stopStream(){
  Pa_StopStream(stream);
}

void quitPA(){
  Pa_CloseStream(stream);
  delRingBuff(&dataBuff);
}

cmplx_rb_t* getAudioRingBuff(){
  return &dataBuff;
}
