#include <PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/math.h>

// NOTE: the C/C++ mangling and weak symbol business here is a fake match
// but getting it to be right requires a lot more context

float fabsf__Ff(float);

const float tmp_float[]
    = { 0.25F, 0.023239374F, 0.00000017055572F, 1.867365e-11F };

float __four_over_pi_m1[4] = { 0 };

#define __two_over_pi        0.63661975F
#define __SQRT_FLT_EPSILON__ 3.4526698300e-4F

extern const float __sincos_poly[];
extern const float __sincos_on_quadrant[];

static void __sinit_trigf_c(void)
{
	__four_over_pi_m1[0] = tmp_float[0];
	__four_over_pi_m1[1] = tmp_float[1];
	__four_over_pi_m1[2] = tmp_float[2];
	__four_over_pi_m1[3] = tmp_float[3];
}

#pragma dont_inline on

float sinf(float x)
{
	float z = __two_over_pi * x;
	// note we multiply n by 2 below because the polynomial we are using is for
	// [o,pi/4]. n is the nearest multiple
	//  of pi/2 not pi/4.
	//   frac_part is the remainder(mod(pi/4))
	//  i.e. the actual arg that will be evaluated is frac_part*(pi/4)
	//  note: since n is signed n<<1 may pad rightmost bit w/a one.
	int n = ((*(int*)&x) & 0x80000000) ? (int)(z - .5f) : (int)(z + .5f);

	const float frac_part = ((((x - (float)(n * 2)) + __four_over_pi_m1[0] * x)
	                          + __four_over_pi_m1[1] * x)
	                         + __four_over_pi_m1[2] * x)
	                        + __four_over_pi_m1[3] * x
	    /*) +
	                                                       __four_over_pi_m1[4]*x
	     */
	    ;

	float xsq;

	// assumes 2's complement integer storage for negative numbers.
	n &= 0x00000003;

	if (fabsf__Ff(frac_part) < __SQRT_FLT_EPSILON__) {
		n <<= 1; // index into __sincos_on_quadrant array
		return __sincos_on_quadrant[n]
		       + (__sincos_on_quadrant[n + 1] * frac_part * __sincos_poly[9]);
	}

	xsq = frac_part * frac_part;
	if (n & 0x00000001) // we are at a multiple of pi/2 thus cos(n*pi/4)= 0
	{
		n <<= 1; // index into __sincos_on_quadrant array
		z = (((__sincos_poly[0] * xsq + __sincos_poly[2]) * xsq
		      + __sincos_poly[4])
		         * xsq
		     + __sincos_poly[6])
		        * xsq
		    + __sincos_poly[8];

		return z * __sincos_on_quadrant[n]; // sin(frac_part)*cos(n*pi/4); note:
		                                    // n*pi/4 is a multiple of pi/2(not
		                                    // pi)
		// return z;// sin(frac_part)*cos(n*pi/4);  note: n*pi/4 is a multiple
		// of pi/2(not pi)
	}

	// if here we are near a multiple of pi so sin(n*pi/4) =0
	n <<= 1; // index into __sincos_on_quadrant array

	z = ((((__sincos_poly[1] * xsq + __sincos_poly[3]) * xsq + __sincos_poly[5])
	          * xsq
	      + __sincos_poly[7])
	         * xsq
	     + __sincos_poly[9])
	    * frac_part;
	// return z;// sin(frac_part)*cos(n*pi/4);  note: n*pi/4 is a multiple of
	// pi/2(not pi)

	return z
	       * __sincos_on_quadrant[n
	                              + 1]; // sin(frac_part)*cos(n*pi/4);  note:
	                                    // n*pi/4 is a multiple of pi/2(not pi)
}

float cosf(float x)
{
	float z = __two_over_pi * x;

	// note we multiply n by 2 below because the polynomial we are using is for
	// [o,pi/4]. n is the nearest multiple
	//  of pi/2 not pi/4.
	//   frac_part is the remainder(mod(pi/4))
	//  i.e. the actual arg that will be evaluated is frac_part*(pi/4)
	//  note: since n is signed n<<1 may pad rightmost bit w/a one.
	int n = ((*(int*)&x) & 0x80000000) ? (int)(z - .5f) : (int)(z + .5f);

	const float frac_part = ((((x - (float)(n * 2)) + __four_over_pi_m1[0] * x)
	                          + __four_over_pi_m1[1] * x)
	                         + __four_over_pi_m1[2] * x)
	                        + __four_over_pi_m1[3] * x
	    /*) +
	                                                        __four_over_pi_m1[4]*x
	     */
	    ;

	float xsq;
	n &= 0x00000003;

	if (fabsf__Ff(frac_part) < __SQRT_FLT_EPSILON__) {
		n <<= 1; // index into __sincos_on_quadrant array
		return __sincos_on_quadrant[n + 1]
		       - (__sincos_on_quadrant[n] * frac_part);
	}

	// use identity cos(x)=cos(n*pi/4 + frac_part)=cos(n*pi/4)cos(frac_part)-
	// sin(n*pi/4)sin(frac_part)
	xsq = frac_part * frac_part;
	if (n & 0x00000001) // we are at a multiple of pi/2 thus cos(n*pi/4)= 0
	{
		n <<= 1; // index into __sincos_on_quadrant array
		z = -((((__sincos_poly[1] * xsq + __sincos_poly[3]) * xsq
		        + __sincos_poly[5])
		           * xsq
		       + __sincos_poly[7])
		          * xsq
		      + __sincos_poly[9])
		    * frac_part;
		return z * __sincos_on_quadrant[n];
	}

	n <<= 1; // index into __sincos_on_quadrant array
	// if here we are near a multiple of pi so sin(n*pi/4) =0

	z = (((__sincos_poly[0] * xsq + __sincos_poly[2]) * xsq + __sincos_poly[4])
	         * xsq
	     + __sincos_poly[6])
	        * xsq
	    + __sincos_poly[8];
	return z
	       * __sincos_on_quadrant[n
	                              + 1]; // sin(frac_part)*cos(n*pi/4);  note:
	                                    // n*pi/4 is a multiple of pi/2(not pi)
}

__declspec(weak) float cos__Ff(float x) { return cosf(x); }
__declspec(weak) float sin__Ff(float x) { return sinf(x); }
#pragma dont_inline reset

float tanf(float x) { return sin__Ff(x) / cos__Ff(x); }
