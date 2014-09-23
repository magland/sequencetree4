#include "readrawdatadlg.h"
#include <QDebug>


void erase_all_files_in_directory(QString dirname) {
	QDir dir(dirname);
	if (!dir.exists()) {
		qWarning() << __FUNCTION__ << "directory does not exist:" << dirname;
		return;
	}
	QStringList filelist=dir.entryList(QStringList("*"),QDir::Files|QDir::NoDotAndDotDot);
	foreach (QString fname,filelist) {
		if (!dir.remove(fname)) {
			qWarning() << __FUNCTION__ << "Unable to erase file:" << fname;
		}
	}
}
