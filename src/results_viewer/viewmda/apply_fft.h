#ifndef apply_fft_H
#define apply_fft_H
#include "mda.h"

void apply_1d_fft(Mda &X,bool do_inverse,int dim1);
void apply_2d_fft(Mda &X,bool do_inverse,int dim1,int dim2);
void apply_3d_fft(Mda &X,bool do_inverse,int dim1,int dim2,int dim3);

#endif
