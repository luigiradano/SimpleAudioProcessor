#include "port_hand.h"

PaStream* stream = NULL;
cmplx_fa_t dataBuff;

static int pa_callback(const void* inputBuffer, void* outputBuffer, uint64_t framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* transData){
  
  float* tmp = (float*) inputBuffer;
  cmplx_f_t* tmpOut = dataBuff.data;

  if(tmp == NULL || tmpOut == NULL){
    for(uint32_t i = 0; i < framesPerBuffer; i ++){
      tmpOut[i].x = 0;
      tmpOut[i].y = 0;
    }
    return paContinue;
  }

  for(uint32_t i = 0; i < framesPerBuffer; i ++){
    tmpOut[i].x = tmp[i];
    tmpOut[i].y = 0;
  }

  return 0;
}

void initPA(uint16_t device, uint32_t buffSize, uint32_t sampleRate){
  PaError err = Pa_Initialize();
  if(err != paNoError){
    fprintf(stderr, "Error in PA init!\n");
    exit(3);
  }
  
  dataBuff = initArray(buffSize);

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
          buffSize,       /* frames per buffer */
          paNoFlag,
          pa_callback,    /* your callback function */
          dataBuff.data        /* user data */
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
  delArray(&dataBuff);
}

cmplx_fa_t getBuff(){
  return dataBuff;
}
cmplx_f_t* getBuffRaw(){
  return dataBuff.data;
}
