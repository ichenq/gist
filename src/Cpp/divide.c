/*
   These functions are examples of unsigned and signed integer division.
   The algorithms assume 32-bit integers.  The algorithms used are
   from
      Digital Computer Arithmetic, by Joseph J.F. Cavanaugh
      McGraw Hill, 1984.
   For an integer of size N, this algorithm will take N steps.  This
   makes it slower the "high-radix" division algorithms.  The algorithm
   has been written with pipelined hardware in mind and where ever
   possible, conditions are avoided in loops.  Assuming a decent
   compiler, this code should also perform well on hardware with 
   branch delay slots.
   If you compile this with Visual C++ and you keep the .c suffix
   use -Tp to default to C++.
   Ian Kaplan, October 1996
   Copyright stuff
   Use of this program, for any purpose, is granted the author,
   Ian Kaplan, as long as this copyright notice is included in
   the source code or any source code derived from this program.
   The user assumes all responsibility for using this code.
*/

void unsigned_divide(unsigned int dividend,
		     unsigned int divisor,
		     unsigned int &quotient,
		     unsigned int &remainder )
{
  unsigned int t, num_bits;
  unsigned int q, bit, d;
  int i;

  remainder = 0;
  quotient = 0;

  if (divisor == 0)
    return;

  if (divisor > dividend) {
    remainder = dividend;
    return;
  }

  if (divisor == dividend) {
    quotient = 1;
    return;
  }

  num_bits = 32;

  while (remainder < divisor) {
    bit = (dividend & 0x80000000) >> 31;
    remainder = (remainder << 1) | bit;
    d = dividend;
    dividend = dividend << 1;
    num_bits--;
  }

  /* The loop, above, always goes one iteration too far.
     To avoid inserting an "if" statement inside the loop
     the last iteration is simply reversed. */
  dividend = d;
  remainder = remainder >> 1;
  num_bits++;

  for (i = 0; i < num_bits; i++) {
    bit = (dividend & 0x80000000) >> 31;
    remainder = (remainder << 1) | bit;
    t = remainder - divisor;
    q = !((t & 0x80000000) >> 31);
    dividend = dividend << 1;
    quotient = (quotient << 1) | q;
    if (q) {
       remainder = t;
     }
  }
}  /* unsigned_divide */



#define ABS(x)  ((x) < 0 ? -(x) : (x))



void signed_divide(int dividend,
		   int divisor,
		   int &quotient,
		   int &remainder )
{
  unsigned int dend, dor;
  unsigned int q, r;

  dend = ABS(dividend);
  dor  = ABS(divisor);
  unsigned_divide( dend, dor, q, r );

  /* the sign of the remainder is the same as the sign of the dividend
     and the quotient is negated if the signs of the operands are
     opposite */
  quotient = q;
  if (dividend < 0) {
    remainder = -r;
    if (divisor > 0)
      quotient = -q;
  }
  else { /* positive dividend */
    remainder = r;
    if (divisor < 0)
      quotient = -q;
  }
  
} /* signed_divide */