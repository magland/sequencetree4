#include "vsobjecttransformation.h"
#include <math.h>

void set_identity(double44 &X) {
	for (int j=0; j<4; j++)
	for (int k=0; k<4; k++)
		X[j][k]=0;
	for (int j=0; j<4; j++)
		X[j][j]=1;
}
double det33(const double44 &X) {
	return X[0][0]*X[1][1]*X[2][2]
		+X[0][1]*X[1][2]*X[2][0]
		+X[0][2]*X[1][0]*X[2][1]
		-X[0][2]*X[1][1]*X[2][0]
		-X[0][1]*X[1][0]*X[2][2]
		-X[0][0]*X[1][2]*X[2][1]; //determinant
}
VSObjectTransformation::VSObjectTransformation(const VSObjectTransformation &X) {
	copy_from(X);
}
VSObjectTransformation::VSObjectTransformation() 
{
	set_identity(m_data);
}


VSObjectTransformation::~VSObjectTransformation()
{
}

void VSObjectTransformation::applyToIspace(double &x,double &y,double &z) const {
	double x2=x; double y2=y; double z2=z;
	x=m_data[0][0]*x2+m_data[0][1]*y2+m_data[0][2]*z2+m_data[0][3];
	y=m_data[1][0]*x2+m_data[1][1]*y2+m_data[1][2]*z2+m_data[1][3];
	z=m_data[2][0]*x2+m_data[2][1]*y2+m_data[2][2]*z2+m_data[2][3];
}
void VSObjectTransformation::applyToKspace(double &kx,double &ky,double &kz,double &phase_offset,double &scale_factor) const {
	scale_factor=det33(m_data);
	phase_offset=(m_data[0][3]*kx+m_data[1][3]*ky+m_data[2][3]*kz)*360; //perhaps multiply by -1?

	//multiply by transpose
	double kx2=kx; double ky2=ky; double kz2=kz;
	kx=m_data[0][0]*kx2+m_data[1][0]*ky2+m_data[2][0]*kz2;
	ky=m_data[0][1]*kx2+m_data[1][1]*ky2+m_data[2][1]*kz2;
	kz=m_data[0][2]*kx2+m_data[1][2]*ky2+m_data[2][2]*kz2;
}

void VSObjectTransformation::xyRotate(double angle /*deg*/) {
	double theta=angle*PI/180;
	double44 hold;
	set_identity(hold);
	hold[0][0]=cos(theta); hold[0][1]=-sin(theta);
	hold[1][0]=sin(theta); hold[1][1]=cos(theta);
	left_multiply_by(hold);
}
void VSObjectTransformation::xzRotate(double angle /*deg*/) {
	double theta=angle*PI/180;
	double44 hold;
	set_identity(hold);
	hold[0][0]=cos(theta); hold[0][2]=-sin(theta);
	hold[2][0]=sin(theta); hold[2][2]=cos(theta);
	left_multiply_by(hold);
}
void VSObjectTransformation::yzRotate(double angle /*deg*/) {
	double theta=angle*PI/180;
	double44 hold;
	set_identity(hold);
	hold[1][1]=cos(theta); hold[1][2]=-sin(theta);
	hold[2][1]=sin(theta); hold[2][2]=cos(theta);
	left_multiply_by(hold);
}
void VSObjectTransformation::translate(double dx,double dy,double dz) {
	double44 hold;
	set_identity(hold);
	hold[0][3]=dx; hold[1][3]=dy; hold[2][3]=dz;
	left_multiply_by(hold);
}
void VSObjectTransformation::scale(double sx,double sy,double sz) {
	double44 hold;
	set_identity(hold);
	hold[0][0]=sx; hold[1][1]=sy; hold[2][2]=sz;
	left_multiply_by(hold);
}

void VSObjectTransformation::left_multiply_by(const double44 &X) {
	double44 result;
	for (int j=0; j<4; j++) 
	for (int k=0; k<4; k++) {
		double val=0;
		for (int l=0; l<4; l++)
			val+=X[j][l]*m_data[l][k];
		result[j][k]=val;
	}
	set_data(result);
	
}

void VSObjectTransformation::set_data(const double44 &X) {
	for (int j=0; j<4; j++) 
	for (int k=0; k<4; k++) {
		m_data[j][k]=X[j][k];
	}
}
void VSObjectTransformation::copy_from(const VSObjectTransformation &X) {
	set_data(X.m_data);
}

void VSObjectTransformation::operator=(const VSObjectTransformation &X) {
	copy_from(X);
}

void VSObjectTransformation::leftMultiply(const VSObjectTransformation &X) {
	left_multiply_by(X.m_data);
}

void VSObjectTransformation::invert() {
	double44 result;
	set_identity(result);
	double d33=det33(m_data);
	result[0][0]=(m_data[1][1]*m_data[2][2]-m_data[2][1]*m_data[1][2])*(+1)/d33;
	result[1][0]=(m_data[1][0]*m_data[2][2]-m_data[2][0]*m_data[1][2])*(-1)/d33;
	result[2][0]=(m_data[1][0]*m_data[2][1]-m_data[2][0]*m_data[1][1])*(+1)/d33;
	result[0][1]=(m_data[0][1]*m_data[2][2]-m_data[2][1]*m_data[0][2])*(-1)/d33;
	result[1][1]=(m_data[0][0]*m_data[2][2]-m_data[2][0]*m_data[0][2])*(+1)/d33;
	result[2][1]=(m_data[0][0]*m_data[2][1]-m_data[2][0]*m_data[0][1])*(-1)/d33;
	result[0][2]=(m_data[0][1]*m_data[1][2]-m_data[1][1]*m_data[0][2])*(+1)/d33;
	result[1][2]=(m_data[0][0]*m_data[1][2]-m_data[1][0]*m_data[0][2])*(-1)/d33;
	result[2][2]=(m_data[0][0]*m_data[1][1]-m_data[1][0]*m_data[0][1])*(+1)/d33;
	
	result[0][3]=-result[0][0]*m_data[0][3]-result[0][1]*m_data[1][3]-result[0][2]*m_data[2][3];
	result[1][3]=-result[1][0]*m_data[0][3]-result[1][1]*m_data[1][3]-result[1][2]*m_data[2][3];
	result[2][3]=-result[2][0]*m_data[0][3]-result[2][1]*m_data[1][3]-result[2][2]*m_data[2][3];
	
	set_data(result);
}

QString VSObjectTransformation::toText() const {
	QString txt;
	for (int j=0; j<4; j++) {
		txt += QString("%1,%2,%3,%4\n").arg(m_data[j][0]).arg(m_data[j][1]).arg(m_data[j][2]).arg(m_data[j][3]);
	}
	return txt;
}

