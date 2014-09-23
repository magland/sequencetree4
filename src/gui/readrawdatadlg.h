#ifndef ReadRawDataDlg_H
#define ReadRawDataDlg_H

#include "ui_readrawdatadlg.h"
#include <QSettings>
#include "stcommon.h"
#include <QDir>
#include <QFileDialog>
#include "distribute_raw_data.h"
#include <QProcess>
#include <QFile>
#include <QDir>

#include "stmetaclass.h"

void erase_all_files_in_directory(QString dirname);

class ReadRawDataDlg : public QDialog {
	Q_OBJECT
private:
	Ui::ReadRawDataDlg ui;
public:
	ReadRawDataDlg(QWidget *parent=0) : QDialog(parent) {
		ui.setupUi(this);
	}
	void setRawDataTemplate(QString fname) {
		ui.raw_data_template->setText(fname);
		QSettings settings("Magland","SequenceTree4");
		ui.num_channels->setText(settings.value("num_channels","1").toString());
		ui.raw_data->setText(settings.value("raw_data").toString());
		ui.siemens_va_format->setChecked(settings.value("raw_data_format").toString()=="siemens_va");
		ui.siemens_vb_format->setChecked(settings.value("raw_data_format").toString()=="siemens_vb");		
		
		connect(ui.browse_raw_data_button,SIGNAL(clicked()),this,SLOT(slot_browse_raw_data()));
		slot_browse_raw_data();
	}
	void accept() {
		QSettings settings("Magland","SequenceTree4");
		settings.setValue("num_channels",ui.num_channels->text());
		settings.setValue("raw_data",ui.raw_data->text());
		
		QDir(ST_TMP_DIR).mkdir("data");
		
		QDir(ST_TMP_DIR).mkdir("data");
		QString data_dir=ST_TMP_DIR+"/data";
		erase_all_files_in_directory(data_dir);		
		
		
		DistributeRawDataStruct X;
		X.output_directory=ST_TMP_DIR+"/data";
		X.template_fname=ui.raw_data_template->text();
		X.data_fname=ui.raw_data->text();
		X.num_channels=ui.num_channels->text().toInt();
		if (ui.siemens_va_format->isChecked()) {
			X.header_size=128;
			X.raw_data_format=RAW_DATA_FORMAT_SIEMENS_VA;
			settings.setValue("raw_data_format","siemens_va");
		}
		else if (ui.siemens_vb_format->isChecked()) {
			X.header_size=128;
			X.raw_data_format=RAW_DATA_FORMAT_SIEMENS_VB;
			settings.setValue("raw_data_format","siemens_vb");
		}
		else {
			X.header_size=128;
			X.raw_data_format=RAW_DATA_FORMAT_SIEMENS_VA;
		}
		
		distribute_raw_data(X);
		
		QDialog::accept();
	}
private slots:
	void slot_browse_raw_data() {
		QString fname=QFileDialog::getOpenFileName(this,"Choose raw data file:",ui.raw_data->text());
		if (!fname.isEmpty())
			ui.raw_data->setText(fname);
	}
	
};

#endif
