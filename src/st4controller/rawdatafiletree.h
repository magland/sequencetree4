#ifndef rawdatafiletree_H
#define rawdatafiletree_H

#include <QFileInfo>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include "stcontroller.h"
#include <QDateTime>

QString file_size_string(long num_bytes);
QString get_sequence_name(QString raw_fname);
int get_num_channels(QString raw_fname);

class RawDataFileTreeItem : public QTreeWidgetItem {
public:
	RawDataFileTreeItem(QString fname) {
		m_file_name=fname;
		m_sequence_name=get_sequence_name(fname);
		m_num_channels=get_num_channels(fname);
		setText(0,QFileInfo(fname).fileName());
		setText(1,QFileInfo(fname).lastModified().toString());
		setText(2,file_size_string(QFileInfo(fname).size()));
		
		QString holdtxt=m_sequence_name;
		if (!QFileInfo(get_seq_bin_dir()+"/"+m_sequence_name+".sts").exists()) 
			holdtxt+=" (not found)";
		setText(3,holdtxt);
		
		setText(4,QString::number(m_num_channels));
	}
	QString fileName() {return m_file_name;}
	QString sequenceName() {return m_sequence_name;}
	int numChannels() {return m_num_channels;}
private:
	QString m_file_name;
	QString m_sequence_name;
	int m_num_channels;
};

class RawDataFileTree : public QTreeWidget {
public:
	RawDataFileTree(QWidget *parent=0);
	virtual ~RawDataFileTree();
	void initialize();
	void addFile(QString fname) {
		RawDataFileTreeItem *item=new RawDataFileTreeItem(fname);
		addTopLevelItem(item);
	}
	QString currentFile() {
		RawDataFileTreeItem *item=(RawDataFileTreeItem *)currentItem();
		if (!item) return "";
		return item->fileName();
	}
};

QString read_text_file(QString fname);

#endif


