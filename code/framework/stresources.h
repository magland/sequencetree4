#ifndef stresources_H
#define stresources_H

#include "sstring.h"
#include "slist.h"

class STResource {
public:
	STResource(SString name_in);
	virtual ~STResource();
	SString name();

	void setDoubleList(long N,const double *data);
	long doubleListCount();
	double getDoubleList(long index);
	
	void setDouble(double val);
	double getDouble();
	
	void writeToText(SString &txt);
	void readFromText(const SString &txt);
private:
	SString m_name;
	SList<double> m_double_list;
	double m_double;
};

class STResources {
public:
	STResources();
	virtual ~STResources();
	void addResource(STResource *);
	STResource *resource(SString name);
	long count();
	STResource *resource(long index);
	void writeToText(SString &txt);
	void readFromText(const SString &txt);
	void removeResource(long index);
private:
	SList<STResource *> m_resources;
};


#endif
