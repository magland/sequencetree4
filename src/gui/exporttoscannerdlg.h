#ifndef ExportToScannerDlg_H
#define ExportToScannerDlg_H

#include "ui_exporttoscannerdlg.h"
#include "stmetasequence.h"

class ExportToScannerDlg : public QDialog {
private:
	Ui::ExportToScannerDlg ui;
public:
	ExportToScannerDlg(QWidget *parent=0) : QDialog(parent) {
		ui.setupUi(this);
		m_sequence=0;
		initialize_controls();
	}
	void setSequence(STMetaSequence *S,QString executable_fname);
protected:
	void accept();
private:
	STMetaSequence *m_sequence;
	QString m_executable_fname;

	void initialize_controls();
	QString export_directory();
};

bool export_to_siemens(STMetaSequence *Seq,QString scanner_directory,QString sequence_directory,QString executable_fname);

#endif
