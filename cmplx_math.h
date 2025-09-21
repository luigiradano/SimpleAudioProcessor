#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>

#ifndef CMPLX_MATH_INC
#define CMPLX_MATH_INC

#define CMPLX_NO_ERR 0
#define CMPLX_ERR 1

// Typedefs

typedef struct {
  float x;  // Real part
  float y;  // Imaginary part
} cmplx_f_t;

typedef struct {
  double x;  // Real part
  double y;  // Imaginary part
} cmplx_d_t;

// Basic operations

/**
 * @brief  Adds two integers.
 *
 * @param  a First integer
 * @param  b Second integer
 * @return Sum of a and b
 */
cmplx_f_t sum(cmplx_f_t a, cmplx_f_t b);

/*
 *   @brief  Perfomrs the subtraction of a and b, complex numbers
 */
cmplx_f_t sub(cmplx_f_t a, cmplx_f_t b);

/*
 *   @brief  Perfomrs the product of a and b, complex numbers
 */
cmplx_f_t mul(cmplx_f_t a, cmplx_f_t b);

/*
 *   @brief  Computes the absolute value of a
 */
float cAbs(cmplx_f_t a);

/**
 *   @brief  Returns the e^i * arg complex number */
cmplx_f_t cExp(float arg);

/**
 *   @brief  Prints the number passed
 */
void print_cmplx_f(cmplx_f_t num);

/**
 *   @brief Swaps two elements
 */
void swapCmpx(cmplx_f_t*a, cmplx_f_t* b);


// Array handling

#define MIN_ARRAY_LEN 8  // must be power 2

typedef __m256 f_minArr;

typedef struct {
  cmplx_f_t* data;
  size_t length;
} cmplx_fa_t;

/*
 *   @brief  Allocates the data for an array
 *   @return Pointer to the array structure that has been allocated
 */
cmplx_fa_t initArray(size_t length);

/*
 *   @brief  Delete an array
 */
void delArray(cmplx_fa_t* array);

/*
 *   @brief  Sums a and b and puts result in out
 */
int sumArray(cmplx_fa_t* a, cmplx_fa_t* b, cmplx_fa_t* out);

/*
 *   @brief  Computes the product of each element in a and b and puts result in out
 */
int mulArray(cmplx_fa_t* a, cmplx_fa_t* b, cmplx_fa_t* out);

/*
 *     @brief  Returns the element at index from the array
 */
cmplx_f_t getElement(cmplx_fa_t* array, size_t index);

/*
 *     @brief  Sets the element at index from the array
 */
int setElement(cmplx_fa_t* array, size_t index, cmplx_f_t value);

/**
*     @brief  Prints the passed array similarly to matlab
*/
void printCmplx_fa(cmplx_fa_t array);

/**
 *  @brief  Moves element at "toMove" to the index equal to "toHere" swapping all the elements before toMove
 */
void moveToIdx(cmplx_fa_t* a, size_t toMove, size_t toHere);

// Ring buffer handling
typedef enum{
  BUFF_NOT_VISITED,
  BUFF_READ,
  BUFF_WRITE
} visitState_t; 

typedef enum{
  BUFF_NORMAL,
  BUFF_OVERRUN,
  BUFF_UNDERRUN,
} runState_t;

typedef struct {
  cmplx_fa_t array;
  size_t writeIndex;
  size_t readIndex;
  runState_t runState;
  visitState_t* visited;
} cmplx_rb_t; // Complex ring buffer type

/**
*   @brief  Initializes a ring buffer of a given size
*/
cmplx_rb_t initRingBuff(size_t buffSize);

/**
  * @brief  Deletes and frees the memory used by a ring buffer
  */
void delRingBuff(cmplx_rb_t* buff);

/**
*  @brief   Writes the data value to the buffer
*/
void appendBuff(cmplx_f_t data, cmplx_rb_t* buff);

/**
*  @brief   Reads 1 element from the buffer
*/
cmplx_f_t extractBuff(cmplx_rb_t* buff);

/**
*  @brief   Returns an array object with the given length. This is accomplished reading elements from the last readindex up to readindex + length 
*/
void getDataBuff(cmplx_rb_t* buff, cmplx_fa_t* out);

void printBuffHealth(cmplx_rb_t* buff);
#endif  // !CMPLX_MATH_INC
