#include <stdio.h>
#include <gmp.h>
#include <mpfr.h>

int main (void)
{
  // This mode specifies round-to-nearest
  mpfr_rnd_t rnd = MPFR_RNDN;

  mpfr_t p, t;

  // allocate unitialized memory for p as 256-bit numbers
  mpfr_init2(p, 256);
  mpfr_init2(t, 256);

  mpfr_set_d (p, 355, rnd);
  mpfr_set_d (t, 113, rnd);

  // a good approx of PI = 355/113
  mpfr_div(p, p, t, rnd);

  // Print Pi to standard out in base 10
  printf ("pi = ");
  mpfr_out_str (stdout, 10, 0, p, rnd);
  putchar ('\n');

  // Release memory
  mpfr_clear(p);
  mpfr_clear(t);
  
  return 0;
}
