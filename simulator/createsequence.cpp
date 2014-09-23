#include "createsequence.h"
#include "foundationclasses.h"
#include "customclasses.h"
#include "loadparametersfile.h"
#include <string.h>

STSequence *createSequence(char *parameter_fname) {
	STRoot *Root=new STRoot;
	if (parameter_fname) {
		Root->loadParametersFromFile(parameter_fname);
	}
	else {
		SString parameter_txt;
		loadParametersFile(parameter_txt);
		Root->loadParametersFromText(parameter_txt);
	}
	SString resource_text;
	loadResourceText(resource_text);
	Root->resources()->readFromText(resource_text);
	Root->addGlobalParameterConnection("TR","Root->ZTELoop->Block->TR");
	Root->addGlobalParameterConnection("TR","Root->PETRALoop->Block->TR");
	Root->addGlobalParameterConnection("TR","Root->PrepLoop->Block->TR");
	Root->addGlobalParameterConnection("FOVx","Root->FOVx");
	Root->addGlobalParameterConnection("FOVy","Root->FOVy");
	Root->addGlobalParameterConnection("flip_angle","Root->ZTELoop->Block->Acquire->RF->flip_angle");
	Root->addGlobalParameterConnection("flip_angle","Root->PETRALoop->Block->Acquire->RF->flip_angle");
	Root->addGlobalParameterConnection("flip_angle","Root->PrepLoop->Block->Acquire->RF->flip_angle");
	Root->addGlobalParameterConnection("N","Root->PETRALoop->Block->Acquire->Readout->N");
	Root->addGlobalParameterConnection("N","Root->ZTELoop->Block->Acquire->Readout->N");
	Root->addGlobalParameterConnection("N","Root->PrepLoop->Block->Acquire->Readout->N");
	Root->addGlobalParameterConnection("num_projections","Root->ZTELoop->num_projections");
	Root->addGlobalParameterConnection("pulse_duration","Root->ZTELoop->Block->Acquire->RF->pulse_duration");
	Root->addGlobalParameterConnection("pulse_duration","Root->PETRALoop->Block->Acquire->RF->pulse_duration");
	Root->addGlobalParameterConnection("pulse_duration","Root->PrepLoop->Block->Acquire->RF->pulse_duration");
	Root->addGlobalParameterConnection("readout_offset","Root->ZTELoop->Block->Acquire->readout_offset");
	Root->addGlobalParameterConnection("readout_offset","Root->PETRALoop->Block->Acquire->readout_offset");
	Root->addGlobalParameterConnection("readout_offset","Root->PrepLoop->Block->Acquire->readout_offset");
	Root->addGlobalParameterConnection("RF_offset","Root->ZTELoop->Block->Acquire->RF_offset");
	Root->addGlobalParameterConnection("RF_offset","Root->PETRALoop->Block->Acquire->RF_offset");
	Root->addGlobalParameterConnection("RF_offset","Root->PrepLoop->Block->Acquire->RF_offset");
	Root->addGlobalParameterConnection("RFS","Root->ZTELoop->RFS");
	Root->addGlobalParameterConnection("RFS","Root->PETRALoop->RFS");
	Root->addGlobalParameterConnection("RFS","Root->PrepLoop->RFS");
	Root->addGlobalParameterConnection("Radius","Root->PETRALoop->Radius");
	Root->addGlobalParameterConnection("Radius","Root->ZTELoop->Radius");
	Root->addGlobalParameterConnection("Radius","Root->PrepLoop->Radius");
	Root->addGlobalParameterConnection("dwell_time","Root->ZTELoop->Block->Acquire->Readout->dwell_time");
	Root->addGlobalParameterConnection("dwell_time","Root->PETRALoop->Block->Acquire->Readout->dwell_time");
	Root->addGlobalParameterConnection("dwell_time","Root->PrepLoop->Block->Acquire->Readout->dwell_time");
	Root->addGlobalParameterConnection("num_prep","Root->PrepLoop->num_projections");
	Root->addGlobalParameterConnection("num_averages","Root->ZTELoop->num_averages");
	Root->addGlobalParameterConnection("num_averages","Root->PETRALoop->num_averages");
	return Root;
}

void getSequenceName(char *ret) {
strcpy(ret,"sequence_name");
}

bool setGlobalParameter(STSequence *Seq,SString pname,SString pval) {
	STParameter *P;
	bool ret=true;
	if (pname=="TR") {
		P=Seq->findParameterFromString("Root->ZTELoop->Block->TR");
		if (P) P->setFromString(pval);
		else ret=false;
		P=Seq->findParameterFromString("Root->PETRALoop->Block->TR");
		if (P) P->setFromString(pval);
		else ret=false;
		P=Seq->findParameterFromString("Root->PrepLoop->Block->TR");
		if (P) P->setFromString(pval);
		else ret=false;
	}
	else if (pname=="FOVx") {
		P=Seq->findParameterFromString("Root->FOVx");
		if (P) P->setFromString(pval);
		else ret=false;
	}
	else if (pname=="FOVy") {
		P=Seq->findParameterFromString("Root->FOVy");
		if (P) P->setFromString(pval);
		else ret=false;
	}
	else if (pname=="flip_angle") {
		P=Seq->findParameterFromString("Root->ZTELoop->Block->Acquire->RF->flip_angle");
		if (P) P->setFromString(pval);
		else ret=false;
		P=Seq->findParameterFromString("Root->PETRALoop->Block->Acquire->RF->flip_angle");
		if (P) P->setFromString(pval);
		else ret=false;
		P=Seq->findParameterFromString("Root->PrepLoop->Block->Acquire->RF->flip_angle");
		if (P) P->setFromString(pval);
		else ret=false;
	}
	else if (pname=="N") {
		P=Seq->findParameterFromString("Root->PETRALoop->Block->Acquire->Readout->N");
		if (P) P->setFromString(pval);
		else ret=false;
		P=Seq->findParameterFromString("Root->ZTELoop->Block->Acquire->Readout->N");
		if (P) P->setFromString(pval);
		else ret=false;
		P=Seq->findParameterFromString("Root->PrepLoop->Block->Acquire->Readout->N");
		if (P) P->setFromString(pval);
		else ret=false;
	}
	else if (pname=="num_projections") {
		P=Seq->findParameterFromString("Root->ZTELoop->num_projections");
		if (P) P->setFromString(pval);
		else ret=false;
	}
	else if (pname=="pulse_duration") {
		P=Seq->findParameterFromString("Root->ZTELoop->Block->Acquire->RF->pulse_duration");
		if (P) P->setFromString(pval);
		else ret=false;
		P=Seq->findParameterFromString("Root->PETRALoop->Block->Acquire->RF->pulse_duration");
		if (P) P->setFromString(pval);
		else ret=false;
		P=Seq->findParameterFromString("Root->PrepLoop->Block->Acquire->RF->pulse_duration");
		if (P) P->setFromString(pval);
		else ret=false;
	}
	else if (pname=="readout_offset") {
		P=Seq->findParameterFromString("Root->ZTELoop->Block->Acquire->readout_offset");
		if (P) P->setFromString(pval);
		else ret=false;
		P=Seq->findParameterFromString("Root->PETRALoop->Block->Acquire->readout_offset");
		if (P) P->setFromString(pval);
		else ret=false;
		P=Seq->findParameterFromString("Root->PrepLoop->Block->Acquire->readout_offset");
		if (P) P->setFromString(pval);
		else ret=false;
	}
	else if (pname=="RF_offset") {
		P=Seq->findParameterFromString("Root->ZTELoop->Block->Acquire->RF_offset");
		if (P) P->setFromString(pval);
		else ret=false;
		P=Seq->findParameterFromString("Root->PETRALoop->Block->Acquire->RF_offset");
		if (P) P->setFromString(pval);
		else ret=false;
		P=Seq->findParameterFromString("Root->PrepLoop->Block->Acquire->RF_offset");
		if (P) P->setFromString(pval);
		else ret=false;
	}
	else if (pname=="RFS") {
		P=Seq->findParameterFromString("Root->ZTELoop->RFS");
		if (P) P->setFromString(pval);
		else ret=false;
		P=Seq->findParameterFromString("Root->PETRALoop->RFS");
		if (P) P->setFromString(pval);
		else ret=false;
		P=Seq->findParameterFromString("Root->PrepLoop->RFS");
		if (P) P->setFromString(pval);
		else ret=false;
	}
	else if (pname=="Radius") {
		P=Seq->findParameterFromString("Root->PETRALoop->Radius");
		if (P) P->setFromString(pval);
		else ret=false;
		P=Seq->findParameterFromString("Root->ZTELoop->Radius");
		if (P) P->setFromString(pval);
		else ret=false;
		P=Seq->findParameterFromString("Root->PrepLoop->Radius");
		if (P) P->setFromString(pval);
		else ret=false;
	}
	else if (pname=="dwell_time") {
		P=Seq->findParameterFromString("Root->ZTELoop->Block->Acquire->Readout->dwell_time");
		if (P) P->setFromString(pval);
		else ret=false;
		P=Seq->findParameterFromString("Root->PETRALoop->Block->Acquire->Readout->dwell_time");
		if (P) P->setFromString(pval);
		else ret=false;
		P=Seq->findParameterFromString("Root->PrepLoop->Block->Acquire->Readout->dwell_time");
		if (P) P->setFromString(pval);
		else ret=false;
	}
	else if (pname=="num_prep") {
		P=Seq->findParameterFromString("Root->PrepLoop->num_projections");
		if (P) P->setFromString(pval);
		else ret=false;
	}
	else if (pname=="num_averages") {
		P=Seq->findParameterFromString("Root->ZTELoop->num_averages");
		if (P) P->setFromString(pval);
		else ret=false;
		P=Seq->findParameterFromString("Root->PETRALoop->num_averages");
		if (P) P->setFromString(pval);
		else ret=false;
	}
	else ret=false;
	return ret;
}

int uiParameterCount() {
	return 14;
}
const char *uiParameterName(int index) {
	if (index==0) return "readout_offset";
	if (index==1) return "FOVx";
	if (index==2) return "FOVy";
	if (index==3) return "N";
	if (index==4) return "pulse_duration";
	if (index==5) return "dwell_time";
	if (index==6) return "num_projections";
	if (index==7) return "num_averages";
	if (index==8) return "RF_offset";
	if (index==9) return "Radius";
	if (index==10) return "num_prep";
	if (index==11) return "RFS";
	if (index==12) return "TR";
	if (index==13) return "flip_angle";
	return "";
}
const char *uiParameterLabel(int index) {
	if (index==0) return "Readout Offset (TE)";
	if (index==1) return "FOVx";
	if (index==2) return "FOVy";
	if (index==3) return "N";
	if (index==4) return "RF Pulse Duration";
	if (index==5) return "Dwell Time";
	if (index==6) return "Number of Projections";
	if (index==7) return "Number of Averages";
	if (index==8) return "RF Offset";
	if (index==9) return "PETRA Radius";
	if (index==10) return "Number of Prep Scans";
	if (index==11) return "RF Spoiling";
	if (index==12) return "TR";
	if (index==13) return "Flip Angle";
	return "";
}
const char *uiParameterType(int index) {
	if (index==0) return "UIDOUBLE";
	if (index==1) return "UIDOUBLE";
	if (index==2) return "UIDOUBLE";
	if (index==3) return "UIDOUBLE";
	if (index==4) return "UIDOUBLE";
	if (index==5) return "UIDOUBLE";
	if (index==6) return "UIDOUBLE";
	if (index==7) return "UIDOUBLE";
	if (index==8) return "UIDOUBLE";
	if (index==9) return "UIDOUBLE";
	if (index==10) return "UIDOUBLE";
	if (index==11) return "UICHECKBOX";
	if (index==12) return "UIDOUBLE";
	if (index==13) return "UIDOUBLE";
	return "";
}
const char *uiParameterUnits(int index) {
	if (index==0) return "us";
	if (index==1) return "mm";
	if (index==2) return "mm";
	if (index==3) return "";
	if (index==4) return "us";
	if (index==5) return "us";
	if (index==6) return "";
	if (index==7) return "";
	if (index==8) return "us";
	if (index==9) return "";
	if (index==10) return "";
	if (index==11) return "";
	if (index==12) return "ms";
	if (index==13) return "degree";
	return "";
}
double uiParameterMin(int index) {
	if (index==0) return 10;
	if (index==1) return 40;
	if (index==2) return 40;
	if (index==3) return 32;
	if (index==4) return 5;
	if (index==5) return 1;
	if (index==6) return 1;
	if (index==7) return 1;
	if (index==8) return 10;
	if (index==9) return 1;
	if (index==10) return 1;
	if (index==11) return 0;
	if (index==12) return 1;
	if (index==13) return 0.1;
	return 1;
}
double uiParameterMax(int index) {
	if (index==0) return 800;
	if (index==1) return 600;
	if (index==2) return 600;
	if (index==3) return 312;
	if (index==4) return 500;
	if (index==5) return 50;
	if (index==6) return 100000;
	if (index==7) return 128;
	if (index==8) return 800;
	if (index==9) return 50;
	if (index==10) return 100000;
	if (index==11) return 0;
	if (index==12) return 500;
	if (index==13) return 90;
	return 10;
}
double uiParameterStep(int index) {
	if (index==0) return 1;
	if (index==1) return 2;
	if (index==2) return 2;
	if (index==3) return 1;
	if (index==4) return 1;
	if (index==5) return 1;
	if (index==6) return 1;
	if (index==7) return 1;
	if (index==8) return 1;
	if (index==9) return 1;
	if (index==10) return 1;
	if (index==11) return 1;
	if (index==12) return 1;
	if (index==13) return 0.1;
	return 1;
}
double uiParameterFactor(int index) {
	if (index==0) return 1;
	if (index==1) return 1;
	if (index==2) return 1;
	if (index==3) return 1;
	if (index==4) return 1;
	if (index==5) return 1;
	if (index==6) return 1;
	if (index==7) return 1;
	if (index==8) return 1;
	if (index==9) return 1;
	if (index==10) return 1;
	if (index==11) return 1;
	if (index==12) return 0.001;
	if (index==13) return 1;
	return 1;
}
const char *uiParameterDefault(int index) {
	if (index==0) return "20";
	if (index==1) return "250";
	if (index==2) return "250";
	if (index==3) return "128";
	if (index==4) return "20";
	if (index==5) return "5";
	if (index==6) return "400";
	if (index==7) return "1";
	if (index==8) return "1000";
	if (index==9) return "20";
	if (index==10) return "400";
	if (index==11) return "0";
	if (index==12) return "15000";
	if (index==13) return "4";
	return "1";
}
int getResolution() {
return 0;
}
