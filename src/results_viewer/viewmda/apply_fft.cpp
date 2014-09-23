#include "apply_fft.h"
#include "fftw_library.h"
#include <QDebug>

void apply_1d_fft(Mda &X,bool do_inverse,int dim1) {
	internal_ifftshift(X);
	internal_fftw(X,dim1+1,do_inverse);
	internal_fftshift(X);
}
void apply_2d_fft(Mda &X,bool do_inverse,int dim1,int dim2) {
	internal_ifftshift(X);
	internal_fftw(X,dim1+1,do_inverse);
	internal_fftw(X,dim2+1,do_inverse);
	internal_fftshift(X);
}
void apply_3d_fft(Mda &X,bool do_inverse,int dim1,int dim2,int dim3) {
	internal_ifftshift(X);
	internal_fftw(X,dim1+1,do_inverse);
	internal_fftw(X,dim2+1,do_inverse);
	internal_fftw(X,dim3+1,do_inverse);
	internal_fftshift(X);
}

