/*-------------------------------------------------------------------------
 *
 * rint.c
 *    rint() implementation
 *
 * By Pedro Gimeno Fortea, donated to the public domain
 *
 *-------------------------------------------------------------------------
 */

#include <float.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

/*
 * Round to nearest integer, with halfway cases going to the nearest even.
 */
double roundHalf(double x)
{
    double      x_orig;
    double      r;

    /* Per POSIX, NaNs must be returned unchanged. */
    if (isnan(x))
        return x;

    if (x <= 0.0)
    {
        /* Both positive and negative zero should be returned unchanged. */
        if (x == 0.0)
            return x;

        /*
         * Subtracting 0.5 from a number very close to -0.5 can round to
         * exactly -1.0, producing incorrect results, so we take the opposite
         * approach: add 0.5 to the negative number, so that it goes closer to
         * zero (or at most to +0.5, which is dealt with next), avoiding the
         * precision issue.
         */
        x_orig = x;
        x += 0.5;

        /*
         * Be careful to return minus zero when input+0.5 >= 0, as that's what
         * rint() should return with negative input.
         */
        if (x >= 0.0)
            return -0.0;

        /*
         * For very big numbers the input may have no decimals.  That case is
         * detected by testing x+0.5 == x+1.0; if that happens, the input is
         * returned unchanged.  This also covers the case of minus infinity.
         */
        if (x == x_orig + 1.0)
            return x_orig;

        /* Otherwise produce a rounded estimate. */
        r = floor(x);

        /*
         * If the rounding did not produce exactly input+0.5 then we're done.
         */
        if (r != x)
            return r;

        /*
         * The original fractional part was exactly 0.5 (since
         * floor(input+0.5) == input+0.5).  We need to round to nearest even.
         * Dividing input+0.5 by 2, taking the floor and multiplying by 2
         * yields the closest even number.  This part assumes that division by
         * 2 is exact, which should be OK because underflow is impossible
         * here: x is an integer.
         */
        return floor(x * 0.5) * 2.0;
    }
    else
    {
        /*
         * The positive case is similar but with signs inverted and using
         * ceil() instead of floor().
         */
        x_orig = x;
        x -= 0.5;
        if (x <= 0.0)
            return 0.0;
        if (x == x_orig - 1.0)
            return x_orig;
        r = ceil(x);
        if (r != x)
            return r;
        return ceil(x * 0.5) * 2.0;
    }
}


inline double doubleFromBits(uint64_t x)
{
    double f = 0;
    memcpy(&f, &x, sizeof(x));
    return f;
}

void test_round()
{
    const double nan = doubleFromBits(0x7FF8000000000001);
    const double inf = doubleFromBits(0x7FF0000000000000);
    const double neginf = doubleFromBits(0xFFF0000000000000);

    double tests[][2] = {
        { -0.49999999999999994, -0.0 }, // -0.5+epsilon
        { -0.5, -1 },
        { -0.5000000000000001, -1 }, // -0.5-epsilon
        { 0, 0 },
        { 0.49999999999999994, 0 },
        { 0.5, 1 },
        { 0.5000000000000001, 1 }, // 0.5+epsilon
        { 1.390671161567e-309, 0 }, // denormal
        { 2.2517998136852485e+15, 2.251799813685249e+15 }, // 1 bit fraction
        { 4.503599627370497e+15, 4.503599627370497e+15 },  // large integer
        { inf, inf },
        { neginf, neginf },
        { nan, nan },
        { -0.0, -0.0 },
    };

    for (int i = 0; i < sizeof(tests) / sizeof(tests[0]); i++)
    {
        double a = tests[i][0];
        double b = tests[i][1];
        double f = roundHalf(a);
        if (f != b)
        {
            assert(false);
        }
    }
}
