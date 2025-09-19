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

  if (index < 0 || index > array->length) {
    fprintf(stderr, "Invalid array access R!\n");
    return err;
  }
  return array->data[index];
}

int setElement(cmplx_fa_t* array, size_t index, cmplx_f_t value) {
  if (index < 0 || index > array->length) {
    fprintf(stderr, "Invalid array access W!\n");
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

void swapCmpx(cmplx_f_t*a, cmplx_f_t* b){
  cmplx_f_t tmp;
  tmp = *a;
  *a = *b;
  *b = tmp;
}

void moveToIdx(cmplx_fa_t* a, size_t toMove, size_t toHere){
  size_t repeatSwap = 0;

  if(toMove < toHere){
    fprintf(stderr, "Invalid moveToIdx operation\n");
    return;
  }

  repeatSwap = toMove - toHere;

  while(repeatSwap > 0){
    swapCmpx(&a->data[toMove], &a->data[toMove-1]);
    repeatSwap --;
  }
}
