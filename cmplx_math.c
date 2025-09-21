#include "cmplx_math.h"

cmplx_f_t sum(cmplx_f_t a, cmplx_f_t b) {
  cmplx_f_t out = {a.x + b.x, a.y + b.y};
  return out;
}

cmplx_f_t sub(cmplx_f_t a, cmplx_f_t b) {
  cmplx_f_t out = {a.x - b.x, a.y - b.y};
  return out;
}

// Performs multiplication of two complex values
cmplx_f_t mul(cmplx_f_t a, cmplx_f_t b) {
  cmplx_f_t out = {a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x};
  return out;
}

float cAbs(cmplx_f_t a) {
  return sqrtf(a.x * a.x + a.y * a.y);
}

cmplx_f_t cExp(float arg) {
  cmplx_f_t out = {cosf(arg), sinf(arg)};
  return out;
}

void print_cmplx_f(cmplx_f_t num) {
  if (num.y >= 0) {
    printf("%.2f + %.2fi", num.x, num.y);
  } else {
    printf("%.2f - %.2fi", num.x, -1 * num.y);
  }
}

cmplx_fa_t initArray(size_t length) {
  cmplx_fa_t out;

  out.length = length;
  out.data = (cmplx_f_t*)malloc(sizeof(cmplx_f_t) * out.length);

  if (out.data == NULL) {
    fprintf(stderr, "Cannot allocate complex vector!\n");
    exit(1);
  }

  return out;
}

void delArray(cmplx_fa_t* array) {
  free(array->data);
  array->length = 0;
}

int sumArray(cmplx_fa_t* a, cmplx_fa_t* b, cmplx_fa_t* out) {
  if (a->length != b->length || a->length != out->length) {
    return CMPLX_ERR;
  }

  for (size_t i = 0; i < out->length; i++) {
    out->data[i] = sum(a->data[i], b->data[i]);
  }

  return CMPLX_NO_ERR;
}

int mulArray(cmplx_fa_t* a, cmplx_fa_t* b, cmplx_fa_t* out) {
  if (a->length != b->length || a->length != out->length) {
    return CMPLX_ERR;
  }

  for (size_t i = 0; i < out->length; i++) {
    out->data[i] = mul(a->data[i], b->data[i]);
  }

  return CMPLX_NO_ERR;
}

cmplx_f_t getElement(cmplx_fa_t* array, size_t index) {
  cmplx_f_t err = {0, 0};

  if (index > array->length) {
    fprintf(stderr, "Invalid array access R!\n");
    return err;
  }
  return array->data[index];
}

int setElement(cmplx_fa_t* array, size_t index, cmplx_f_t value) {
  if (index > array->length) {
    fprintf(stderr, "Invalid array access W!\n");
    exit(1);
    return CMPLX_ERR;
  }

  array->data[index] = value;

  return CMPLX_NO_ERR;
}

void printCmplx_fa(cmplx_fa_t array) {
  printf("[ ");
  for (size_t i = 0; i < array.length; i++) {
    print_cmplx_f(array.data[i]);
    printf("; ");
  }
  printf("]\n");
}

void swapCmpx(cmplx_f_t* a, cmplx_f_t* b) {
  cmplx_f_t tmp;
  tmp = *a;
  *a = *b;
  *b = tmp;
}

void moveToIdx(cmplx_fa_t* a, size_t toMove, size_t toHere) {
  size_t repeatSwap = 0;

  if (toMove < toHere) {
    fprintf(stderr, "Invalid moveToIdx operation\n");
    return;
  }

  repeatSwap = toMove - toHere;

  while (repeatSwap > 0) {
    swapCmpx(&a->data[toMove], &a->data[toMove - 1]);
    toMove--;
    repeatSwap--;
  }
}

cmplx_rb_t initRingBuff(size_t buffSize) {
  cmplx_rb_t out;

  out.array = initArray(buffSize);
  out.visited = (visitState_t*)malloc(sizeof(visitState_t) * buffSize);

  if (out.visited == NULL) {
    fprintf(stderr, "Error allocating visit array!\n");
    exit(1);
  }

  out.runState = BUFF_NORMAL;
  out.readIndex = 0;
  out.writeIndex = 0;

  return out;
}

void delRingBuff(cmplx_rb_t* buff) {
  free(buff->visited);
  delArray(&buff->array);
}

void appendBuff(cmplx_f_t data, cmplx_rb_t* buff) {
  buff->writeIndex++;

  // Implement ring buffer strategy for the pointer
  if (buff->writeIndex >= buff->array.length) {
    buff->writeIndex -= buff->array.length;
  }

  // Check for overrun/underrun condition
  if (buff->visited[buff->writeIndex] == BUFF_WRITE) {
    //    fprintf(stderr, "Overrun on buffer!\n");
    buff->runState |= BUFF_OVERRUN;
  }

  buff->visited[buff->writeIndex] = BUFF_WRITE;
  setElement(&buff->array, buff->writeIndex, data);
}

cmplx_f_t extractBuff(cmplx_rb_t* buff) {
  buff->readIndex++;

  // Implement ring buffer stategy
  if (buff->readIndex >= buff->array.length) {
    buff->readIndex -= buff->array.length;
  }

  // Check for overrun/underrun
  if (buff->visited[buff->readIndex] == BUFF_READ) {
    // fprintf(stderr, "Underrun on buffer!\n");
    buff->runState |= BUFF_UNDERRUN;
  }

  buff->visited[buff->readIndex] = BUFF_READ;
  return getElement(&buff->array, buff->readIndex);
}

void getDataBuff(cmplx_rb_t* buff, cmplx_fa_t* out) {
  for (size_t i = 0; i < out->length; i++) {
    setElement(out, i, extractBuff(buff));
  }
}

void printBuffHealth(cmplx_rb_t *buff){
  if(buff->runState & BUFF_OVERRUN){
    printf("Overrun on buffer\n");
  }
  if(buff->runState & BUFF_UNDERRUN){
    printf("Underrun on buffer\n");
  }

  buff->runState = BUFF_NORMAL;
}
