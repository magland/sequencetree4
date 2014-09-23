
#include "sstring.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "slist.h"


#ifdef USING_SSTRING

#define SSTRING_INITIAL_SIZE 30
#define SSTRING_INCREMENT_1 1000
#define SSTRING_INCREMENT_2 10000

SString::SString() 
{
	initialize();
}
void SString::initialize() {
	m_data=0;
	m_count=0;
	m_allocated_size=0;
	allocate(SSTRING_INITIAL_SIZE);
}

SString::~SString()
{
	if (m_data) free(m_data);
}

SString::SString(const SString &X) {
	initialize();
	set(X.m_data,X.count());
}

SString::SString(const char *X) {
	initialize();
	set(X);
}

void SString::allocate(long size) {
	if (size<m_allocated_size) return;
	char *hold_data=m_data;
	m_data=(char *)malloc(sizeof(char)*(size+1));
	m_allocated_size=size;
	if (m_count>0) memcpy(m_data,hold_data,m_count);
	m_data[m_count]=0;
	if (hold_data) free(hold_data);
}

void SString::set(const char *str,long length) {
	if (!length) length=strlen(str);
	if (length>=m_allocated_size) {
		if (length<SSTRING_INCREMENT_1)
			allocate(length+SSTRING_INCREMENT_1);
		else
			allocate(length+SSTRING_INCREMENT_2);
	}
	if (length>0) memcpy(m_data,str,length);
	m_data[length]=0;
	m_count=length;
}

void SString::operator=(const SString &X) {
	set(X.data(),X.count());
}

void SString::operator=(const char *X) {
	set(X);
}

SString SString::operator+(const char *X) const {
	return (*this)+SString(X);
}

SString SString::operator+(const SString &X) const {
	SString ret;
	ret.allocate(count()+X.count()+SSTRING_INITIAL_SIZE);
	if (count()>0) memcpy(ret.m_data,m_data,count());
	if (X.count()>0) memcpy(&(ret.m_data)[count()],X.m_data,X.count());
	ret.m_data[count()+X.count()]=0;
	ret.m_count=count()+X.count();
	return ret;
}

void SString::operator+=(const char *X) {
	SString hold=X;
	(*this)+=hold;
}

void SString::operator+=(const SString &X) {
	allocate(count()+X.count()+SSTRING_INITIAL_SIZE);
	if (X.count()>0) memcpy(&m_data[count()],X.data(),X.count());
	m_data[count()+X.count()]=0;
	m_count=count()+X.count();
}

void SString::remove(const char *substr) {
	SString hold;
	long len=strlen(substr);
	for (long j=0; j<count(); j++) {
		if (strncmp(&m_data[j],substr,len)==0) {
			j+=len-1;
		}
		else hold.append(m_data[j]);
	}
	(*this)=hold;
}

void SString::remove(char c) {
	char hold[2]; hold[0]=c; hold[1]=0;
	remove(hold);
}

void SString::append(char c) {
	char hold[2]; hold[0]=c; hold[1]=0;
	append(hold);
}
void SString::append(const char *str) {
	(*this)+=str;
}
void SString::append(const SString &X) {
	(*this)+=X;
}

SList<SString> SString::split(char c) const {
	SList<SString> ret;
	SString hold;
	for (long j=0; j<count(); j++) {
		if (m_data[j]==c) {
			ret.append(hold);
			hold="";
		}
		else {
			hold.append(m_data[j]);
		}
	}
	ret << hold;
	return ret;
}

SList<SString> SString::split(const char *str) const {
	SList<SString> ret;
	if (strlen(str)==0) return ret;
	SString hold;
	for (long j=0; j<count(); j++) {
		if (strncmp(&m_data[j],str,strlen(str))==0) {
			ret.append(hold);
			hold="";
			j+=strlen(str)-1;
		}
		else {
			hold.append(m_data[j]);
		}
	}
	ret << hold;
	return ret;
}

int SString::toInt() {
	return atoi(m_data);
}
double SString::toDouble() {
	double ret=atof(m_data);
	return ret;
}

SString SString::number(double val) {
	char ret[50];
	sprintf(ret,"%g",val);
	return SString(ret);
}

bool SString::operator==(const SString &X) const {
	return (strcmp(m_data,X.m_data)==0);
}
bool SString::operator==(const char *X) const {
	return (strcmp(m_data,X)==0);
}
bool SString::operator!=(const SString &X) const {
	return (!((*this)==X));
}
bool SString::operator!=(const char *X) const {
	return (!((*this)==X));
}
char SString::operator[](long index) const {
	return m_data[index];
}

long SString::indexOf(SString substr) {
	long N=count();
	long M=substr.count();
	for (long j=0; j<N; j++)
		if (strncmp(&m_data[j],substr.data(),M)==0)
			return j;
	return -1;
}

SString SString::mid(long index,long ct) {
	SString ret;
	if (ct<0) ct=count()-index;
	for (long j=0; j<ct; j++) {
		if (index+j<count())
			ret.append(m_data[index+j]);
	}
	return ret;
}

#endif




