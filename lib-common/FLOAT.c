#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {
	long long result = (long long)a * b;
	return result >> 16;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
	/*long long la = a << 16;

	unsigned int result, temp, rel;
	result = 0;
	rel = 0xffffffff / b;
	while (la > (long long)0xffffffff)
	{
		temp = la >> 32;
		result += temp * rel;
		la -= (long long)rel * b * temp;
	}

	result += (unsigned)la / b;
	return (int) result;*/

	unsigned int result1 = a / b;
	unsigned int yushu = a - result1 * b;
	int i = 0;
	int result[16] = {0};
	for (i = 0; i < 16; ++i) {
		unsigned int beichushu = yushu << 1;
		result[i] = beichushu / b;
		yushu = beichushu - result[i] * b;
	}
	unsigned int ans = result1 << 16 + result[0] << 15 + result[1] << 14 + result[2] << 13 + result[3] << 12 + result[4] << 11 + result[5] << 10 + result[6] << 9 + result[7] << 8 + result[8] << 7 + result[9] << 6 + result[10] << 5 + result[11] << 4 + result[12] << 3 + result[13] << 2 + result[14] << 1 + result[15];
	return ans;
}

FLOAT f2F(float a) {
	/*int ieee_exp = *(int *)&a;
	  int sign = (ieee_exp >> 31) & 1;
	  int exp = ((ieee_exp >> 23) & 0xff) - 127;
	  int frac = ieee_exp & 0x7fffff;

	  int result;

	  if (exp > 8)
	  result = ((1 << 23) + frac) << (exp - 8);
	  else
	  result = ((1 << 23) + frac) >> (8 - exp);

	  if (sign) result = -result;
	  return result;*/
	unsigned int *temp;
	temp = (void *)&a;
	int sign = (*temp) >> 31;
	unsigned int exp = 0xff & (*temp >> 23);
	unsigned int frac = 0x7fffff & (*temp);
	unsigned int num = 0x800000 | frac;
	int zhishu = exp - 127 + 16;
	unsigned int result = 0;
	if (zhishu <= 23 && zhishu >= 0) {
		result = num >> (23 - zhishu);
	}
	else if (zhishu > 23 && zhishu < 31) {
		result = num << (zhishu - 23);
	}
	else if (zhishu < 0) {
		result = 0;
	}
	if (sign == 0)
		return result;
	else
		return ((~result) + 1);
}

FLOAT Fabs(FLOAT a) {
	int sign = a >> 31 & 0x1;
	if (sign == 0)
		return -a;
	else
		return a;
}

FLOAT sqrt(FLOAT x) {
	FLOAT dt, t = int2F(2);

	do {
		dt = F_div_int((F_div_F(x, t) - t), 2);
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

FLOAT pow(FLOAT x, FLOAT y) {
	/* we only compute x^0.333 */
	FLOAT t2, dt, t = int2F(2);

	do {
		t2 = F_mul_F(t, t);
		dt = (F_div_F(x, t2) - t) / 3;
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

