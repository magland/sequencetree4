#ifndef createsequence_H
#define createsequence_H

#include "stsequence.h"

STSequence *createSequence(char *parameter_fname);
bool setGlobalParameter(STSequence *Seq,SString pname,SString pval);

void getSequenceName(char *ret);
int uiParameterCount();
const char *uiParameterName(int index);
const char *uiParameterLabel(int index);
const char *uiParameterType(int index);
const char *uiParameterUnits(int index);
double uiParameterMin(int index);
double uiParameterMax(int index);
double uiParameterStep(int index);
double uiParameterFactor(int index);
const char *uiParameterDefault(int index);
int getResolution();

#endif
