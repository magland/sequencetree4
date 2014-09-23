#ifndef complex_H
#define complex_H

#include "chainlinkglobal.h"

typedef float real;
class AbstractComplex {
public:
	virtual real re() const {return 0;}
	virtual real im() const {return 0;}	
};

class CHAINLINK_CORE_INTERFACE_EXPORT Complex : public AbstractComplex {
public:
	Complex(real rp=0,real ip=0) {m_re=rp; m_im=ip;}
	Complex(const AbstractComplex &X) {m_re=X.re(); m_im=X.im();}
	virtual ~Complex() {};
	
	real re() const {return m_re;}
	real im() const {return m_im;}
	void setRe(real val) {m_re=val;}
	void setIm(real val) {m_im=val;}
	
	void operator=(const AbstractComplex &X) {setRe(X.re()); setIm(X.im());}
	void operator=(real X) {setRe(X); setIm(0);}
	
private:
	real m_re,m_im;
};

//operatations on complex numbers
/*! Norm squared*/
real CHAINLINK_CORE_INTERFACE_EXPORT normsqr(const AbstractComplex &X);
/*! Complex product.*/
Complex CHAINLINK_CORE_INTERFACE_EXPORT operator*(const AbstractComplex &X1, const AbstractComplex &X2);
/*! Complex quotient.*/
Complex CHAINLINK_CORE_INTERFACE_EXPORT operator/(const AbstractComplex &X1, const AbstractComplex &X2);
/*! Complex sum.*/
Complex CHAINLINK_CORE_INTERFACE_EXPORT operator+(const AbstractComplex &X1, const AbstractComplex &X2);
/*! Complex difference.*/
Complex CHAINLINK_CORE_INTERFACE_EXPORT operator-(const AbstractComplex &X1, const AbstractComplex &X2);
/*! Product of complex and real.*/
Complex CHAINLINK_CORE_INTERFACE_EXPORT operator*(const AbstractComplex &X1, real X2);
/*! Quotient of complex and real.*/
Complex CHAINLINK_CORE_INTERFACE_EXPORT operator/(const AbstractComplex &X1, real X2);
/*! Sum of complex and real.*/
Complex CHAINLINK_CORE_INTERFACE_EXPORT operator+(const AbstractComplex &X1, real X2);
/*! Difference of complex and real.*/
Complex CHAINLINK_CORE_INTERFACE_EXPORT operator-(const AbstractComplex &X1, real X2);
/*! Product of real and complex.*/
Complex CHAINLINK_CORE_INTERFACE_EXPORT operator*(real X1, const AbstractComplex &X2);
/*! Quotient of real and complex.*/
Complex CHAINLINK_CORE_INTERFACE_EXPORT operator/(real X1, const AbstractComplex &X2);
/*! Sum of real and complex.*/
Complex CHAINLINK_CORE_INTERFACE_EXPORT operator+(real X1, const AbstractComplex &X2);
/*! Difference of real and complex.*/
Complex CHAINLINK_CORE_INTERFACE_EXPORT operator-(real X1, const AbstractComplex &X2);
/*! Complex comparison.*/
bool CHAINLINK_CORE_INTERFACE_EXPORT operator==(const AbstractComplex &X1, const AbstractComplex &X2);
/*! Complex/real comparison.*/
bool CHAINLINK_CORE_INTERFACE_EXPORT operator==(const AbstractComplex &X1, real X2);
/*! Real/complex comparison.*/
bool CHAINLINK_CORE_INTERFACE_EXPORT operator==(real X1, const AbstractComplex &X2);
/*! Absolute value of a complex number*/
real CHAINLINK_CORE_INTERFACE_EXPORT abs(const AbstractComplex &X);
/*! Complex conjugate*/
Complex CHAINLINK_CORE_INTERFACE_EXPORT conj(const AbstractComplex &X);
/*! Complex exponential function*/
Complex CHAINLINK_CORE_INTERFACE_EXPORT exp(const AbstractComplex &X);
/*! Complex logarithmic function*/
Complex CHAINLINK_CORE_INTERFACE_EXPORT log(const AbstractComplex &X);
/*! Complex square root function*/
Complex CHAINLINK_CORE_INTERFACE_EXPORT sqrt(const AbstractComplex &X);
/*! Complex power*/
Complex CHAINLINK_CORE_INTERFACE_EXPORT power(const AbstractComplex &X1, const AbstractComplex &X2);
Complex CHAINLINK_CORE_INTERFACE_EXPORT operator-(const AbstractComplex &X);


#endif
