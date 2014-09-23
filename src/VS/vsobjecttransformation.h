#ifndef vsobjecttransformation_H
#define vsobjecttransformation_H

#ifndef PI
	#define PI 3.14159265358979323846
#endif

#include <QString>
typedef double double44[4][4];

class VSObjectTransformation {
public:
	VSObjectTransformation();
	VSObjectTransformation(const VSObjectTransformation &X);
	virtual ~VSObjectTransformation();
	void applyToIspace(double &x,double &y,double &z) const;
	void applyToKspace(double &kx,double &ky,double &kz,double &phase_offset,double &scale_factor) const;
	
	void xyRotate(double angle /*deg*/);
	void xzRotate(double angle /*deg*/);
	void yzRotate(double angle /*deg*/);
	void translate(double dx,double dy,double dz);
	void scale(double sx,double sy,double sz);
	
	void operator=(const VSObjectTransformation &X);	
	void leftMultiply(const VSObjectTransformation &X);
	void invert();
	
	QString toText() const;
	
private:
	double44 m_data;
	
	void left_multiply_by(const double44 &X);
	void set_data(const double44 &X);
	void copy_from(const VSObjectTransformation &X);
};

#endif
