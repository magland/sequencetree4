/* BEGIN EXCLUDE */
#include "stloop.h"
#include "stscanner.h"
#include "stsequence.h"
/* END EXCLUDE */

STLoop::STLoop() {
	ST_CLASS(STLoop,STNode)
}

bool STLoop::loopInitialize() {
	m_iterators.clear();
	{for (int j=0; j<m_parameters.count(); j++) {
		if (m_parameters[j]->objectType()=="STIterator") {
			m_iterators.append((STIterator *)(m_parameters[j]));
		}
	}}
	{for (int j=0; j<m_iterators.count(); j++) {
		(*m_iterators[j])=m_iterators[j]->minimum();
		if (scanner()) scanner()->iteratorInitialized(j);
	}}
	return true;
}
bool STLoop::loopStep() {
	if (sequence()->checkMode()) {
		for (int j=0; j<m_iterators.count(); j++) {
			if (m_iterators[j]->incrementCheck()) {
				return true;
			}
			else (*m_iterators[j])=m_iterators[j]->minimum();
		}
		return false;
	}
	else {
		for (int j=0; j<m_iterators.count(); j++) {
			if (m_iterators[j]->increment()) {
				if (scanner()) scanner()->iteratorStepped(j);
				return true;
			}
			else {
				(*m_iterators[j])=m_iterators[j]->minimum();
				if (scanner()) scanner()->iteratorReset(j);
			}
		}
		return false;
	}
}
bool STLoop::loopRun() {
	return true;
}
bool STLoop::run() {
	if (!scanner()) return false;
	if (!loopInitialize()) return false;
	if (isModified()) prepare(); //don't worry if this fails
	bool done=false;
	while (!done) {	
		if (!loopRun()) return false;
		if (!loopStep()) {
			done=true;
		}
		if (scanner()->doneScanning()) done=true;
	}	
	return true;
}





