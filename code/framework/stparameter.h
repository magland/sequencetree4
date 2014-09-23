#ifndef stparameter_H
#define stparameter_H

#include "slist.h"
#include "stobject.h"
#include <math.h>

class ST_EXPORT Vector3 {
private:	
	double m_x,m_y,m_z;
public:
	Vector3(double x_in=0,double y_in=0,double z_in=0);
	Vector3(const Vector3 &X);
	virtual ~Vector3() {}
	double x() const {return m_x;}
	double y() const {return m_y;}
	double z() const {return m_z;}
	void setX(double val) {m_x=val;}
	void setY(double val) {m_y=val;}
	void setZ(double val) {m_z=val;}
	void operator=(const Vector3 &X);
	Vector3 operator+(const Vector3 &X) const;
	Vector3 operator-(const Vector3 &X) const;
	Vector3 operator*(const double val) const;
	Vector3 operator/(const double val) const;
	bool operator==(const Vector3 &X) const;
	bool operator!=(const Vector3 &X) const;
	double abs() const {return sqrt(m_x*m_x+m_y*m_y+m_z*m_z);}
};

class ST_EXPORT STParameter : public STObject {
public:
	STParameter();
	virtual ~STParameter() {}
	bool isParameter();
	
	virtual void setFromString(SString str) {}
	virtual SString toString() {return "";}
	
	bool hasBeenSet() {return m_has_been_set;}
	void setHasBeenSet(bool val) {m_has_been_set=val;}
private:
	bool m_has_been_set;
};

class STReal;

class ST_EXPORT STInteger : public STParameter {
public:
	STInteger(long val=0);
	virtual ~STInteger() {}
	operator long() {return m_data;}
	void operator=(long val);
	void operator=(STInteger val) {(*this)=val.m_data;}
	void setFromString(SString val) {setHasBeenSet(true); m_data=val.toInt();}
	SString toString() {return SString::number(m_data);}
private:
	long m_data;
};

class ST_EXPORT STReal : public STParameter {
public:
	STReal(double val=0);
	virtual ~STReal() {}
	operator double() {return m_data;}
	void operator=(double val);
	void operator=(STReal val) {(*this)=val.m_data;}
	void setFromString(SString val) {setHasBeenSet(true); m_data=val.toDouble();}
	SString toString() {return SString::number(m_data);}
private:
	double m_data;
};

class ST_EXPORT STVector3 : public STParameter {
public:
	STVector3(Vector3 val=Vector3(0,0,0));
	virtual ~STVector3() {}
	operator Vector3() {return m_data;}
	void operator=(Vector3 val);
	void operator=(STVector3 val) {(*this)=val.m_data;}
	double x() const {return m_data.x();}
	double y() const {return m_data.y();}
	double z() const {return m_data.z();}
	void setX(double val) {m_data.setX(val);}
	void setY(double val) {m_data.setY(val);}
	void setZ(double val) {m_data.setZ(val);}
	Vector3 operator+(const Vector3 &X) {return ((Vector3)(*this))+X;}
	Vector3 operator-(const Vector3 &X) {return ((Vector3)(*this))-X;}
	Vector3 operator*(double val) {return ((Vector3)(*this))*val;}
	Vector3 operator/(double val) {return ((Vector3)(*this))/val;}
	
	double abs() const {return m_data.abs();}
	void setFromString(SString val) {
		setHasBeenSet(true); 
		val.remove("(");
		val.remove(")");
		val.remove(" ");
		SList<SString> list;
		list=val.split(',');
		//QStringList list=val.split(',');
		m_data=Vector3(
			list.value(0).toDouble(),
			list.value(1).toDouble(),
			list.value(2).toDouble()
		);
	}
	SString toString() {
		SString ret="(";
		ret=ret+SString::number(m_data.x())+",";
		ret=ret+SString::number(m_data.y())+",";
		ret=ret+SString::number(m_data.z())+")";
		return ret;
	}
private:
	Vector3 m_data;
};

class ST_EXPORT STString : public STParameter {
public:
	STString(SString val="");
	virtual ~STString() {}
	operator SString() {return m_data;}
	void operator=(SString val);
	void operator=(STString val) {(*this)=val.m_data;}
	void setFromString(SString val) {setHasBeenSet(true); m_data=val;}
	SString toString() {return m_data;}
private:
	SString m_data;
};

class ST_EXPORT STIterator : public STParameter {
public:
	STIterator(double min_in=0,double step_in=1,double max_in=0);
	virtual ~STIterator() {}
	double minimum() const;
	double step() const;
	double maximum() const;
	long numSteps() const;
	long stepNumber() const;
	
	void initialize();
	bool increment();
	bool incrementToEnd();
	bool incrementCheck();
	double value();
	
	void set(double min,double step,double max);
	
	operator double() {return m_value;}
	void operator=(double val) {m_value=val;}
	
	void setFromString(SString val) {
		setHasBeenSet(true); 
		m_check_values.clear();
		int checkindex=val.indexOf("check:");
		if (checkindex>=0) {
			SString checkstr=val.mid(checkindex+6);
			checkstr.remove(" ");
			SList<SString> checklist=checkstr.split(';');
			for (int j=0; j<checklist.count(); j++)
				m_check_values << checklist[j].toDouble();
			val=val.mid(0,checkindex);
		}
		val.remove("(");
		val.remove(")");
		val.remove(" ");
		SList<SString> list=val.split(':');
		//QStringList list=val.split(':');
		set(
			list.value(0).toDouble(),
			list.value(1).toDouble(),
			list.value(2).toDouble()
		);
	}
	SString toString() {
		SString ret="";
		ret=ret+SString::number(m_data_min)+":";
		ret=ret+SString::number(m_data_step)+":";
		ret=ret+SString::number(m_data_max);
		if (m_check_values.count()>0) {
			ret+="   check: ";
			for (int j=0; j<m_check_values.count(); j++) {
				if (j>0) ret+=";";
				ret+=SString::number(m_check_values[j]);
			}
			
		}
		return ret;
	}
private:
	double m_data_min;
	double m_data_max;
	double m_data_step;
	double m_value;
	int m_check_index;
	SList<double> m_check_values;
};



#endif
