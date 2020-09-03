#include <math.h>

/*
float powf(float _X,float _Y) { return ((float)pow((double)_X,(double)_Y)); }
float sinf(float _X) { return ((float)sin((double)_X)); }
float sinhf(float _X) { return ((float)sinh((double)_X)); }
float sqrtf(float _X) { return ((float)sqrt((double)_X)); }
float tanf(float _X) { return ((float)tan((double)_X)); }
float tanhf(float _X) { return ((float)tanh((double)_X)); }
float cosf(float _X) { return ((float)cos((double)_X)); }
*/
float sqrtf  (float f)            { return (float)sqrt(f); }
float asinf  (float f)            { return (float)asin(f); }
float cosf   (float f)            { return (float)cos(f); }
float sinf   (float f)            { return (float)sin(f); }
float atan2f (float f1, float f2) { return (float)atan2(f1, f2); }
float powf   (float f1, float f2) { return (float)pow(f1, f2); }
float logf   (float f)            { return (float)log(f); }
float ceilf  (float f)            { return (float)ceil(f); }
