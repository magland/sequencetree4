#include "stsimview.h"
#include <QFile>
#include <QProcess>

STSimView::STSimView(QWidget *par) : QAbstractScrollArea(par) {
	m_sequence=0;
	m_is_compiled=false;
}
STSimView::~STSimView() {
}

void STSimView::setSequence(STMetaSequence *S) {
	m_sequence=S;
	setCurrentBlockNumber(0);
}

long STSimView::currentBlockNumber() {
	return m_current_block_number;
}
void STSimView::setCurrentBlockNumber(long val) {
	m_current_block_number=val;
	update_plots();
}

void STSimView::update_plots() {
}

bool STSimView::simulate() {
	if (!m_sequence) return false;
	if (!isCompiled()) return false;
	QString parameter_file="c:\\dev\\st4\\simulator\\sequence_parameters.txt";
	QString output_file="c:\\dev\\st4\\simulator\\simulation_output.dat";
	QString executable_fname="c:\\dev\\st4\\simulator\\simulator.exe";
	m_sequence->writeParametersToFile(parameter_file);
	QFile::remove(output_file);
	QStringList args;
	args << parameter_file << output_file;
	QProcess::startDetached(executable_fname,args);
	return QFile(output_file).exists();
}

