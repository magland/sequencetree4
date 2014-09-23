#include "vssequenceview.h"

void VSSequenceView::slot_global_parameter_value_changed(QString name,QString value) {
	if (!m_simulator) return;
	if (!m_simulator->metaSequence()) return;
	m_simulator->metaSequence()->setGlobalParameterValue(name,value);
	emit clearSimulateLog();
	updateView();
}