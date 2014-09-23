/* BEGIN EXCLUDE */
#include "stsequence.h"
#include <stdio.h>
/* END EXCLUDE */

STSequence::STSequence() {
	setName("Root");
	ST_CLASS(STSequence,STLoop)
	setSequence(this);
	
	ST_PARAMETER(STVector3, FOV, (200,200,50),mm)
	ST_PARAMETER(STReal, maxamp, 20,uT/mm)
	ST_PARAMETER(STReal, ramprate, 0.1,[uT/mm]/us)
	ST_PARAMETER(STReal, gamma, 42.5764,Hz/uT)
	ST_PARAMETER(STVector3, FOV_shift, (0,0,0),mm)
	ST_PARAMETER(STVector3, phase_shift, 0, degrees)
	ST_PARAMETER(STVector3, FOV_shift_offset, (0,0,0),mm; used internally)
	
	m_current_loop_index=0;
	m_check_mode=false;
	m_statistics_mode=false;
	m_check_but_dont_run_mode=false;
}
Vector3 STSequence::kspace2moment(Vector3 kspace) {
	Vector3 ret(
		kspace.x()/(FOV.x()*(double)gamma)*1.0E6,
		kspace.y()/(FOV.y()*(double)gamma)*1.0E6,
		kspace.z()/(FOV.z()*(double)gamma)*1.0E6
	);
	return ret;
}

STLoop *STSequence::loop(int index) {
	if (!child(index)->isLoop()) return 0;
	return (STLoop *)child(index);
}

bool STSequence::loopInitialize() {
	for (int j=0; j<childCount(); j++) {
		if (!loop(j)) return false;
		if (!loop(j)->loopInitialize()) return false;
	}
	m_current_loop_index=0;
	return true;
}
bool STSequence::loopStep() {
	if (m_current_loop_index>=childCount()) return false;
	if (!loop(m_current_loop_index)) return false;
	if (!loop(m_current_loop_index)->loopStep()) {
		m_current_loop_index++;
		if (m_current_loop_index>=childCount()) return false;
	}
	return true;
}
bool STSequence::loopRun() {
	if (m_current_loop_index>=childCount()) return false;
	if (!loop(m_current_loop_index)) return false;
	return loop(m_current_loop_index)->loopRun();
}

bool STSequence::initialize() {
	resetHasBeenSet();
	setAllModified();
	return STLoop::initialize();
}

bool STSequence::run() {
	if (!scanner()) return false;
	if (!m_check_but_dont_run_mode) {
		scanner()->beginSequence();
	}
	setADCIndices();
	bool ret=STLoop::run();
	if (!m_check_but_dont_run_mode) {
		scanner()->endSequence();
	}
	return ret;
}

bool STSequence::check() {
	bool hold_check_mode=m_check_mode;
	m_check_mode=true;
	bool ret=run();
	m_check_mode=hold_check_mode;
	return ret;
}

bool STSequence::checkButDontRun() {
	bool hold_check_but_dont_run_mode=m_check_but_dont_run_mode;
	bool ret=check();
	m_check_but_dont_run_mode=hold_check_but_dont_run_mode;
	return ret;
}

bool STSequence::computeStatistics() {
	bool hold_statistics_mode=m_statistics_mode;
	m_statistics_mode=true;
	m_stat_block_count=0;
	m_stat_duration=0;
	m_stat_SAR=0;
	bool ret=run();
	m_statistics_mode=hold_statistics_mode;
	return ret;
}

SString read_text_file_b(SString fname) {
	#ifdef USING_SSTRING
	FILE *inf=fopen(fname.data(),"r");
	#else
	FILE *inf=fopen(fname.toAscii().data(),"r");
	#endif
	if (!inf) return SString("");
	SString ret;
	char str[5001];
	while (!feof(inf)) {
		if (fgets(str,5000,inf)) {
			ret.append(str);
		}
	}
	fclose(inf);
	return ret;
}
bool write_text_file_b(const SString & fname,const SString &txt) {
	
	#ifdef USING_SSTRING
	FILE *outf=fopen(fname.data(),"w");
	if (!outf) return false;
	fprintf(outf,"%s",txt.data());
	fclose(outf);
	
	#else
	FILE *outf=fopen(fname.toAscii().data(),"w");
	if (!outf) return false;
	fprintf(outf,"%s",txt.toAscii().data());
	fclose(outf);
	#endif
	
	return true;
}
void STSequence::loadParametersFromFile(SString file_name) {
	SString txt=read_text_file_b(file_name);
	loadParametersFromText(txt);
}
void STSequence::loadParametersFromText(const SString &txt) {
	SList<SString> list=txt.split('\n');
	STNode *ptr=this;
	int active_level=0;
	for (long j=0; j<list.count(); j++) {
		SString line=list[j];
		SList<SString> vals=line.split('\t');
		if (vals.value(0)=="CHILD") {
			if (active_level>0) active_level++;
			else {
				if (ptr->child(vals.value(1))) {
					ptr=ptr->child(vals.value(1));
				}
				else {
					active_level++;
				}
			}
		}
		else if (vals.value(0)=="ENDCHILD") {
			if (active_level>0) active_level--;
			else ptr=ptr->parent();
		}
		else if (vals.value(0)=="PARAM") {
			if (active_level==0) {
				ptr->setParameterFromString(vals.value(1),vals.value(2));
			}
		}
	}
}
void STSequence::writeParametersToFile(SString file_name) {
	SString txt;
	write_parameters_to_text(txt,this);
	write_text_file_b(file_name,txt);
}
void STSequence::write_parameters_to_text(SString &txt,STNode *ptr) {
	if (!ptr) ptr=this;
	if (ptr->treeIndex()<0) return;
	{for (int j=0; j<ptr->parameterCount(); j++) {
		STParameter *param=ptr->parameter(j);
		SString passive_txt="active";
		if (param->hasBeenSet()) passive_txt="passive";
		SString str=SString("PARAM\t")+param->name()+"\t"+param->toString()+"\t"+passive_txt;
		txt+=str+"\n";
	}}
	{for (int j=0; j<ptr->childCount(); j++) {
		STNode *ch=ptr->child(j);
		SString str=SString("CHILD\t")+ch->name()+"\t"+ch->objectType();
		txt+=str+"\n";
		write_parameters_to_text(txt,ch);
		txt+=SString("ENDCHILD\n");
	}}
}

double STSequence::phaseAdjustment() {
	if (!scanner()) return 0;
	double ret=(scanner()->totalGradientMomentX()*getFOVShiftX()
			+scanner()->totalGradientMomentY()*getFOVShiftY()
			+scanner()->totalGradientMomentZ()*getFOVShiftZ())*gamma/1.0E6*360; 
	ret+=phase_shift;
	return normalizePhase(ret);
}

STNode *STSequence::findNodeFromString(SString str) {
	SList<SString> list=str.split("->");
	if (list.isEmpty()) return 0;
	if (list[0]!="Root") return 0;
	int index=0;
	STNode *ptr=this;
	while ((ptr)&&(index+1<list.count())) {
		ptr=ptr->child(list[index+1]);
		index++;
	}
	return ptr;
}

STParameter *STSequence::findParameterFromString(SString str) {
	int ind=str.count()-1;
	while ((ind>=0)&&(str[ind]!='-'))
		ind--;
	if (ind-1<0) return 0;
	STNode *N=findNodeFromString(str.mid(0,ind));
	if (!N) return 0;
	SString pname=str.mid(ind+2);
	for (int k=0; k<N->parameterCount(); k++)
		if (N->parameter(k)->name()==pname)
			return N->parameter(k);
	return 0;
}

STResources *STSequence::resources() {
	return &m_resources;
}
STResource *STSequence::resource(SString name) {
	return m_resources.resource(name);
}

void STSequence::addGlobalParameterConnection(SString name,SString connection) {
	for (int j=0; j<m_global_parameters.count(); j++) 
		if (m_global_parameters[j].name==name) {
			m_global_parameters[j].connections.append(connection);
			return;
		}
	STSequenceGlobalParameter P;
	P.name=name;
	P.connections.append(connection);
	m_global_parameters.append(P);
}
double STSequence::globalParameter(SString name) {
	for (int j=0; j<m_global_parameters.count(); j++) 
		if (m_global_parameters[j].name==name) {
			if (m_global_parameters[j].connections.count()>0) {
				STParameter *P=findParameterFromString(m_global_parameters[j].connections[0]);
				if (P) return P->toString().toDouble();
			}
		}
	return 0;
}
void STSequence::setGlobalParameter(SString name,double val) {
	for (int j=0; j<m_global_parameters.count(); j++) 
		if (m_global_parameters[j].name==name) {
			for (int k=0; k<m_global_parameters[j].connections.count(); k++) {
				STParameter *P=findParameterFromString(m_global_parameters[j].connections[k]);
				if (P) P->setFromString(SString::number(val));
			}
		}
}

