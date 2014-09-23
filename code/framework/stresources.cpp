#include "stresources.h"
#include <stdlib.h>

STResource::STResource(SString name_in) {
	m_name=name_in;
}
STResource::~STResource() {
}
SString STResource::name() {
	return m_name;
}

void STResource::setDoubleList(long N,const double *data) {
	m_double_list.clear();
	for (long j=0; j<N; j++) {
		m_double_list << data[j];
	}
}
long STResource::doubleListCount() {
	return m_double_list.count();
}
double STResource::getDoubleList(long index) {
	return m_double_list.value(index);
}

void STResource::setDouble(double val) {
	double hold[1];
	hold[0]=val;
	setDoubleList(1,hold);
}
double STResource::getDouble() {
	if (doubleListCount()<=0) return 0;
	return getDoubleList(0);
}

void STResource::writeToText(SString &txt) {
	if (doubleListCount()>0) {
		txt+=SString("DOUBLE_LIST\t")+SString::number(doubleListCount());
		for (long j=0; j<doubleListCount(); j++)
			txt+=SString("\t")+SString::number(getDoubleList(j));
		txt+="\n";
	}
}
void STResource::readFromText(const SString &txt) {
	SList<SString> lines=txt.split('\n');
	for (long j=0; j<lines.count(); j++) {
		SString line=lines[j];
		SList<SString> list=line.split('\t');
		SString hold=list.value(0);
		if (hold=="DOUBLE_LIST") {
			long N=list.value(1).toInt();
			if (N>0) {
				double *hold_data=(double *)malloc(sizeof(double)*N);
				for (long j=0; j<N; j++) {
					hold_data[j]=list.value(j+2).toDouble();
				}
				setDoubleList(N,hold_data);
				free(hold_data);
			}
		}
	}
}


STResources::STResources() {
}
STResources::~STResources() {
	for (long j=0; j<m_resources.count(); j++) {
		delete m_resources[j];
	}
	m_resources.clear();
}
void STResources::addResource(STResource *R) {
	for (long j=0; j<m_resources.count(); j++) {
		if (m_resources[j]->name()==R->name()) {
			delete m_resources[j];
			m_resources.removeAt(j);
			j--;
		}
	}
	m_resources << R;
}
STResource *STResources::resource(SString name) {
	for (long j=0; j<m_resources.count(); j++) {
		if (m_resources[j]->name()==name) 
			return m_resources[j];
	}
	return 0;
}
long STResources::count() {
	return m_resources.count();
}
STResource *STResources::resource(long index) {
	return m_resources[index];
}

void STResources::writeToText(SString &txt) {
	for (long j=0; j<m_resources.count(); j++) {
		txt+=SString("RESOURCE\t")+m_resources[j]->name()+"\n";
		m_resources[j]->writeToText(txt);
		txt+=SString("END RESOURCE\n");		
	}
}
void STResources::readFromText(const SString &txt) {
	SList<SString> lines=txt.split('\n');
	for (long j=0; j<lines.count(); j++) {
		SList<SString> list=lines[j].split('\t');
		if (list.value(0)=="RESOURCE") {
			SString resource_name=list.value(1);
			if (resource_name.count()>0) {
				long k=j+1;
				bool done=false;
				while ((!done)&&(k<lines.count())) {
					if (lines[k].mid(0,3)=="END") done=true;
					else k++;
				}
				if (k<lines.count()) {
					SString holdstr;
					for (long ind=j+1; ind<=k-1; ind++) {
						holdstr=holdstr+lines[ind]+"\n";						
					}
					STResource *new_resource=new STResource(resource_name);
					new_resource->readFromText(holdstr);
					addResource(new_resource);
				}
			}
		}
	}
}

void STResources::removeResource(long index) {
	delete m_resources[index];
	m_resources.removeAt(index);	
}

