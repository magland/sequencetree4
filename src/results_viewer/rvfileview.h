#ifndef RVFileView_H
#define RVFileView_H

#include "ui_rvfileview.h"
#include "stcommon.h"
#include <QDebug>
#include <QDir>

class RVFileViewItem : public QTreeWidgetItem {
public:
	RVFileViewItem(QString fname);
	QString fileName() {return m_file_name;}
private:
	QString m_file_name;
};

class RVFileView : public QWidget {
	Q_OBJECT
private:
	Ui::RVFileView ui;
public:
	RVFileView(QWidget *parent=0);
	void setDirectory(QString directory);
	QString directory() {return m_directory;}
	void refresh();
	void openExperiment() {slot_open();}
	void saveExperiment() {slot_save();}
	void configuration();
signals:
	void fileActivated(QString fname);
private slots:
	void slot_reload();
	void slot_save();
	void slot_open();
	
	void slot_item_activated(QTreeWidgetItem *item);
private:
	QString m_directory;
	
	QString experiments_folder();
};

#endif
