#ifndef stcontroller_H
#define stcontroller_H

#include <QObject>
#include <QSet>
#include <QStringList>

class STController : public QObject {
	Q_OBJECT
public:
	STController();
	virtual ~STController();
	void setTimeInterval(double val) {m_time_interval=val;}
	bool paused() {return m_paused;}
	void setPaused(bool val) {m_paused=val;}
	void startTimer() {slot_timer();}
	QStringList rawDataFiles() {return m_raw_data_files;}
	QStringList sequenceFiles() {return m_sequence_files;}
signals:
	void message(QString msg,bool bold);
	void rawDataFilesChanged();
	void sequenceFilesChanged();
public slots:
	void processRawData(); //to be deleted
private slots:
	void slot_timer();
private:
	double m_time_interval; //seconds
	bool m_paused;
	
	QStringList m_raw_data_files;
	QStringList m_sequence_files;
	
	void check_for_new_raw_data();
	void check_for_new_sequences();
	
	
};

QString get_raw_data_dir();
QString get_seq_bin_dir();

QByteArray extract_siemens_vb_header(QString raw_fname);

#endif
