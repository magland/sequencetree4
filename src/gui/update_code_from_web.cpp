#include "update_code_from_web.h"

#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QDebug>
#include "stcommon.h"
#include "httpdownloader.h"
#include "stmetaclass.h" //for read_text_file

void update_code_from_web(QString url_src_dir,QString dst_dir) {
	if (url_src_dir.isEmpty()) {
		//url_src_dir="c:/dev/st4code";
		url_src_dir="http://sequencetree.googlecode.com/svn/trunk/%20code";
	}
	if (dst_dir.isEmpty()) {
		dst_dir=ST_CODE_DIR;	
	}
	
	
	qDebug()  << "--->" << dst_dir;
	HttpDownloader HD;
	QString url=url_src_dir+"/_filelist.txt";
	QString txt1=HD.downloadPage(url);
	if (txt1.isEmpty()) {
		qWarning() << "Unable to download: " << url;
		qWarning() << "Perhaps you are not connected to the internet.";
		return;
	}
	
	QString fname2=dst_dir+"/_filelist.txt";
	QString txt2=read_text_file(fname2);
	
	QStringList list1=txt1.split('\n');
	QStringList list2=txt2.split('\n');
	QHash<QString,QString> hash1;
	QHash<QString,QString> hash2;
	foreach (QString line,list1) {
		QStringList words=line.split('\t');
		if (words.count()==2) {
			hash1[words.value(0)]=words.value(1);
		}
	}
	foreach (QString line,list2) {
		QStringList words=line.split('\t');
		if (words.count()==2) {
			hash2[words.value(0)]=words.value(1);
		}
	}
	
	QStringList keys1=hash1.keys();
	QStringList keys2=hash2.keys();
	foreach (QString key1,keys1) {
		bool already_exists=hash2.contains(key1);
		if (hash1[key1].indexOf("FOLDER")==0) {
			QDir(dst_dir).mkdir(key1);
			update_code_from_web(url_src_dir+"/"+key1,dst_dir+"/"+key1);				
		}
		else {
			if ((!already_exists)||(QDateTime::fromString(hash2[key1])<QDateTime::fromString(hash1[key1]))) {
				QString filetxt=HD.downloadPage(url_src_dir+"/"+key1);
				if (filetxt.isEmpty()) {
					qWarning() << "Could not download:" << key1;
					hash1[key1]=hash2[key1]; //keep old date
				}
				else {
					qDebug()  << "Updating file:" << key1;
					write_text_file(dst_dir+"/"+key1,filetxt);
				}		
			}
		}
	}
	foreach (QString key2,keys2) {
		if (!hash1.contains(key2)) {
			qDebug()  << "Removing file:" << key2;
			QFile::remove(dst_dir+"/"+key2);
		}
	}
	
	write_text_file(fname2,txt1);
}

