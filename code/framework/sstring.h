
#ifndef sstring_H
#define sstring_H

//#include <QStringList>
//#include <QString>
//#define SString QString


#define USING_SSTRING
#include "slist.h"

class ST_EXPORT SString {
public:
	SString();
	SString(const SString &X);
	SString(const char *X);
	virtual ~SString();
	
	char *data() const {return m_data;}
	
	long count() const {return m_count;}
	void operator=(const SString &X);
	void operator=(const char *X);
	SString operator+(const SString &X) const;
	SString operator+(const char *X) const;
	void operator+=(const SString &X);
	void operator+=(const char *X);
	void remove(const char *substr);
	void remove(char c);
	bool operator==(const SString &X) const;
	bool operator==(const char *X) const;
	bool operator!=(const SString &X) const;
	bool operator!=(const char *X) const;
	char operator[](long index) const;
	
	void append(char c);
	void append(const char *str);
	void append(const SString &X);
	
	SList<SString> split(char c) const;
	SList<SString> split(const char *str) const;
	
	int toInt();
	double toDouble();
	static SString number(double val);
	
	long indexOf(SString substr);
	SString mid(long index,long ct=-1);
	
	void allocate(long size);
private:
	char *m_data;
	long m_allocated_size;
	long m_count;
	
	void initialize();	
	void set(const char *str,long length=0);
	
};

#endif

