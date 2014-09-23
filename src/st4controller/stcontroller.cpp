#include "stcontroller.h"
#include <QTimer>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QStringList>
#include <QSettings>

STController::STController() 
{
	m_time_interval=2;
	m_paused=false;
}

STController::~STController()
{
}

void STController::slot_timer() {
	static long ct=0;
	//qDebug() << "TIMER" << ct;
	ct++;

	if (!m_paused) {	
		check_for_new_raw_data();
		check_for_new_sequences();
	}
	
	QTimer::singleShot(m_time_interval*1000,this,SLOT(slot_timer()));
}

void STController::processRawData() {
	QString raw_data_dirname=get_raw_data_dir();
	QString seq_bin_dirname=get_seq_bin_dir();
	QDir raw_data_dir(raw_data_dirname);
	QStringList raw_filters; raw_filters << "*.out" << "*.dat";
	QStringList list=raw_data_dir.entryList(raw_filters,QDir::Files);
	foreach (QString fname,list) {
		emit message(QString("Processing raw data file: %1").arg(raw_data_dirname+"/"+fname),false);
		int num=1;
		while (QFile(raw_data_dirname+"/"+fname+".p"+QString::number(num)).exists(),false)
			num++;
		QString new_fname=raw_data_dirname+"/"+fname+".p"+QString::number(num);
		QFile::rename(raw_data_dirname+"/"+fname,new_fname);	
		emit message(QString("Done processing raw data file: %1").arg(new_fname),false);
	}
}

QString get_raw_data_dir() {
	QSettings settings("Magland","ST4Controller");
	return settings.value("raw_data_dir").toString();
}
QString get_seq_bin_dir() {
	QSettings settings("Magland","ST4Controller");
	return settings.value("seq_bin_dir").toString();
}

void STController::check_for_new_raw_data() {
	QString raw_data_dirname=get_raw_data_dir();
	QDir raw_data_dir(raw_data_dirname);
	QStringList raw_filters; raw_filters << "*.out" << "*.dat";
	QStringList list=raw_data_dir.entryList(raw_filters,QDir::Files);
	QStringList raw_data_files;
	bool something_changed=false;
	foreach (QString fname,list) {
		QString hold=raw_data_dirname+"/"+fname;
		if (!m_raw_data_files.contains(hold)) {
			emit message(QString("Raw data file detected: %1").arg(hold),true);
			something_changed=true;
		}
		raw_data_files << hold;
	}
	if (!something_changed) {
		if (raw_data_files.count()!=m_raw_data_files.count())
			something_changed=true;
	}
	m_raw_data_files=raw_data_files;
	if (something_changed) emit rawDataFilesChanged();
}
void STController::check_for_new_sequences() {
	QString seq_bin_dirname=get_seq_bin_dir();
	QDir sequence_dir(seq_bin_dirname);
	QStringList seq_filters; seq_filters << "*.sts";
	QStringList list=sequence_dir.entryList(seq_filters,QDir::Files);
	QStringList sequence_files;
	bool something_changed=false;
	foreach (QString fname,list) {
		QString hold=seq_bin_dirname+"/"+fname;
		if (!m_sequence_files.contains(hold)) {
			emit message(QString("Sequence file detected: %1").arg(hold),true);
			something_changed=true;
		}
		sequence_files << hold;
	}
	if (!something_changed) {
		if (sequence_files.count()!=m_sequence_files.count())
			something_changed=true;
	}
	m_sequence_files=sequence_files;
	if (something_changed) emit sequenceFilesChanged();
}


QByteArray extract_siemens_vb_header(QString raw_fname) {
	FILE *inf=fopen(raw_fname.toAscii().data(),"rb");
	if (!inf) return "";
	QByteArray ret;
	quint32 header_size;
	fread(&header_size,sizeof(quint32),1,inf);
	qDebug() << "HEADER SIZE:" << header_size;
	if (header_size<=4) return "";
	if (header_size>1000000) header_size=1000000;
	for (long j=0; j<header_size-4; j++) {
		char c;
		fread(&c,1,1,inf);
		if (c>0) ret.append(c);
	}
	fclose(inf);
	return ret;
}

