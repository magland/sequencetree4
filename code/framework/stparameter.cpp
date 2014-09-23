
#include "stparameter.h"

Vector3::Vector3(double x_in,double y_in,double z_in) {
	m_x=x_in; m_y=y_in; m_z=z_in;
}
Vector3::Vector3(const Vector3 &X) {
	m_x=X.x(); m_y=X.y(); m_z=X.z();
}
void Vector3::operator=(const Vector3 &X) {
	m_x=X.x(); m_y=X.y(); m_z=X.z();
}
Vector3 Vector3::operator+(const Vector3 &X) const {
	Vector3 ret(m_x+X.x(),m_y+X.y(),m_z+X.z());
	return ret;
}
Vector3 Vector3::operator-(const Vector3 &X) const {
	Vector3 ret(m_x-X.x(),m_y-X.y(),m_z-X.z());
	return ret;
}
Vector3 Vector3::operator*(const double val) const {
	Vector3 ret(m_x*val,m_y*val,m_z*val);
	return ret;
}
Vector3 Vector3::operator/(const double val) const {
	Vector3 ret(m_x/val,m_y/val,m_z/val);
	return ret;
}
bool Vector3::operator==(const Vector3 &X) const {
	if (m_x!=X.x()) return false;
	if (m_y!=X.y()) return false;
	if (m_z!=X.z()) return false;
	return true;
}
bool Vector3::operator!=(const Vector3 &X) const {
	if ((*this)==X) return false;
	return true;
}




STParameter::STParameter() {
	setObjectType("STParameter");
	m_has_been_set=true;
}
bool STParameter::isParameter() {
	return true;
}


STInteger::STInteger(long val) {
	setObjectType("STInteger");
	m_data=val;
}
void STInteger::operator=(long val) {
	setHasBeenSet(true);
	if (m_data!=val) {
		m_data=val;
		setModified(true);
	}
}

STReal::STReal(double val) {
	setObjectType("STReal");
	m_data=val;
}
void STReal::operator=(double val) {
	setHasBeenSet(true);
	if (m_data!=val) {
		m_data=val;
		setModified(true);
	}
}

STVector3::STVector3(Vector3 val) {
	setObjectType("STVector3");
	m_data=val;
}
void STVector3::operator=(Vector3 val) {
	setHasBeenSet(true);
	if (m_data!=val) {
		m_data=val;
		setModified(true);
	}
}

STString::STString(SString val) {
	setObjectType("STString");
	m_data=val;
}
void STString::operator=(SString val) {
	setHasBeenSet(true);
	if (m_data!=val) {
		m_data=val;
		setModified(true);
	}
}

STIterator::STIterator(double min_in,double step_in,double max_in) {
	setObjectType("STIterator");
	m_data_min=min_in;
	m_data_step=step_in;
	m_data_max=max_in;
	m_value=0;
	m_check_index=0;
}
double STIterator::minimum() const {return m_data_min;}
double STIterator::maximum() const {return m_data_max;}
double STIterator::step() const {return m_data_step;}
long STIterator::numSteps() const {
	if (m_data_step==0) return 0;
	if (m_data_step>0) {
		if (m_data_max<m_data_min) return 0;
		double hold=(m_data_max-m_data_min)/m_data_step+0.0001;
		return ((long)hold)+1;
	}
	else {
		if (m_data_max>m_data_min) return 0;
		double hold=(m_data_max-m_data_min)/m_data_step+0.0001;
		return ((long)hold)+1;
	}
}
long STIterator::stepNumber() const {
	if (m_data_step==0) return 0;
	return (long)((m_value-m_data_min)/m_data_step);
}
	
void STIterator::initialize() {
	m_value=m_data_min;
	m_check_index=0;
}
bool STIterator::increment() {
	m_value+=m_data_step;
	if (m_data_step==0) return false;
	if (m_data_step>0) {
		double hold=(m_data_max-m_value)/m_data_step+0.0001;
		return (hold>=0);
	}
	else {
		double hold=(m_data_max-m_value)/m_data_step+0.0001;
		return (hold>=0);
	}
}
bool STIterator::incrementCheck() {
	if (m_check_values.count()==0) return incrementToEnd();
	m_check_index++;
	if (m_check_index>=m_check_values.count()) return false;
	m_value=m_check_values[m_check_index];
	return true;
}
bool STIterator::incrementToEnd() {
	if (m_value==m_data_max) return false;
	else m_value=m_data_max;
	return true;
}

double STIterator::value() {
	return m_value;
}

void STIterator::set(double min,double step,double max) {
	setHasBeenSet(true);
	if ((m_data_min!=min)||(m_data_step!=step)||(m_data_max!=max)) {
		m_data_min=min;
		m_data_step=step;
		m_data_max=max;
		setModified(true);
	}
}

