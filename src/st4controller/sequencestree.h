#ifndef sequencestree_H
#define sequencestree_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QFileInfo>

class SequencesTreeItem : public QTreeWidgetItem {
public:
	SequencesTreeItem(QString sequence_fname) {
		m_sequence_file_name=sequence_fname;
		setText(0,QFileInfo(sequence_fname).fileName());
		QString hold="";
		QStringList extensions; extensions << "sts" << "exe" << "st4parameters";
		foreach (QString ext, extensions) {
			if (QFileInfo(baseFileName()+"."+ext).exists()) {
				if (!hold.isEmpty()) hold+=",";
				hold+="."+ext+" ";
			}
		}
		setText(1,hold);
	}
	QString stsFileName() {return m_sequence_file_name;}
	QString exeFileName() {return baseFileName()+".exe";}
	QString parametersFileName() {return baseFileName()+".st4parameters";}
	QString baseFileName() {return QFileInfo(m_sequence_file_name).path()+"/"+QFileInfo(m_sequence_file_name).baseName();};
private:
	QString m_sequence_file_name;
};

class SequencesTree : public QTreeWidget {
public:
	SequencesTree(QWidget *parent=0);
	virtual ~SequencesTree();
	void initialize();
	void addSequence(QString sequence_fname);
};

#endif
