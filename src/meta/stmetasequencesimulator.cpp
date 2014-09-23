/****************************************************************************
 This file is part of SequenceTree
 Copyright (C) 2005-2008 Jeremy Magland (Jeremy.Magland@gmail.com)

 SequenceTree is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 SequenceTree is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with SequenceTree; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*****************************************************************************/


#include "stmetasequencesimulator.h"
//#include "stsimscanner.h"
#include "stcommon.h"
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QSettings>
#include <QApplication>

STMetaSequenceSimulator::STMetaSequenceSimulator() {
	m_sequence=0;
	m_is_compiled=false;
	m_max_RF_amplitude=0;
	m_total_duration=0;
	m_num_blocks=0;
	m_total_SAR=0;
	m_first_block_index=0;
	
	m_compile_process=new QProcess(this);
	m_simulate_process=new QProcess(this);
	connect(m_compile_process,SIGNAL(readyReadStandardError()),this,SLOT(slot_compile_message()),Qt::DirectConnection);
	connect(m_compile_process,SIGNAL(readyReadStandardOutput()),this,SLOT(slot_compile_message()),Qt::DirectConnection);
	connect(m_compile_process,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(slot_compile_finished()),Qt::DirectConnection);
	
	connect(m_simulate_process,SIGNAL(readyReadStandardError()),this,SLOT(slot_simulate_message()),Qt::DirectConnection);
	connect(m_simulate_process,SIGNAL(readyReadStandardOutput()),this,SLOT(slot_simulate_message()),Qt::DirectConnection);
	connect(m_simulate_process,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(slot_simulate_finished()),Qt::DirectConnection);
	
	cleanup_old_executables();
}
STMetaSequenceSimulator::~STMetaSequenceSimulator() {
	if (!m_executable_fname.isEmpty()) {
		QFile(m_executable_fname).remove();
		QFile(m_executable_fname+".manifest").remove();
	}
}
void STMetaSequenceSimulator::setMetaSequence(STMetaSequence *Seq) {
	m_sequence=Seq;
	m_is_compiled=false;
	m_max_RF_amplitude=0;
	clearBlocks();
}
STSimScannerBlock *STMetaSequenceSimulator::retrieveBlock(long block_index) {
	if (!m_is_compiled) return 0;
	long adjusted_index=block_index-m_first_block_index;
	if ((adjusted_index<0)||(adjusted_index>=m_blocklist.count())) {
		run_sequence(qMax<long>(0,block_index-50),block_index+50,"run");
	}
	adjusted_index=block_index-m_first_block_index;
	if ((adjusted_index<0)||(adjusted_index>=m_blocklist.count())) {
		return 0;
	}
	return m_blocklist.block(adjusted_index);
}
bool STMetaSequenceSimulator::isCompiled() {
	return m_is_compiled;
}
void STMetaSequenceSimulator::compile(bool wait_for_finished) {
	QSettings settings("Magland","SequenceTree4");
	if (!m_executable_fname.isEmpty()) {
		QFile::remove(m_executable_fname);
		QFile::remove(m_executable_fname+".manifest");
	}
	if (m_compile_process->state()!=QProcess::NotRunning) return;
	emit compilationStarted();
	if (!m_sequence) return;
	QString foundation_classes_header=srcDirectory()+"foundationclasses.h";
	QString foundation_classes_source=srcDirectory()+"foundationclasses.cpp";
	QString custom_classes_header=srcDirectory()+"customclasses.h";
	QString custom_classes_source=srcDirectory()+"customclasses.cpp";
	QString create_sequence_source=srcDirectory()+"createsequence.cpp";
	QString load_parameters_file_source=srcDirectory()+"loadparametersfile.cpp";
	#ifdef WIN32
	QString defaultval="mingw";
	#else
		#ifdef Q_WS_MAC
		QString defaultval="mac";
		#else
		QString defaultval="linux";
		#endif
	#endif
	QString compiler_str=settings.value("compiler",defaultval).toString();
	#ifdef WIN32
	QString compile_script=srcDirectory()+"compile-"+compiler_str+".bat";
	#else
	QString compile_script="/bin/sh \"" +
		srcDirectory()+"compile-"+compiler_str+".sh\""; //thanks Martin!
	#endif
	#ifdef WIN32
	QString executable=binDirectory()+"simulator.exe";
	#else
	QString executable=binDirectory()+"simulator";
	#endif
	QFile::remove(executable);
	
	qDebug()  << QString("Writing %1...").arg(foundation_classes_header);
	m_sequence->writeFoundationClassesHeader(foundation_classes_header);
	qDebug()  << QString("Writing %1..").arg(foundation_classes_source);
	m_sequence->writeFoundationClassesSource(foundation_classes_source);
	qDebug()  << QString("Writing %1...").arg(custom_classes_header);
	m_sequence->writeCustomClassesHeader(custom_classes_header);
	qDebug()  << QString("Writing %1..").arg(custom_classes_source);
	m_sequence->writeCustomClassesSource(custom_classes_source);
	qDebug()  << QString("Writing %1..").arg(create_sequence_source);
	m_sequence->writeCreateSequenceSource(create_sequence_source,"sequence_name");
	qDebug()  << QString("Writing %1..").arg(load_parameters_file_source);
	m_sequence->writeLoadParametersFileSource(load_parameters_file_source);
	qDebug()  << QString("Running %1..").arg(compile_script);
	m_compile_process->setWorkingDirectory(srcDirectory());
	#ifdef WIN32
	QStringList env = m_compile_process->systemEnvironment();
	env.replaceInStrings(QRegExp("^PATH=(.*)", Qt::CaseInsensitive), QString("PATH=\\1;")+ST_ROOT_DIR+"\\bin");
	m_compile_process->setEnvironment(env);
	#endif
	m_compile_process->start(compile_script);
	if (wait_for_finished) m_compile_process->waitForFinished();
}
void STMetaSequenceSimulator::computeStatistics(bool wait_for_finished) {
	run_sequence(-1,-1,"stat","",wait_for_finished);
}
double STMetaSequenceSimulator::totalDuration() {
	return m_total_duration;
}
long STMetaSequenceSimulator::numBlocks() {
	return m_num_blocks;
}
double STMetaSequenceSimulator::totalSAR() {
	return m_total_SAR;
}

void STMetaSequenceSimulator::slot_compile_message() {
	QProcess *P=m_compile_process;
	if (!P) return;
	QString msg=P->readAllStandardError()+P->readAllStandardOutput();
	QStringList messages=msg.split("\n");
	foreach (QString message,messages) {
		message.remove(QChar(10));
		message.remove(QChar(13));
		message=message.trimmed();
		if (!message.isEmpty()) emit compilationMessage(message);
	}
}

void STMetaSequenceSimulator::slot_simulate_message() {
	QString msg;
	msg=m_simulate_process->readAllStandardError();
	msg+=m_simulate_process->readAllStandardOutput();
	QStringList messages=msg.split("\n");
	foreach (QString message,messages) {
		message.remove(QChar(10));
		message.remove(QChar(13));
		message=message.trimmed();
		if (!message.isEmpty()) emit simulationMessage(message);
	}
}


void STMetaSequenceSimulator::slot_compile_finished() {
	#ifdef WIN32
		QString executable=binDirectory()+"simulator.exe";
	#else
		QString executable=binDirectory()+"simulator";
	#endif
	bool ret=QFile(executable).exists();
	
	m_is_compiled=ret;
	
	if (m_is_compiled) {
		m_executable_fname=create_new_executable_fname();
		QFile::copy(executable,m_executable_fname);
		QFile::copy(executable+".manifest",m_executable_fname+".manifest");
	}
	
	slot_compile_message(); //clear out the messages
	
	emit compilationFinished();
}

bool STMetaSequenceSimulator::run_sequence(long block_min,long block_max,QString runmode,QString param1,bool wait_for_finished) {
	if (m_simulate_process->state()!=QProcess::NotRunning) return false;
	m_runmode=runmode;
	m_hold_start_block_index=block_min;
	emit simulationStarted();
	clearBlocks();
	if (!m_sequence) return false;
	if (!isCompiled()) return false;
	emit simulationStarted();
	QString parameter_input_file=tmpDirectory()+"sequence_parameters_in.txt";
	m_parameter_output_file=tmpDirectory()+"sequence_parameters_out.txt";
	m_simulation_output_file=tmpDirectory()+"simulation_output.dat";
	m_results_output_file=tmpDirectory()+"results_output.txt";
	QString executable_fname=m_executable_fname;
	
	if (runmode=="raw_template") m_simulation_output_file=param1;
	
	m_results_output_file.replace(".txt","_"+runmode+".txt");

	m_sequence->writeParametersToFile(parameter_input_file);
	QFile::remove(m_parameter_output_file);
	QFile::remove(m_simulation_output_file);
	QFile::remove(m_results_output_file);
	QStringList args;
	args << runmode << parameter_input_file << m_parameter_output_file << m_simulation_output_file << m_results_output_file << QString::number(block_min) << QString::number(block_max);
	qDebug()  << "Running:" << executable_fname << args;
	m_simulate_process->start(executable_fname,args);
	if ((runmode=="check")||(wait_for_finished)) {
		m_simulate_process->waitForFinished(-1);
	}
	return true;
}
void STMetaSequenceSimulator::slot_simulate_finished() {
	emit simulationMessage("Simulation finished...");
	if (m_runmode=="check") return;
	if ((QFile(m_parameter_output_file).exists())&&(QFile(m_simulation_output_file).exists())&&(QFile(m_results_output_file).exists())) {
		if (m_runmode=="run") {
			m_blocklist.read(sstr(m_simulation_output_file));
			m_max_RF_amplitude=m_blocklist.maxRFAmplitude();
			if (m_hold_start_block_index>=0) m_first_block_index=m_hold_start_block_index;
			else m_first_block_index=0;
			m_sequence->loadParametersFromFile(m_parameter_output_file);
			emit runFinished(true);
		}
		if (m_runmode=="stat") {
			m_sequence->loadParametersFromFile(m_parameter_output_file);
			load_statistics_from_file(m_results_output_file);
			emit statFinished(true);
		}
		return;
	}
	else {
		emit simulationMessage("Unexpected problem in simulation, output file does not exist.");
		return;
	}
}

bool STMetaSequenceSimulator::determine_success_from_file(QString fname) {
	QString txt=read_text_file(fname);
	QStringList list=txt.split("\n");
	QHash<QString,QString> values;
	foreach (QString line,list) {
		QStringList list2=line.split("=");
		if (list2.count()==2)
			values[list2[0]]=list2[1];
	}
	m_total_duration=0;
	m_num_blocks=0;
	if (values.contains("ret")) return values["ret"]=="true";
	return false;
}

void STMetaSequenceSimulator::load_statistics_from_file(QString fname) {
	QString txt=read_text_file(fname);
	QStringList list=txt.split("\n");
	QHash<QString,QString> values;
	foreach (QString line,list) {
		QStringList list2=line.split("=");
		if (list2.count()==2)
			values[list2[0]]=list2[1];
	}
	m_total_duration=0;
	m_num_blocks=0;
	m_total_SAR=0;
	if (values.contains("duration")) m_total_duration=values["duration"].toDouble();
	if (values.contains("num_blocks")) m_num_blocks=values["num_blocks"].toLong();
	if (values.contains("SAR")) m_total_SAR=values["SAR"].toDouble();
}

QString STMetaSequenceSimulator::create_new_executable_fname() {
	QString basename=binDirectory()+"simulator_executable";
	QString ret;
	int number=1;
	bool done=false;
	while (!done) {
		ret=QString("%1%2.exe").arg(basename).arg(number);
		if (!QFile(ret).exists()) done=true;
		else number++;
	}
	return ret;
}

void STMetaSequenceSimulator::cleanup_old_executables() {
	QDir dir(ST_BIN_DIR);
	QStringList list=dir.entryList(QStringList("simulator_executable*"));
	foreach (QString str,list) {
		QString fname=dir.path()+"/"+str;
		if (QFileInfo(fname).lastModified().daysTo(QDateTime::currentDateTime())>1)
			QFile::remove(fname);
	}
}

bool STMetaSequenceSimulator::check_sequence() {
	run_sequence(-1,-1,"check");
	return determine_success_from_file(tmpDirectory()+"results_output_check.txt");
}
void STMetaSequenceSimulator::minimizeParameter(STMetaNode *N,QString param_name) {
	if (!N) return;
	STMetaNodeParameter P=N->parameter(param_name);
	if (!check_sequence()) return;
	double maxval=P.value.toDouble();
	double minval=0;
	bool done=false;
	while (!done) {
		double tryval=(maxval+minval)/2;
		P.value=QString::number(tryval);
		N->setParameter(param_name,P);
		if (check_sequence())
			maxval=tryval;
		else
			minval=tryval;
		if (maxval-minval<10) done=true;
	}
	double val=((long)(maxval/10+0.999999))*10;
	P.value=QString::number(val);
	N->setParameter(param_name,P);
}
void STMetaSequenceSimulator::maximizeParameter(STMetaNode *N,QString param_name) {
	if (!N) return;
	STMetaNodeParameter P=N->parameter(param_name);
	if (!check_sequence()) return;
	double minval=P.value.toDouble();
	double maxval=minval*10;
	bool done=false;
	while (!done) {
		double tryval=(maxval+minval)/2;
		P.value=QString::number(tryval);
		N->setParameter(param_name,P);
		if (check_sequence())
			minval=tryval;
		else
			maxval=tryval;
		if (maxval-minval<10) done=true;
	}
	double val=((long)(maxval/10))*10;
	P.value=QString::number(val);
	N->setParameter(param_name,P);
}

void STMetaSequenceSimulator::writeRawTemplateFile(QString fname) {
	run_sequence(-1,-1,"raw_template",fname,true);
}

bool STMetaSequenceSimulator::runEntireSequence(bool wait_for_finished) {
	if (!m_is_compiled) return false;
	run_sequence(-1,-1,"run","",wait_for_finished);
	return true;
}

void STMetaSequenceSimulator::setDirectories(QString bin_dir,QString tmp_dir,QString src_dir) {
	#ifdef WIN32
	m_bin_directory=bin_dir+"\\";
	m_tmp_directory=tmp_dir+"\\";
	m_src_directory=src_dir+"\\";
	#else
	m_bin_directory=bin_dir+"/";
	m_tmp_directory=tmp_dir+"/";
	m_src_directory=src_dir+"/";
	#endif
}

