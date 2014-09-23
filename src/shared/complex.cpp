#include "complex.h"
#include <math.h>

real normsqr(const AbstractComplex &X) {
	return X.re()*X.re()+X.im()*X.im();
}
Complex operator*(const AbstractComplex &X1, const AbstractComplex &X2) {
	return Complex(X1.re()*X2.re()-X1.im()*X2.im(),X1.re()*X2.im()+X1.im()*X2.re());
}
Complex operator/(const AbstractComplex &X1, const AbstractComplex &X2) {
	real ns=normsqr(X2);
	if (ns==0) return Complex(0,0);
	else return X1*conj(X2)/ns;
}
Complex operator+(const AbstractComplex &X1, const AbstractComplex &X2) {
	return Complex(X1.re()+X2.re(),X1.im()+X2.im());
}
Complex operator-(const AbstractComplex &X1, const AbstractComplex &X2)  {
	return Complex(X1.re()-X2.re(),X1.im()-X2.im());
}
Complex operator*(const AbstractComplex &X1, real X2) {
	return Complex(X1.re()*X2,X1.im()*X2);
}
Complex operator/(const AbstractComplex &X1, real X2) {
	if (X2==0) return Complex(0,0);
	return Complex(X1.re()/X2,X1.im()/X2);
}
Complex operator+(const AbstractComplex &X1, real X2) {
	return Complex(X1.re()+X2,X1.im());
}
Complex operator-(const AbstractComplex &X1, real X2) {
	return Complex(X1.re()-X2,X1.im());
}
Complex operator*(real X1, const AbstractComplex &X2) {
	return X2*X1;
}
Complex operator/(real X1, const AbstractComplex &X2) {
	return Complex(X1,0)/X2;
}
Complex operator+(real X1, const AbstractComplex &X2) {
	return Complex(X1+X2.re(),X2.im());
}
Complex operator-(real X1, const AbstractComplex &X2) {
	return Complex(X1-X2.re(),-X2.im());
}
bool operator==(const AbstractComplex &X1, const AbstractComplex &X2) {
	return ((X1.re()==X2.re())&&(X1.im()==X2.im()));
}
bool operator==(const AbstractComplex &X1, real X2) {
	return ((X1.re()==X2)&&(X1.im()==0));
}
bool operator==(real X1, const AbstractComplex &X2) {
	return (X2==X1);
}
real abs(const AbstractComplex &X) {
	return (real)sqrt(normsqr(X));
}
Complex conj(const AbstractComplex &X) {
	return Complex(X.re(),-X.im());
}
Complex exp(const AbstractComplex &X) {
	real r=(real)exp(X.re());
	real theta=X.im();
	return Complex(r*(real)cos(theta),r*(real)sin(theta));
}
Complex log(const AbstractComplex &X) {
	real theta=(real)atan2(X.im(),X.re());
	real r=(real)log(normsqr(X))/2;
	return Complex(r,theta);
}
Complex sqrt(const AbstractComplex &X) {
	if (X==0) return Complex(0,0);
	return exp(log(X)*0.5);
}
Complex power(const AbstractComplex &X1, const AbstractComplex &X2) {
	if (X1==0) return Complex(0,0);
	return exp(log(X1)*X2);
}

Complex CHAINLINK_CORE_INTERFACE_EXPORT operator-(const AbstractComplex &X) {
	return X*(-1);
}

