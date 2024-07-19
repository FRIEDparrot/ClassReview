#ifndef FLOAT_TO_STRING
#define FLOAT_TO_STRING

typedef uint8_t UINT8;

void PrecisionHandle(float *value,unsigned char Wrange);
signed char FloatToString(float value,UINT8 int_width,UINT8 Wrange,UINT8 * data,UINT8 len);
#endif //  
