#include "httpdownloader.h"
#include <QUrl>
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QString>

QString read_text_file_httpdownloader(QString fname) {
	QFile file(fname);
	if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
		return "";
	QString ret=file.readAll();
	file.close();
	return ret;
}


HttpDownloader::HttpDownloader() 
{
	m_busy=false;
	m_done=false;
	connect(&m_http,SIGNAL(dataReadProgress(int,int)),this,SLOT(slot_data_read_progress(int,int)));
	connect(&m_http,SIGNAL(done(bool)),this,SLOT(slot_done(bool)));
	connect(&m_http,SIGNAL(requestFinished(int,bool)),this,SLOT(slot_request_finished(int,bool)));
	connect(&m_http,SIGNAL(requestStarted(int)),this,SLOT(slot_request_started(int)));
	
}

HttpDownloader::~HttpDownloader()
{
}


QString HttpDownloader::downloadPage(QString url) {	
	if (m_busy) return "";
	if (url.mid(1,1)==":") { //like c:, must be name of file on local system
		return read_text_file_httpdownloader(url);
	}
	QUrl Url(url);
	m_busy=true;
	m_done=false;
	m_data="";
	QString urlpath=Url.path();
	urlpath.replace(" ","%20");
	m_http.setHost(Url.host());
	m_http.get(urlpath);
	while (!m_done) {
		qApp->processEvents();
	}
	m_busy=false;
	return m_data;
}

void HttpDownloader::slot_data_read_progress(int vdone,int total) {
}
void HttpDownloader::slot_done(bool err) {
	m_done=true;
	m_data=m_http.readAll();
	m_data.replace(QString("\r")+"\n","\n");
	
}
void HttpDownloader::slot_request_finished(int id,bool err) {
}
void HttpDownloader::slot_request_started(int id) {
}
