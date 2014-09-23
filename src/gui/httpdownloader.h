#ifndef httpdownloader_H
#define httpdownloader_H

#include <QHttp>

class HttpDownloader : public QObject {
	Q_OBJECT
public:
	HttpDownloader();
	virtual ~HttpDownloader();
	
	QString downloadPage(QString url);
private slots:
	void slot_data_read_progress(int,int);
	void slot_done(bool);
	void slot_request_finished(int,bool);
	void slot_request_started(int);
private:
	QHttp m_http;
	bool m_busy;
	bool m_done;
	QByteArray m_data;
};

#endif
