#include "PPintrin.h"
#include <string> 
#include <algorithm>
using namespace std;

#define Overflow 9.999999

// implementation of absSerial(), but it is vectorized using PP intrinsics
void absVector(float *values, float *output, int N)
{
  __pp_vec_float x;
  __pp_vec_float result;
  __pp_vec_float zero = _pp_vset_float(0.f);
  __pp_mask maskAll, maskIsNegative, maskIsNotNegative;

  //  Note: Take a careful look at this loop indexing.  This example
  //  code is not guaranteed to work when (N % VECTOR_WIDTH) != 0.
  //  Why is that the case?
  for (int i = 0; i < N; i += VECTOR_WIDTH)
  {
    // All ones
    maskAll = _pp_init_ones();

    // All zeros
    maskIsNegative = _pp_init_ones(0);

    // Load vector of values from contiguous memory addresses
    _pp_vload_float(x, values + i, maskAll); // x = values[i];

    // Set mask according to predicate
    _pp_vlt_float(maskIsNegative, x, zero, maskAll); // if (x < 0) {

    // Execute instruction using mask ("if" clause)
    _pp_vsub_float(result, zero, x, maskIsNegative); // output[i] = -x;

    // Inverse maskIsNegative to generate "else" mask
    maskIsNotNegative = _pp_mask_not(maskIsNegative); // } else {

    // Execute instruction ("else" clause)
    _pp_vload_float(result, values + i, maskIsNotNegative); //   output[i] = x; }

    // Write results back to memory
    _pp_vstore_float(output + i, result, maskAll);
  }

}

void clampedExpVector(float *values, int *exponents, float *output, int N)
{
  //
  // PP STUDENTS TODO: Implement your vectorized version of
  // clampedExpSerial() here.
  //
  // Your solution should work for any value of
  // N and VECTOR_WIDTH, not just when VECTOR_WIDTH divides N
  //
  int loopNum = 0, exp_len = sizeof(exponents) / sizeof(int);
  string msg;
  __pp_vec_float x; // input data
  __pp_vec_int exp; // input exp
  __pp_vec_float result; // return result
  __pp_vec_int zero = _pp_vset_int(0), one = _pp_vset_int(1);
  __pp_vec_float overflow = _pp_vset_float(Overflow);
  __pp_mask maskAll, maskIsNegative, maskIsPositive, maskIsZero, expMask;
  maskAll = _pp_init_ones(); // all lane is active
  maskIsNegative = _pp_init_ones(0);
  maskIsPositive = _pp_init_ones(0);
  expMask = _pp_init_ones();
  maskIsZero = _pp_init_ones(0);
  int i;
  bool key = false;
  for (i = 0; i < N; i += VECTOR_WIDTH)
  {
    if((i + VECTOR_WIDTH) > N){
      key = true;
      break;
    }

    // -------- Load vector of values from contiguous memory addresses -------- 
    _pp_vload_int(exp, exponents + i, maskAll); // exp = exponents[i: i + VECTOR_WIDTH];
    _pp_vload_float(x, values + i, maskAll); // x = values[i: i + VECTOR_WIDTH];
    _pp_vset_float(result, 1., maskAll);

    // -------- Set mask -------- 
    _pp_vlt_int(maskIsNegative, exp, zero, maskAll); // if (exp < 0)
    _pp_vgt_int(maskIsPositive, exp, zero, maskAll); // if (exp > 0)
    _pp_veq_int(maskIsZero, exp, zero, maskAll); // if (exp == 0)

    // -------- Exponential computation --------
    // exp > 0
    
    loopNum = *max_element(exponents + i, exponents + i + VECTOR_WIDTH) + 1;
    // msg = "loopNum: " + to_string(loopNum) + "\n";
    // addUserLog(msg.c_str());
    while(loopNum--){
      _pp_vgt_int(expMask, exp, zero, maskAll);
      _pp_vmult_float(result, result, x, expMask); // result *= x
      _pp_vsub_int(exp, exp, one, maskAll);
    }
    _pp_veq_int(expMask, exp, zero, maskAll);
    _pp_vmult_float(result, result, x, expMask);

    // judge overflow
    _pp_vgt_float(expMask, result, overflow, maskAll);
    _pp_vset_float(result, Overflow, expMask);

    // Write results back to memory
    _pp_vstore_float(output + i, result, maskAll);
  }

  msg = "key: " + to_string(key) + "\n";
  addUserLog(msg.c_str());
  if(key){
    N -= i;
    maskAll = _pp_init_ones(1);
    for(int j = 0; j < N; j++)
      maskAll.value[i] = 0;
    
    // -------- Load vector of values from contiguous memory addresses -------- 
    _pp_vload_int(exp, exponents + i, maskAll); // exp = exponents[i: i + VECTOR_WIDTH];
    _pp_vload_float(x, values + i, maskAll); // x = values[i: i + VECTOR_WIDTH];
    _pp_vset_float(result, 1., maskAll);

    // -------- Set mask -------- 
    _pp_vlt_int(maskIsNegative, exp, zero, maskAll); // if (exp < 0)
    _pp_vgt_int(maskIsPositive, exp, zero, maskAll); // if (exp > 0)
    _pp_veq_int(maskIsZero, exp, zero, maskAll); // if (exp == 0)

    loopNum = *max_element(exponents + i, exponents + i + N) + 1;
    // msg = "From " + to_string(i) + "To " + to_string(i + N) + "\n";
    msg = "loopNum: " + to_string(loopNum) + "\n";
    addUserLog(msg.c_str());
    while(loopNum--){
      _pp_vgt_int(expMask, exp, zero, maskAll);
      _pp_vmult_float(result, result, x, expMask); // result *= x
      _pp_vsub_int(exp, exp, one, maskAll);
    }
    _pp_veq_int(expMask, exp, zero, maskAll);
    _pp_vmult_float(result, result, x, expMask);

    // Write results back to memory
    _pp_vstore_float(output + i, result, maskAll);
  }

}

// returns the sum of all elements in values
// You can assume N is a multiple of VECTOR_WIDTH
// You can assume VECTOR_WIDTH is a power of 2
float arraySumVector(float *values, int N)
{

  //
  // PP STUDENTS TODO: Implement your vectorized version of arraySumSerial here
  //

  for (int i = 0; i < N; i += VECTOR_WIDTH)
  {
  }

  return 0.0;
}